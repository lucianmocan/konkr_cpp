// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "rendering/sprite_sheet.h"

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace konkr {

bool SpriteSheet::LoadFromFile(const std::filesystem::path& file_path) {
  if (!texture_.loadFromFile(file_path.string())) {
    return false;
  }
  loaded_ = true;
  return true;
}

void SpriteSheet::AddSpriteInfo(const std::string& name,
                                const SpriteInfo& sprite_info) {
  sprites_map_.emplace(name, sprite_info);
}

std::optional<SpriteInfo> SpriteSheet::GetSpriteInfo(
    const std::string& name) const {
  auto it = sprites_map_.find(name);
  if (it != sprites_map_.end()) {
    return it->second;
  }
  std::cerr << "Sprite '" << name << "' not found in loaded sprite definitions."
            << std::endl;
  return std::nullopt;
}

std::optional<sf::Sprite> SpriteSheet::CreateSprite(
    const std::string& name) const {
  if (!loaded_) {
    return std::nullopt;
  }
  // if the sprite is not found, then there's a mistake somewhere in the calling
  // function
  std::optional<SpriteInfo> sprite_info = GetSpriteInfo(name);
  if (!sprite_info) {
    return std::nullopt;
  }
  return sf::Sprite(texture_, sprite_info->rect);
}

const sf::Texture& SpriteSheet::GetTexture() const {
  if (!loaded_) {
    throw std::runtime_error(
        "SpriteSheet::getTexture() called before loading the texture.");
  }
  return texture_;
}

bool SpriteSheet::LoadSpriteDefinitions(
    const std::filesystem::path& definition_file_path) {
  std::ifstream definition_stream(definition_file_path);
  if (!definition_stream.is_open()) {
    std::cerr << "Failed to open sprite definition file: "
              << definition_file_path << std::endl;
    return false;
  }

  nlohmann::json json_data;
  try {
    json_data = nlohmann::json::parse(definition_stream);
  } catch (nlohmann::json::parse_error& e) {
    std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
    return false;
  }

  try {
    if (!json_data.contains("textures") || !json_data["textures"].is_array() ||
        json_data["textures"].empty()) {
      std::cerr << "Invalid JSON format: 'textures' array is missing or empty."
                << std::endl;
      return false;
    }

    const nlohmann::json& textures_data = json_data["textures"][0];

    if (!textures_data.contains("frames") ||
        !textures_data["frames"].is_array()) {
      std::cerr << "Invalid JSON format: 'frames' array is missing."
                << std::endl;
      return false;
    }

    for (const auto& sprite_data : textures_data["frames"]) {
      if (!sprite_data.is_object() || !sprite_data.contains("filename") ||
          !sprite_data["filename"].is_string() ||
          !sprite_data.contains("frame") || !sprite_data["frame"].is_object() ||
          !sprite_data["frame"].contains("x") ||
          !sprite_data["frame"]["x"].is_number_integer() ||
          !sprite_data["frame"].contains("y") ||
          !sprite_data["frame"]["y"].is_number_integer() ||
          !sprite_data["frame"].contains("w") ||
          !sprite_data["frame"]["w"].is_number_integer() ||
          !sprite_data["frame"].contains("h") ||
          !sprite_data["frame"]["h"].is_number_integer()) {
        std::string filename =
            (sprite_data.is_object() && sprite_data.contains("filename")) &&
                    sprite_data["filename"].is_string()
                ? sprite_data["filename"].get<std::string>()
                : "unknown";

        std::cerr << "Invalid sprite data format for sprite: " << filename
                  << std::endl;
        // if there's a mistake, we continue, let's see if we can load the rest
        continue;
      }

      std::string filename = sprite_data["filename"].get<std::string>();
      int x = sprite_data["frame"]["x"].get<int>();
      int y = sprite_data["frame"]["y"].get<int>();
      int width = sprite_data["frame"]["w"].get<int>();
      int height = sprite_data["frame"]["h"].get<int>();

      AddSpriteInfo(filename, SpriteInfo{sf::IntRect({x, y}, {width, height})});
    }
  } catch (nlohmann::json::exception& e) {
    std::cerr << "Error processing JSON in sprite definition file: "
              << definition_file_path << std::endl;
    return false;
  }

  std::cout << "Successfully loaded " << sprites_map_.size() << " sprites from "
            << definition_file_path << std::endl;
  return true;
}

std::vector<std::string> SpriteSheet::GetAllSpriteNames() const {
  std::vector<std::string> sprite_names;
  sprite_names.reserve(sprites_map_.size());
  for (const auto& pair : sprites_map_) {
    sprite_names.push_back(pair.first);
  }
  return sprite_names;
}

bool SpriteSheet::LoadEntitySpriteMappings(
    const std::filesystem::path& mapping_file_path) {
      std::ifstream mapping_stream(mapping_file_path);
    if (!mapping_stream.is_open()) {
      std::cerr << "Failed to open entity sprite mapping file: "
                << mapping_file_path << std::endl;
      return false;
    }

    nlohmann::json json_data;

    try {
      mapping_stream >> json_data;
    } catch (nlohmann::json::parse_error& e) {
      std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
      return false;
    }
    
    entity_sprite_vectors_.clear();
    for (auto it = json_data.begin(); it != json_data.end(); ++it) {
      if (it.value().is_array()) {
        std::vector<std::string> sprite_names;
        for (const auto& sprite_name: it.value()) {
          if (sprite_name.is_string()) {
            sprite_names.push_back(sprite_name.get<std::string>());
          } else {
            std::cerr << "Invalid sprite name format in entity mapping: "
                      << sprite_name.dump() << std::endl;
          }
        }
        entity_sprite_vectors_[it.key()] = std::move(sprite_names);
      }
    }

    return true;
  }

std::optional<std::string> SpriteSheet::GetSpriteNameForEntity(
    const Entity::EntityType& entity_type, int level) const {
      const std::string entity_type_str =
          Entity::entity_type_to_string(entity_type);
      auto it = entity_sprite_vectors_.find(entity_type_str);
      if (it != entity_sprite_vectors_.end() && level >= 0 && level < static_cast<int>(it->second.size())) {
        return it->second[level];
      }
      return std::nullopt;
    }

  int SpriteSheet::GetEntitySpriteArraySize(const Entity::EntityType& entity_type) const {
    const std::string entity_type_str =
        Entity::entity_type_to_string(entity_type);
    auto it = entity_sprite_vectors_.find(entity_type_str);
    if (it != entity_sprite_vectors_.end()) {
      return static_cast<int>(it->second.size());
    }
    return 0;
  }

}  // namespace konkr