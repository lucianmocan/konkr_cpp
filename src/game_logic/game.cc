// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "game_logic/game.h"

namespace konkr {

void NextLevel() {

}

const bool Game::CheckEnd() {
  const auto& tiles = levels_[cur_level_idx_]->tiles();
  std::optional<int> winner;
  bool end = true;
  if (tiles.empty()) return end;

  for (size_t row = 0; row < tiles.size() && !end; ++row) {
    const auto& tile_row = tiles[row];

    for (size_t col = 0; col < tile_row.size() && !end; ++col) {
      const auto& tile_opt = tile_row[col];
      if (!tile_opt) continue;

      std::optional<int> tile_owner = ((Tile&) tile_opt).getOwner();
      // If current "winner" isn't the only one left on the map, then the game
      // isn't over:
      if (tile_owner.has_value()) {
        if (!winner.has_value()) {
          winner = tile_owner;
        }
        else if (winner != tile_owner) {
          end = false;
        }
      }
    }
  }

  return end;
}

}