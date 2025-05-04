// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.
//
// tile.h
//
// TODO: add .h description

#ifndef KONKR_WORLD_TILE_H
#define KONKR_WORLD_TILE_H

#include <array>
#include <memory>
#include <optional>

#include "rendering/graphics.h"
#include "rendering/sprite_sheet.h"
#include "world/entity.h"

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
enum class TileType { Water, Forest, Sand };

/**
  @class A tile is an hexagon representing a location that can contain an entity
  or be empty.
*/
class Tile {
 public:
  static std::shared_ptr<Tile> FromAscii(
      char c, std::optional<int> player_id = std::nullopt);

  static inline bool is_sand(char c) { return c == 'S'; }

  static inline bool is_sand(TileType type) { return type == TileType::Sand; }

  static inline bool is_decoration(char c) { return is_forest(c) || c == '~'; }

  static inline bool is_decoration(TileType type) {
    return type == TileType::Forest || type == TileType::Water;
  }

  static inline bool is_forest(char c) { return c == '#'; }

  Tile(TileType type) : type_(type) {}
  Tile(TileType type, std::optional<int> player_id)
      : type_(type), player_id_(player_id) {}

  inline void change_owner(int player_id) { player_id_ = player_id; }

  inline std::optional<int> get_owner() { return player_id_; }
  inline std::shared_ptr<CircleShape> get_shape() {
    return std::make_shared<CircleShape>(*shape_);
  }

  inline int level() const { return level_; }

  inline void set_level(int level) { level_ = level; }

  inline void orphan() { is_orphan_ = true; }

  inline bool is_orphan() const { return is_orphan_; }

  inline void claim() { is_orphan_ = false; }

  inline void add_wall(WallPosition wall_position) {
    walls_[static_cast<int>(wall_position)] = true;
  }

  inline void remove_wall(WallPosition wall_position) {
    walls_[static_cast<int>(wall_position)] = false;
  }

  inline bool has_wall(WallPosition wall_position) const {
    return walls_[static_cast<int>(wall_position)];
  }

  inline bool has_any_walls() const {
    for (const auto& wall : walls_) {
      if (wall) {
        return true;
      }
    }
    return false;
  }

  inline TileType type() const { return type_; }

  inline void set_grid_position(Vector2i grid_position) {
    grid_position_ = grid_position;
  };

  inline const Vector2i& grid_position() const { return grid_position_; }

  inline void set_entity(std::unique_ptr<Entity> entity) {
    entity_ = std::move(entity);
  }
  inline const std::shared_ptr<Entity>& entity() const { return entity_; }

  std::vector<Vector2i> GetNeighboringTilesGridPosition() const;

  void Render(RenderTarget& target, Vector2f position, float radius,
              const SpriteSheet& sprite_sheet);

 private:
  std::shared_ptr<Entity> entity_ = nullptr;
  std::unique_ptr<CircleShape> shape_ =
      nullptr;  // Object being rendered on the window
  TileType type_;
  std::array<bool, 6> walls_ = {false};
  std::optional<int> player_id_;
  bool is_orphan_ = true;
  int level_ = -1;
  Vector2i grid_position_ = {0, 0};
};

}  // namespace konkr

#endif  // KONKR_WORLD_TILE_H