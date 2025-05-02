// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.
//
// sprite_sheet.h
//
// Declares the SpriteSheet class, which manages loading sprite textures and
// definitions from files, and provides access to individual sprite rectangles
// for rendering.

#ifndef KONKR_RENDERING_SPRITE_SHEET_H
#define KONKR_RENDERING_SPRITE_SHEET_H

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "world/entity.h"

namespace konkr {

// Wraps SFML's IntRect (position, size) to represent a sprite's rectangle.
struct SpriteInfo {
  sf::IntRect rect;
};

// Manages loading and storing the sprite definitions (name and frame) from the
// JSON file and the texture from the PNG file. It also provides methods to
// create sprites and retrieve sprite information.
class SpriteSheet {
 public:

  inline const std::unordered_map<std::string, std::vector<std::string>>& entity_sprite_vectors() const {
    return entity_sprite_vectors_;
  }


  // Stores the texture in texture_
  bool LoadFromFile(const std::filesystem::path& file_path);

  // Adds a sprite's name and its corresponding rectangle to the sprites_map_
  void AddSpriteInfo(const std::string& name, const SpriteInfo& sprite_info);

  // Retrieves the sprite's rectangle based on its name
  std::optional<SpriteInfo> GetSpriteInfo(const std::string& name) const;

  // Creates a sprite using the texture and the rectangle from the sprites_map_
  std::optional<sf::Sprite> CreateSprite(const std::string& name) const;

  // Returns the texture used for the sprites.
  const sf::Texture& GetTexture() const;

  bool LoadSpriteDefinitions(const std::filesystem::path& definition_file_path);

  std::vector<std::string> GetAllSpriteNames() const;

  bool LoadEntitySpriteMappings(const std::filesystem::path& mapping_file_path);

  std::optional<std::string> GetSpriteNameForEntity(
    const Entity::EntityType& entity_type,
    int level = 0) const;

  int GetEntitySpriteArraySize(const Entity::EntityType& entity_type) const;

 private:
  sf::Texture texture_;
  std::unordered_map<std::string, SpriteInfo> sprites_map_;
  std::unordered_map<std::string, std::vector<std::string>> entity_sprite_vectors_;
  bool loaded_ = false;
};

}  // namespace konkr

#endif  // KONKR_RENDERING_SPRITE_SHEET_H