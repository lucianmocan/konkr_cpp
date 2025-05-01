// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.
//
// tile.h
//
// TODO: add .h description

#ifndef KONKR_WORLD_TILE_H
#define KONKR_WORLD_TILE_H

#include <optional>
#include <array>
#include <SFML/Graphics/RenderTarget.hpp>


namespace konkr {

// Represents the 6 walls of a hex tile, 
// ordered clockwise starting from top.
enum class WallPosition {
    TopRight = 0,
    Right = 1,
    BottomRight = 2,
    BottomLeft = 3,
    Left = 4,
    TopLeft = 5
};

// Sand tile is the tile the game is played on.
// The other tiles are just for decoration.
enum class TileType {
    Water, Forest, Sand
};


class Tile {
    public:

        static std::optional<Tile> FromAscii(char c, std::optional<int> player_id = std::nullopt);

        Tile(TileType type) : type_(type) {}
        Tile(TileType type, std::optional<int> player_id) : type_(type), player_id_(player_id) {}

        inline void changeOwner(int player_id) {
            player_id_ = player_id;
        }
        
        inline void orphan() {
            is_orphan_ = true;
        }

        inline void claim() {
            is_orphan_ = false;
        }
        
        inline void addWall(WallPosition wall_position) {
            walls_[static_cast<int>(wall_position)] = true;
        }
        
        inline void removeWall(WallPosition wall_position) {
            walls_[static_cast<int>(wall_position)] = false;
        }
        
        inline bool hasWall(WallPosition wall_position) const {
            return walls_[static_cast<int>(wall_position)];
        }
        
        inline bool hasAnyWall() const {
            for (const auto& wall : walls_) {
                if (wall) {
                    return true;
                }
            }
            return false;
        }
        
        inline TileType type() const {
            return type_;
        }

        void Render(sf::RenderTarget& target, sf::Vector2f position, float radius) const;



    private:
        TileType type_;
        std::array<bool, 6> walls_ = {false};
        std::optional<int> player_id_;
        bool is_orphan_ = true;

};


} // namespace konkr


#endif  // KONKR_WORLD_TILE_H