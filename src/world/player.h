// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.
//
// player.h
//
// TODO: add .h description

#ifndef KONKR_GAME_LOGIC_PLAYER_H
#define KONKR_GAME_LOGIC_PLAYER_H

#include <string>

namespace konkr {

// Generates a random warrior name using a combination of prefixes, stems, and
// suffixes. The idea is to have a funny name (with some easter eggs).
std::string GenerateWarriorName();

class Player {
 public:
  Player(int id) : id_(id) { name_ = GenerateWarriorName(); }

  inline std::string name() const { return name_; }
  inline int id() const { return id_; }

 private:
  int id_;
  std::string name_;
};
}  // namespace konkr

#endif
