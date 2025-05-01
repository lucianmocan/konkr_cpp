// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "world/tile.h"

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

#include "rendering/color_palette.h"

namespace konkr {

std::optional<Tile> Tile::FromAscii(char c, std::optional<int> player_id) {
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

void Tile::Render(sf::RenderTarget& target, sf::Vector2f position, float radius) const {
    sf::CircleShape tile(radius, 6);
    tile.setOrigin({radius, radius});
    tile.setPosition(position);

    if (type_ == TileType::Sand) {
        tile.setFillColor(ColorPalette::SandColorForPlayer(player_id_));
    } else if (type_ == TileType::Water) {
        tile.setFillColor(ColorPalette::OceanBlue);
    } else if (type_ == TileType::Forest) {
        tile.setFillColor(sf::Color(60, 120, 60));
    }
    target.draw(tile);
}

} // namespace konkr