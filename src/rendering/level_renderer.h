// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.
//
// level_renderer.h
//
// TODO: add .h description

#ifndef KONKR_RENDERING_LEVEL_RENDERER_H
#define KONKR_RENDERING_LEVEL_RENDERER_H

#include <memory>

#include "rendering/graphics.h"
#include "rendering/level.h"

namespace konkr {

class LevelRenderer {
 public:
  static bool LoadFont(const std::string& path);

  static const Font& get_font();

  /**
     @brief Renders the level on the window.
     @param target SFML RenderTarget.
     @param level Level to render.
     @param hex_radius Radius of the hexagon representing a tile.
  */
  void Render(RenderTarget& target, std::shared_ptr<const Level> level,
              float hex_radius) const;

 private:
  bool font_loaded_ = false;
};

}  // namespace konkr

#endif  // KONKR_RENDERING_LEVEL_RENDERER_H