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
          if (!Tile::is_sand(c)) {
            std::cout << "Found not sand: " << c << " for player " << player_id
                      << std::endl;
            // if it's not sand we create an entity
            tile->set_entity(CreateEntity(c));
          }
          if (Entity::is_building(c)) {
            tiles_buildings_.push_back(tile);
            if (Entity::is_townhall(c)) {
              // for each townhall, we create a player
              // if the player doesn't exist
              if (std::none_of(players_.begin(), players_.end(),
                               [player_id](const Player& player) {
                                 return player.id() == player_id;
                               })) {
                players_.emplace_back(player_id);
              }
            }
          }
          col += 2;
        } else {
          ++col;
        }
      }
      tile->set_grid_position({i, j++});
      row.push_back(std::move(tile));
    }
    tiles_.push_back(std::move(row));
    ++i;
  }
  UpdateTilesLevel();
}



void Level::UpdateTilesLevel() {
  // if Tile is Townhall or Castle, or neighbor of a Townhall or Castle,
  // then set level_ to 1
  for (const auto& building : tiles_buildings_) {
    if (auto tile = building.lock()) {
      tile->set_level(1);
      tile->claim();
      for (const auto& neighbor : tile->GetNeighboringTilesGridPosition()) {
        auto& neighbor_tile = tiles_[neighbor.x][neighbor.y];
        if (neighbor_tile && !Tile::is_decoration(neighbor_tile->type()) && tile->get_owner() == neighbor_tile->get_owner()) {
          neighbor_tile->set_level(1);
          neighbor_tile->claim();
        }
      }
    }
  }
  
  
}

void Level::UpdateActivePlayers() {
  if (tiles_.empty()) return;

  std::vector<std::optional<int>> active_players;

  for (size_t row = 0; row < tiles_.size(); ++row) {
    const auto& tile_row = tiles_[row];

    for (size_t col = 0; col < tile_row.size(); ++col) {
      const auto& tile_opt = tile_row[col];
      if (!tile_opt) continue;

      std::optional<int> tile_owner = ((Tile&)tile_opt).get_owner();
      // If current "winner" isn't the only one left on the map, then the game
      // isn't over:
      if (tile_owner.has_value()) {
        active_players.push_back(tile_owner);
      }
    }
  }

  for (std::vector<Player>::iterator p = players_.begin(); p != players_.end();
       ++p) {
    bool active = false;
    for (auto pid : active_players) {
      if (pid == (*p).id()) {
        active = true;
      }
    }
    if (!active) {
      if (cur_player_idx_ >= (*p).id()) {
        cur_player_idx_--;  //  Calibrating current player index according to
                            //  the new number of players
      }
      players_.erase(p);
    }
  }
}

void Level::NextTurn() {
  UpdateActivePlayers();
  cur_player_idx_ = (cur_player_idx_ + 1) % activePlayersNb();
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