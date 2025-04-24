// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for details.
//
// sprite_sheet.h
//
// Declares the SpriteSheet class and SpriteInfo struct for a Konkr.io clone,
// managing sprite positions and dimensions from a spritesheet for rendering.

#ifndef KONKR_RENDERING_SPRITE_SHEET_H
#define KONKR_RENDERING_SPRITE_SHEET_H

#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

namespace konkr {

    struct SpriteInfo {
        sf::IntRect rect;
    };

    class SpriteSheet {
        public:

            bool LoadFromFile(const std::filesystem::path& file_path);
            void AddSpriteInfo(const std::string& name, const SpriteInfo& sprite_info);

            std::optional<SpriteInfo> GetSpriteInfo(const std::string& name) const;
            std::optional<sf::Sprite> CreateSprite(const std::string& name) const;

            const sf::Texture& GetTexture() const;

            bool LoadSpriteDefinitions(const std::filesystem::path& definition_file_path);

            std::vector<std::string> GetAllSpriteNames() const;

        private:
            sf::Texture texture_;
            std::unordered_map<std::string, SpriteInfo> sprites_map_;
            bool loaded_ = false;
    };

} // namespace konkr

#endif // KONKR_RENDERING_SPRITE_SHEET_H