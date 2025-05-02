// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for
// details.
//
// entity.h
//
// TODO: add .h description

#ifndef KONKR_WORLD_ENTITY_H
#define KONKR_WORLD_ENTITY_H

#include <string>

namespace konkr {

std::string entity_format_display_name(const std::string& sprite_name);

class Entity {
 public:
  enum class EntityType { Forest, Townhall, Castle, HumanUnit, Unknown };

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
      default:
        return "Unknown";
    }
  }

  static const EntityType char_to_entity_type(char type) {
    switch (type) {
      case 'F':
        return EntityType::Forest;
      case 'T':
        return EntityType::Townhall;
      case 'C':
        return EntityType::Castle;
      case 'V':
        return EntityType::HumanUnit;
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
  inline int level() const { return level_; }

 private:
  EntityType type_;
  int level_ = 0;
  std::string sprite_name_;
  std::string name_;
};

}  // namespace konkr

#endif  // KONKR_WORLD_ENTITY_H