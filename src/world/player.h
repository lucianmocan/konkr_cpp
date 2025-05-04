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

#include "world/townhall.h"

namespace konkr {

// Generates a random warrior name using a combination of prefixes, stems, and
// suffixes. The idea is to have a funny name (with some easter eggs).
std::string GenerateWarriorName();

/**
  @class Represents a player of the game.
*/
class Player {
 public:
  Player(int id) : id_(id) { name_ = GenerateWarriorName(); }

  inline std::string name() const { return name_; }
  inline int id() const { return id_; }
  inline const std::vector<std::shared_ptr<Townhall>>& townhalls() {
    return townhalls_;
  }

  inline std::vector<std::shared_ptr<Townhall>>& townhalls_mutable() {
    return townhalls_;
  }

  inline const int townhall_count() const { return townhalls_.size(); }

 private:
  int id_;
  std::string name_;
  std::vector<std::shared_ptr<Townhall>> townhalls_;
  int selected_townhall_ = 0;
};
}  // namespace konkr

#endif
