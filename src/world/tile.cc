// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "world/tile.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <string>

#include "rendering/color_palette.h"
#include "rendering/sprite_sheet.h"

namespace konkr {

std::optional<Tile> Tile::FromAscii(char c, std::optional<int> player_id) {
  if (c == '~') {
    return Tile(TileType::Water);
  } else if (c == '#') {
    return Tile(TileType::Forest);
  } else if (std::string("STCVB").find(c) != std::string::npos) {
    // TODO: pass the right entity type
    return Tile(TileType::Sand, player_id);
  } else {
    return std::nullopt;
  }
}

void Tile::Render(sf::RenderTarget& target, sf::Vector2f position, float radius,
                  const SpriteSheet& sprite_sheet) const {
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

  if (entity_) {
    if (entity_->type() != Entity::EntityType::Unknown) {
      std::optional<std::string> sprite_name =
          sprite_sheet.GetSpriteNameForEntity(entity_->type(), entity_->level());
      if (sprite_name) {
        auto info = sprite_sheet.GetSpriteInfo(*sprite_name);
        if (info) {
          sf::Sprite sprite(sprite_sheet.GetTexture());
          sprite.setTextureRect(info->rect);
          // Sets origin to center of the sprite
          sprite.setOrigin({info->rect.size.x / 2.f, info->rect.size.y / 2.f});

          sprite.setPosition(position);
          target.draw(sprite);
        } else {
          std::cerr << "Failed to get sprite info for entity: " << *sprite_name
                    << std::endl;
        }
      } else {
        std::cerr << "Failed to get sprite name for entity: "
                  << Entity::entity_type_to_string(entity_->type()) << std::endl;
      }
    }
  }
}

}  // namespace konkr