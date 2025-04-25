// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for
// details.

#include "rendering/level.h"

#include <fstream>
#include <iostream>
#include <optional>

namespace konkr {

std::optional<Level> Level::LoadFromFile(const std::string& file_path) {

    std::ifstream definition_stream(file_path);
    if (!definition_stream.is_open()) {
      std::cerr << "Failed to open level file: "
                << file_path<< std::endl;
      return std::nullopt;
    }

    Level level;
    std::string line;
    while (std::getline(definition_stream, line)) {
      if (level.name_.empty()) {
        level.name_ = line;  // First non-empty line is the level name
      } else {
        level.map_.push_back(line);  // The rest are part of the map
      }
    }

    if (level.map_.empty()) {
      std::cerr << "No map data found in level file: " << file_path << std::endl;
      return std::nullopt;
    }

    return level;
  }

  void Level::DisplayMapAscii() const {
    for (const auto& line : map_) {
      std::cout << line << std::endl;
    }
  }
}