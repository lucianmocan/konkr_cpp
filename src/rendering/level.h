// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for
// details.
//
// level.h
//
// Declares the Level class 
//

#ifndef KONKR_RENDERING_LEVEL_H
#define KONKR_RENDERING_LEVEL_H

#include <string>
#include <vector>
#include <optional>

namespace konkr {

class Level {
 public:
  // Returns a level if the file is successfully loaded
  static std::optional<Level> LoadFromFile(const std::string& file_path);

  const std::string& name() const { return name_; }
  const std::vector<std::string>& map() const { return map_; }

  void DisplayMapAscii() const;
  
 private:
  std::string name_;
  std::vector<std::string> map_;
};

}  // namespace konkr

#endif  // KONKR_RENDERING_LEVEL_H