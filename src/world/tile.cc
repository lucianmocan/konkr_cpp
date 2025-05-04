// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "world/tile.h"

#include <iostream>
#include <memory>
#include <filesystem>
#include <optional>
#include <string>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include "rendering/color_palette.h"
#include "rendering/graphics.h"
#include "rendering/sprite_sheet.h"
#include "rendering/level_renderer.h"

namespace konkr {

std::shared_ptr<Tile> Tile::FromAscii(char c, std::optional<int> player_id) {
  if (c == '~') {
    return std::make_shared<Tile>(TileType::Water);
  } else if (c == '#') {
    return std::make_shared<Tile>(TileType::Forest);
  } else if (std::string("STCVB").find(c) != std::string::npos) {
    return std::make_shared<Tile>(TileType::Sand, player_id);
  } else {
    return nullptr;
  }
}

std::vector<Vector2i> Tile::GetNeighboringTilesGridPosition() const {
  std::vector<Vector2i> neighbors;
  neighbors.reserve(6);
  // The tile is a hexagon, so we have 6 neighbors
  // The neighbors are at the following positions:
  // (x, y - 1), (x, y + 1), (x - 1, y), (x - 1, y + 1),
  // (x + 1, y), (x + 1, y + 1)
  // x is the row, and y is the column
  neighbors.push_back({grid_position_.x, grid_position_.y - 1});
  neighbors.push_back({grid_position_.x, grid_position_.y + 1});
  neighbors.push_back({grid_position_.x - 1, grid_position_.y});
  neighbors.push_back({grid_position_.x - 1, grid_position_.y + 1});
  neighbors.push_back({grid_position_.x + 1, grid_position_.y});
  neighbors.push_back({grid_position_.x + 1, grid_position_.y + 1});

  // Remove neighbors that are out of bounds
  for (auto it = neighbors.begin(); it != neighbors.end();) {
    if (it->x < 0 || it->y < 0) {
      it = neighbors.erase(it);
    } else {
      ++it;
    }
  }

  return neighbors;
}

void Tile::Render(RenderTarget& target, Vector2f position, float radius,
                  const SpriteSheet& sprite_sheet) const {
  CircleShape tile(radius, 6);
  tile.set_origin({radius, radius});
  tile.set_position(position);

  if (type_ == TileType::Sand) {
    tile.set_fill_color(ColorPalette::SandColorForPlayer(player_id_));
  } else if (type_ == TileType::Water) {
    tile.set_fill_color(ColorPalette::OceanBlue);
  } else if (type_ == TileType::Forest) {
    tile.set_fill_color(Color(60, 120, 60));
  }
  target.draw(tile);

  if (entity_) {
    if (entity_->type() != Entity::EntityType::Unknown) {
      std::optional<std::string> sprite_name =
          sprite_sheet.GetSpriteNameForEntity(entity_->type(),
                                              entity_->level());
      if (sprite_name) {
        auto info = sprite_sheet.GetSpriteInfo(*sprite_name);
        if (info) {
          auto sprite =
              Graphics::CreateSprite(sprite_sheet.GetTexture(), info->rect);
          // Sets origin to center of the sprite
          if (sprite) {
            sprite->set_origin(
                {info->rect.size.x / 2.f, info->rect.size.y / 2.f});
            sprite->set_position(position);
            target.draw(*sprite);
          }
        } else {
          std::cerr << "Failed to get sprite info for entity: " << *sprite_name
                    << std::endl;
        }
      } else {
        std::cerr << "Failed to get sprite name for entity: "
                  << Entity::entity_type_to_string(entity_->type())
                  << std::endl;
      }
    }
  }

  // --- Draw "D" if level_ == 1 ---
  if (level() == 1) {
    const Font& font = LevelRenderer::get_font();
    std::cout << "Font loaded: " << font.is_loaded() << std::endl;
    Text text(font, "D", 16);
    text.set_fill_color(Color::Yellow);
    text.set_outline_color(Color::Black);
    text.set_outline_thickness(2);

    FloatRect bounds = text.get_local_bounds();
    text.set_origin({bounds.size.x / 2, bounds.size.y / 2});
    text.set_position({position.x, position.y - radius / 2});
    target.draw(text);
  }
}

}  // namespace konkr