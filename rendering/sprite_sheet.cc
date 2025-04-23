// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for details.
//
// sprite_sheet.cc
//
// Implements the SpriteSheet class for a Konkr.io clone, handling the loading
// and rendering of sprite sheets, as well as the management of individual sprites.
//
#include "rendering/sprite_sheet.h"

#include <stdexcept>
#include <string>
#include <optional>
#include <utility>

#include <SFML/Graphics.hpp>

namespace konkr {


bool SpriteSheet::loadFromFile(const std::filesystem::path& file_path) {
    if (!texture_.loadFromFile(file_path.string())) {
        return false;
    }
    loaded_ = true;
    return true;
}

void SpriteSheet::addSprite(const std::string& name, const SpriteInfo& sprite_info) {
    sprites_map_.emplace(name, sprite_info);
}

std::optional<SpriteInfo> SpriteSheet::getSpriteInfo(const std::string& name) const {
    auto it = sprites_map_.find(name);
    if (it != sprites_map_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<sf::Sprite> SpriteSheet::createSprite(const std::string& name) const {
    if (!loaded_) {
        return std::nullopt;
    }
    // if the sprite is not found, then there's a mistake somewhere in the calling function
    std::optional<SpriteInfo> sprite_info = getSpriteInfo(name);
    if (!sprite_info) {
        return std::nullopt;
    }
    return sf::Sprite(texture_, sprite_info->rect);
}

const sf::Texture& SpriteSheet::getTexture() const {
    if (!loaded_) {
        throw std::runtime_error("SpriteSheet::getTexture() called before loading the texture.");
    }
    return texture_;
}


} // namespace konkr