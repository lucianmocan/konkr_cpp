// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "rendering/level.h"

namespace konkr {

class Game {
  public:
    Game(int nb_players, std::shared_ptr<Level> cur_level)
        : nb_players_(nb_players),
          cur_level_(cur_level) {}

    bool CheckEnd();
  private:
    int nb_players_;
    int cur_player_ = 0;
    std::shared_ptr<Level> cur_level_;
};

}