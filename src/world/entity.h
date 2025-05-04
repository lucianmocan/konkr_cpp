// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for
// details.
//
// entity.h
//
// TODO: add .h description

#ifndef KONKR_WORLD_ENTITY_H
#define KONKR_WORLD_ENTITY_H

#include <iostream>
#include <memory>
#include <string>

#include "rendering/graphics.h"

namespace konkr {

std::string entity_format_display_name(const std::string& sprite_name);

/**
  @class An entity is an object that can be put on a tile (either by a player or
  by the game itself, in the case of bandits). */
class Entity {
 public:
  enum class EntityType {
    Forest,
    Townhall,
    Castle,
    HumanUnit,
    Bandit,
    Unknown
  };

  static inline bool is_building(char c) { return is_townhall(c) || c == 'C'; }

  static inline bool is_townhall(char c) { return c == 'T'; }

  virtual ~Entity() = default;

  static const std::string entity_type_to_string(EntityType type) {
    switch (type) {
      case EntityType::Forest:
        return "Forest";
      case EntityType::Townhall:
        return "Townhall";
      case EntityType::Castle:
        return "Castle";
      case EntityType::HumanUnit:
        return "HumanUnit";
      case EntityType::Bandit:
        return "Bandit";
      default:
        return "Unknown";
    }
  }

  static const EntityType char_to_entity_type(char type) {
    std::cout << "char_to_entity_type: " << type << std::endl;

    switch (type) {
      case 'F':
        return EntityType::Forest;
      case 'T':
        return EntityType::Townhall;
      case 'C':
        return EntityType::Castle;
      case 'V':
        return EntityType::HumanUnit;
      case 'B':
        return EntityType::Bandit;
      default:
        return EntityType::Unknown;
    }
  }

  Entity(EntityType type, int level = 0)
      : type_(type),
        level_(level),
        sprite_name_(entity_type_to_string(type)),
        name_(entity_format_display_name(entity_type_to_string(type))) {}

  Entity(const char type, int level = 0)
      : type_(char_to_entity_type(type)),
        level_(level),
        sprite_name_(entity_type_to_string(type_)),
        name_(entity_format_display_name(entity_type_to_string(type_))) {}

  inline EntityType type() const { return type_; }
  inline bool is_townhall() { return type() == EntityType::Townhall; }

  inline bool is_bandit() { return type() == EntityType::Bandit; }

  inline int level() const { return level_; }
  inline void setLevel(int level) { level_ = level; }

  virtual void IncreaseLevel() {}
  virtual void DecreaseLevel() {}

  inline bool is_building() {
    return type_ == EntityType::Townhall || type_ == EntityType::Castle;
  }

  inline bool is_human_unit() { return type_ == EntityType::HumanUnit; }

  virtual int upkeep_cost() const { return upkeep_cost_; }
  inline void set_upkeep_cost(int upkeep_cost) { upkeep_cost_ = upkeep_cost; }

  inline Vector2i grid_position() const { return grid_position_; }
  inline void set_grid_position(Vector2i grid_position) {
    grid_position_ = grid_position;
  }

 private:
  EntityType type_;
  int level_ = 0;
  int upkeep_cost_ = 1;
  std::string sprite_name_;
  std::string name_;
  Vector2i grid_position_ = {0, 0};
};

std::unique_ptr<Entity> CreateEntity(Entity::EntityType type, int level = 0);
std::unique_ptr<Entity> CreateEntity(char type_char, int level = 0);

}  // namespace konkr

#endif  // KONKR_WORLD_ENTITY_H