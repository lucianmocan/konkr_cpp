// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "rendering/level_renderer.h"

#include "world/tile.h"

namespace konkr {
void LevelRenderer::Render(sf::RenderTarget& target, const Level& level,
                           float hex_radius) const {
  const float hex_height = 2 * hex_radius;
  const float hex_width = std::sqrt(3.0f) * hex_radius;
  const float vert_spacing = hex_height * 0.75f;

  const float x_padding = 40.0f;
  const float y_padding = 40.0f;

  const auto& tiles = level.tiles();
  if (tiles.empty()) return;

  // We want to center the map in the window
  // Calculate the map size
  const size_t num_rows = tiles.size();
  const size_t num_cols = tiles[0].size();
  const float map_width =
      num_cols * hex_width + (hex_width / 2);  // Account for indentation
  const float map_height = num_rows * vert_spacing;

  // Then we get the window size
  const sf::Vector2u window_size = target.getSize();
  const float window_width = static_cast<float>(window_size.x);
  const float window_height = static_cast<float>(window_size.y);

  // And we calculate the offsets (so that the map is centered)
  const float x_offset = (window_width - map_width) / 2.0f;
  const float y_offset = (window_height - map_height) / 2.0f;

  for (size_t row = 0; row < tiles.size(); ++row) {
    const auto& tile_row = tiles[row];
    bool indent = !level.map()[row].empty() && level.map()[row][0] == '|';

    for (size_t col = 0; col < tile_row.size(); ++col) {
      const auto& tile_opt = tile_row[col];
      if (!tile_opt) continue;

      float x = col * hex_width + (indent ? (hex_width / 2) : 0) + x_padding +
                x_offset;
      float y = row * vert_spacing + y_padding + y_offset;
      tile_opt->Render(target, sf::Vector2f(x, y), hex_radius);
    }
  }
}

}  // namespace konkr
