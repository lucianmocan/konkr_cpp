// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "game_logic/game.h"

namespace konkr {

bool Game::CheckEnd() {
  const auto& tiles = this->cur_level_->tiles();
  std::optional<int> winner = -1;
  bool end = true;
  if (tiles.empty()) return end;

  for (size_t row = 0; row < tiles.size() && !end; ++row) {
    const auto& tile_row = tiles[row];

    for (size_t col = 0; col < tile_row.size() && !end; ++col) {
      const auto& tile_opt = tile_row[col];
      if (!tile_opt) continue;

      std:std::optional<int> tile_owner = ((Tile&) tile_opt).getOwner();
      if (winner == -1) {
        winner = tile_owner;
      }
      else if (winner != tile_owner) {
        end = false;
      }
    }
  }

  return end;
}

inline std::optional<int> Game::GetCurPlayer() { return this->cur_player_; }

}