// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "rendering/level.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <set>

#include "world/entity.h"
#include "world/player.h"

namespace konkr {

// Level names are in snake_case, we want to display them in a more
// human-readable format. Replaces underscores with spaces and capitalizes the
// first letter.
std::string FormatLevelNameForDisplay(std::string name) {
  std::replace(name.begin(), name.end(), '_', ' ');
  name[0] = std::toupper(name[0]);
  return name;
}

std::vector<std::shared_ptr<Level>> Level::GetAvailableLevels(
    const std::filesystem::path& levels_directory) {
  std::vector<std::shared_ptr<Level>> levels;

  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(levels_directory)) {
    if (entry.is_regular_file() && entry.path().extension() == ".level") {
      std::string filename = entry.path().stem().string();
      std::string category = entry.path().parent_path().filename().string();
      category[0] = std::toupper(category[0]);
      std::string formatted_name = FormatLevelNameForDisplay(filename);

      levels.push_back(std::make_shared<Level>(
          std::move(formatted_name), std::move(category), entry.path()));
    }
  }

  return levels;
}

bool Level::Load() {
  map_.clear();
  std::ifstream definition_stream(file_path_);
  if (!definition_stream.is_open()) {
    std::cerr << "Failed to open level file: " << file_path_ << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(definition_stream, line)) {
    map_.push_back(line);
  }

  if (map_.empty()) {
    std::cerr << "No map data found in level file: " << file_path_ << std::endl;
    return false;
  }

  CreateTiles();
  loaded_ = true;
  return true;
}

void Level::DisplayMapAscii() const {
  for (const auto& line : map_) {
    std::cout << line << std::endl;
  }
}

void Level::CreateTiles() {
  tiles_.clear();
  players_.clear();
  tiles_buildings_.clear();
  tiles_.reserve(map_.size());

  int i = 0;
  for (const std::string& line : map_) {
    std::vector<std::shared_ptr<Tile>> row;
    bool indent = !line.empty() && line[0] == '|';
    size_t col_start = indent ? 1 : 0;
    size_t col = col_start;
    int j = 0;
    while (col < line.size()) {
      std::shared_ptr<Tile> tile;
      char c = line[col];
      if (Tile::is_decoration(c)) {
        tile = Tile::FromAscii(c);
        if (Tile::is_forest(c)) {
          tile->set_entity(CreateEntity(Entity::EntityType::Forest));
        }
        ++col;
      } else {
        if (col + 1 < line.size() && std::isdigit(line[col + 1])) {
          int player_id = line[col + 1] - '0';
          tile = Tile::FromAscii(c, player_id);
          // if it's not sand we create an entity
          tile->set_entity(CreateEntity(c));
          if (Entity::is_building(c)) {
            tiles_buildings_.push_back(tile);
            if (Entity::is_townhall(c)) {
              // for each townhall, we create a player
              // if the player doesn't exist
              if (std::none_of(
                      players_.begin(), players_.end(),
                      [player_id](const std::pair<const int, Player>& p) {
                        return p.second.id() == player_id;
                      })) {
                players_.emplace(player_id, Player(player_id));
              }
              // get the player
              auto& player = players_.at(player_id);
              // add the townhall to the player
              player.townhalls_mutable().push_back(
                  std::dynamic_pointer_cast<Townhall>(tile->entity()));
            }
          }
          col += 2;
        } else {
          ++col;
        }
      }
      tile->set_grid_position({i, j++});
      if (tile->entity()) {
        tile->entity()->set_grid_position({i, j - 1});
      }
      row.push_back(std::move(tile));
    }
    tiles_.push_back(std::move(row));
    ++i;
  }
  UpdateTilesLevel();
}

void Level::UpdateTilesLevel() {
  // For each building (e.g., Townhall or Castle), claim connected tiles
  for (const auto& building : tiles_buildings_) {
    if (auto tile = building.lock()) {
      auto connected = GetConnectedOwnedTiles(tile);
      for (auto& t : connected) {
        t->claim();
        // if the tile is a townhall, then update the upkeep cost
        // using the upkeep of the new connected tile
        if (tile->entity()->is_townhall()) {
          tile->entity()->set_upkeep_cost(tile->entity()->upkeep_cost() +
                                          t->entity()->upkeep_cost());
        }
      }
    }
  }

  // if Tile is Townhall or Castle, or neighbor of a Townhall or Castle,
  // then set level_ to 1
  for (const auto& building : tiles_buildings_) {
    if (auto tile = building.lock()) {
      tile->set_level(1);
      auto neighbors = tile->GetNeighboringTilesGridPosition();
      for (const auto& neighbor : neighbors) {
        auto& neighbor_tile = tiles_[neighbor.x][neighbor.y];
        if (neighbor_tile && !Tile::is_decoration(neighbor_tile->type()) &&
            tile->get_owner() == neighbor_tile->get_owner()) {
          neighbor_tile->set_level(1);
        }
      }
    }
  }
}

std::vector<std::shared_ptr<Tile>> Level::GetConnectedOwnedTiles(
    const Vector2i start_tile) {
  return GetConnectedOwnedTiles(tiles_[start_tile.x][start_tile.y]);
}

std::vector<std::shared_ptr<Tile>> Level::GetConnectedOwnedTiles(
    const std::shared_ptr<Tile>& start_tile) {
  std::vector<std::shared_ptr<Tile>> connected_tiles;
  auto owner = start_tile->get_owner();
  if (!owner.has_value()) {
    return connected_tiles;
  }

  std::set<std::pair<int, int>> visited;
  std::queue<std::shared_ptr<Tile>> to_visit;
  to_visit.push(start_tile);

  while (!to_visit.empty()) {
    auto current_tile = to_visit.front();
    to_visit.pop();

    auto pos = current_tile->grid_position();
    auto key = std::make_pair(pos.x, pos.y);
    if (visited.count(key)) continue;
    visited.insert(key);

    if (current_tile->get_owner() == owner &&
        !Tile::is_decoration(current_tile->type())) {
      connected_tiles.push_back(current_tile);
      current_tile->claim();

      for (const auto& neighbor_pos :
           current_tile->GetNeighboringTilesGridPosition()) {
        auto& neighbor_tile = tiles_[neighbor_pos.x][neighbor_pos.y];
        if (neighbor_tile &&
            !visited.count(std::make_pair(neighbor_pos.x, neighbor_pos.y)) &&
            neighbor_tile->get_owner() == owner &&
            !Tile::is_decoration(neighbor_tile->type())) {
          to_visit.push(neighbor_tile);
        }
      }
    }
  }
  return connected_tiles;
}

void Level::UpdateMoney() {
  auto player = get_current_player();
  if (!player) return;

  for (const auto& townhall : player->townhalls()) {
    townhall->set_money(townhall->money() + townhall->upkeep_cost());
    if (townhall->money() < 0) {
      auto connected_tiles = GetConnectedOwnedTiles(townhall->grid_position());
      for (const auto& tile : connected_tiles) {
        if (tile->entity()->is_human_unit()) {
          tile->set_entity(CreateEntity(Entity::EntityType::Bandit));
        }
      }
    }
  }
}

void Level::UpdateActivePlayers() {
  UpdateTilesLevel();
  UpdateMoney();

  if (tiles_.empty()) return;

  std::vector<std::optional<int>> active_players;

  for (size_t row = 0; row < tiles_.size(); ++row) {
    const auto& tile_row = tiles_[row];

    for (size_t col = 0; col < tile_row.size(); ++col) {
      const auto& tile = tile_row[col];
      if (!tile) continue;

      std::optional<int> tile_owner = tile->get_owner();
      // If current "winner" isn't the only one left on the map, then the game
      // isn't over:
      if (tile_owner.has_value()) {
        active_players.push_back(tile_owner);
      }
    }
  }

  for (auto it = players_.begin(); it != players_.end();) {
    bool active = false;
    for (auto pid : active_players) {
      if (pid.has_value() && pid.value() == it->first) {
        active = true;
        break;
      }
    }
    if (!active) {
      if (cur_player_idx_ >= players_.size() - 1) {
        if (cur_player_idx_ > 0) cur_player_idx_--;
      }
      it = players_.erase(it);
    } else {
      ++it;
    }
  }
}

void Level::NextTurn() {
  UpdateActivePlayers();
  cur_player_idx_ = (cur_player_idx_ + 1) % active_players_count();
  std::cout << "Next turn: " << cur_player_idx_ << std::endl;
}

const bool Level::CheckEnd() {
  std::optional<int> winner;
  bool end = true;
  if (tiles_.empty()) return end;

  for (size_t row = 0; row < tiles_.size() && !end; ++row) {
    const auto& tile_row = tiles_[row];

    for (size_t col = 0; col < tile_row.size() && !end; ++col) {
      const auto& tile_opt = tile_row[col];
      if (!tile_opt) continue;

      std::optional<int> tile_owner = ((Tile&)tile_opt).get_owner();
      // If current "winner" isn't the only one left on the map, then the game
      // isn't over:
      if (tile_owner.has_value()) {
        if (!winner.has_value()) {
          winner = tile_owner;
        } else if (winner != tile_owner) {
          end = false;
        }
      }
    }
  }

  return end;
}

}  // namespace konkr