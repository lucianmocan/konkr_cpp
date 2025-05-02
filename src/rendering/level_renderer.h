// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.
//
// level_renderer.h
//
// TODO: add .h description

#ifndef KONKR_RENDERING_LEVEL_RENDERER_H
#define KONKR_RENDERING_LEVEL_RENDERER_H

#include <SFML/Graphics/RenderTarget.hpp>

#include "rendering/level.h"

#include "rendering/sprite_sheet.h"

namespace konkr {

class LevelRenderer {
 public:
  void Render(sf::RenderTarget& target, const Level& level,
              float hex_radius, const SpriteSheet& sprite_sheet) const;
};

}  // namespace konkr

#endif  // KONKR_RENDERING_LEVEL_RENDERER_H