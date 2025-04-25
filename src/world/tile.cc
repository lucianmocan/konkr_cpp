// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "world/tile.h"

#include <optional>
#include <string>

namespace konkr {

static std::optional<Tile> FromAscii(char c, std::optional<int> player_id = std::nullopt) {
    if (c == '~') {
        return Tile(TileType::Water);
    } else if (c == '#') {
        return Tile(TileType::Forest);
    } else if (std::string("STCV").find(c) != std::string::npos) {
        // TODO: pass the right entity type
        return Tile(TileType::Sand, player_id);
    } else {
        return std::nullopt;
    }
}

}