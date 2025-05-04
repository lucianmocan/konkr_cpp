// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.
//
// level_renderer.h
//
// TODO: add .h description

#ifndef KONKR_RENDERING_LEVEL_RENDERER_H
#define KONKR_RENDERING_LEVEL_RENDERER_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "rendering/graphics.h"
#include "rendering/level.h"
#include "rendering/sprite_sheet.h"

namespace konkr {

class LevelRenderer {
 public:
  bool LoadFont(const std::string& path);

  void Render(RenderTarget& target, std::shared_ptr<const Level> level,
              float hex_radius) const;

 private:
  sf::Font font_;
  bool font_loaded_ = false;
};

}  // namespace konkr

#endif  // KONKR_RENDERING_LEVEL_RENDERER_H