// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "rendering/level.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <optional>

#include "world/entity.h"

namespace konkr {

// Level names are in snake_case, we want to display them in a more
// human-readable format. Replaces underscores with spaces and capitalizes the
// first letter.
std::string FormatLevelNameForDisplay(std::string name) {
  std::replace(name.begin(), name.end(), '_', ' ');
  name[0] = std::toupper(name[0]);
  return name;
}

std::vector<Level> Level::GetAvailableLevels(
    const std::filesystem::path& levels_directory) {
  std::vector<Level> levels;

  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(levels_directory)) {
    if (entry.is_regular_file() && entry.path().extension() == ".level") {
      std::string filename = entry.path().stem().string();
      std::string category = entry.path().parent_path().filename().string();
      std::string formatted_name = FormatLevelNameForDisplay(filename);

      levels.emplace_back(formatted_name, category, entry.path());
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

  for (const std::string& line : map_) {
    std::vector<std::optional<Tile>> row;
    bool indent = !line.empty() && line[0] == '|';
    size_t col_start = indent ? 1 : 0;
    size_t col = col_start;

    while (col < line.size()) {
      std::optional<Tile> tile_opt;
      char c = line[col];
      if (c == '~' || c == '#') {
        tile_opt = Tile::FromAscii(c);
        if (c == '#') {
          tile_opt->setEntity(Entity(Entity::EntityType::Forest));
        }
        ++col;
      } else {
        if (col + 1 < line.size() && std::isdigit(line[col + 1])) {
          int player_id = line[col + 1] - '0';
          tile_opt = Tile::FromAscii(c, player_id);
          if (c != 'S') {
            tile_opt->setEntity(Entity(c));
          }
          col += 2;
        } else {
          ++col;
        }
      }
      row.push_back(tile_opt);
    }
    tiles_.push_back(std::move(row));
  }
}

}  // namespace konkr