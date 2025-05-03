// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#ifndef KONKR_GAME_LOGIC_GAME_H
#define KONKR_GAME_LOGIC_GAME_H

#include "rendering/level.h"
#include "game_logic/player.h"

namespace konkr {

class Game {
  using PlayersList = std::vector<std::shared_ptr<Player>>;

  public:
    Game(std::shared_ptr<Level> level)
        : level_(level) {
          for (size_t i = 0; i < level_.CountActivePlayers(); i++) {
            players_.push_back(std::make_shared<Player>(new Player(i)));
          }
        }

    Game(PlayersList players, LevelsList levels)
    : players_(std::move(players)),
      levels_(std::move(levels)) {}

    inline const size_t getNumberPlayers() { return players_.size(); }

    inline const std::shared_ptr<Player> getCurPlayer() { return players_[cur_player_idx_]; }

    inline void nextTurn() { cur_player_idx_ = (cur_player_idx_+1) % getNumberPlayers(); };

    void NextLevel();

    const bool CheckEnd();

  private:
    std::shared_ptr<Level> level_;
    size_t cur_player_idx_ = 0;
    size_t cur_level_idx_ = 0;
};

}  // namespace konkr

#endif  // KONKR_GAME_LOGIC_GAME_H