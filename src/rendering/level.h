// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.
//
// level.h
//
// Declares the Level class
//

#ifndef KONKR_RENDERING_LEVEL_H
#define KONKR_RENDERING_LEVEL_H

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace konkr {

class Level {
 public:
  // Creates Level objects for all levels found in the specified directory.
  // The directory should contain subdirectories for each category, and each
  // level file should have a ".level" extension.
  // The level name is derived from the file name, and the category is derived
  // from the directory name.
  // It doesn't load the contents of the level files, just prepares the Level
  // objects.
  static std::vector<Level> GetAvailableLevels(
      const std::filesystem::path& levels_directory);

  Level(std::string name, std::string category, std::filesystem::path file_path)
      : name_(std::move(name)),
        category_(std::move(category)),
        file_path_(std::move(file_path)) {};

  // Loads the level from the file_path_ if not already loaded
  bool Load();

  const std::string& name() const { return name_; }
  const std::string& category() const { return category_; }
  const std::filesystem::path& file_path() const { return file_path_; }
  const std::vector<std::string>& map() const { return map_; }
  bool isLoaded() const { return loaded_; }

  void DisplayMapAscii() const;

 private:
  std::string name_;
  std::string category_;
  std::filesystem::path file_path_;
  std::vector<std::string> map_;
  bool loaded_ = false;
};

}  // namespace konkr

#endif  // KONKR_RENDERING_LEVEL_H