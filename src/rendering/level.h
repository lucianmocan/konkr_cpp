// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.
//
// level.h
//
// Declares the Level class, which represents a game level
// and allows loading and displaying its contents.
//

#ifndef KONKR_RENDERING_LEVEL_H
#define KONKR_RENDERING_LEVEL_H

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "world/player.h"
#include "world/tile.h"

namespace konkr {

// A game Level has a name, category, and a map.
// It provides methods to load levels from files, retrieve level
// information, and display the map in ASCII format.
class Level {
 public:
  Level(const Level&) = delete;
  Level& operator=(const Level&) = delete;
  Level(Level&&) = default;
  Level& operator=(Level&&) = default;

  // Creates Level objects for all levels found in the specified directory.
  // The directory should contain subdirectories for each category, and each
  // level file should have a ".level" extension.
  // The level name is derived from the file name, and the category is derived
  // from the directory name.
  // It doesn't load the contents of the level files, just prepares the Level
  // objects.
  static std::vector<std::shared_ptr<Level>> GetAvailableLevels(
      const std::filesystem::path& levels_directory);

  Level(std::string name, std::string category, std::filesystem::path file_path)
      : name_(std::move(name)),
        category_(std::move(category)),
        file_path_(std::move(file_path)) {}

  // Loads the level from the file_path_ if not already loaded
  bool Load();

  inline const std::string& name() const { return name_; }
  inline const std::string& category() const { return category_; }
  inline const std::filesystem::path& file_path() const { return file_path_; }
  inline const std::vector<std::string>& map() const { return map_; }
  inline bool is_loaded() const { return loaded_; }

  void DisplayMapAscii() const;

  void CreateTiles();
  inline const std::vector<std::vector<std::optional<Tile>>>& tiles() const {
    return tiles_;
  }

  inline std::shared_ptr<std::vector<Player>> activePlayers() {
    return std::make_shared<std::vector<Player>>(players_);
  }

  inline size_t activePlayersNb() { return activePlayers()->size(); }

  inline const std::shared_ptr<Player> getCurPlayer() { return std::make_shared<Player>(players_[cur_player_idx_]); }

  inline void nextTurn() { cur_player_idx_ = (cur_player_idx_+1) % activePlayersNb(); };

  void NextLevel();

  const bool CheckEnd();

 private:
  std::string name_;
  std::string category_;
  std::filesystem::path file_path_;
  std::vector<std::string> map_;
  std::vector<std::vector<std::optional<Tile>>> tiles_;
  std::vector<Player> players_;
  size_t cur_player_idx_ = 0;
  bool loaded_ = false;
};

}  // namespace konkr

#endif  // KONKR_RENDERING_LEVEL_H