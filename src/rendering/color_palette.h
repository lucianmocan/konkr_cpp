// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.
//
// color_palette.h
//
// TODO: add .h description

#ifndef KONKR_RENDERING_COLOR_PALETTE_H
#define KONKR_RENDERING_COLOR_PALETTE_H

#include <SFML/Graphics/Color.hpp>
#include <cmath>
#include <optional>

namespace konkr {

struct ColorPalette {
  static constexpr sf::Color OceanBlue = sf::Color(50, 120, 200);

  // Generates a visually distinct color for each player using HSL hue rotation.
  static sf::Color SandColorForPlayer(std::optional<int> player_id) {
    if (!player_id) {
      // default sand color
      return sf::Color(235, 220, 150);
    }

    // golden ratio to get random colors
    // https://martin.ankerl.com/2009/12/09/how-to-create-random-colors-programmatically/
    float hue = std::fmod(*player_id * 137.508f, 360.0f);
    float saturation = 0.6f;
    float lightness = 0.7f;

    // Convert HSL to RGB
    // largely based on
    // https://github.com/sherif-elmetainy/DotnetGD/blob/master/src/CodeArt.DotnetGD/Color.cs
    float c = (1 - std::fabs(2 * lightness - 1)) * saturation;
    float x = c * (1 - std::fabs(std::fmod(hue / 60.0f, 2) - 1));
    float m = lightness - c / 2;
    float r = 0, g = 0, b = 0;

    if (hue < 60) {
      r = c;
      g = x;
      b = 0;
    } else if (hue < 120) {
      r = x;
      g = c;
      b = 0;
    } else if (hue < 180) {
      r = 0;
      g = c;
      b = x;
    } else if (hue < 240) {
      r = 0;
      g = x;
      b = c;
    } else if (hue < 300) {
      r = x;
      g = 0;
      b = c;
    } else {
      r = c;
      g = 0;
      b = x;
    }

    return sf::Color(static_cast<uint8_t>((r + m) * 255),
                     static_cast<uint8_t>((g + m) * 255),
                     static_cast<uint8_t>((b + m) * 255));
  }
};

}  // namespace konkr

#endif  // KONKR_RENDERING_COLOR_PALETTE_H