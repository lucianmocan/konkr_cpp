// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "world/entity.h"

#include <algorithm>
#include <filesystem>
#include <string>
#include <memory>

#include "world/human_unit.h"
#include "world/townhall.h"
#include "world/castle.h"

namespace konkr {

std::string entity_format_display_name(const std::string& sprite_name) {
  std::filesystem::path sprite_path(sprite_name);
  std::string formatted_name = sprite_path.stem().string();
  std::replace(formatted_name.begin(), formatted_name.end(), '_', ' ');
  formatted_name[0] = std::toupper(formatted_name[0]);
  return formatted_name;
}

std::unique_ptr<Entity> CreateEntity(Entity::EntityType type, int level) {
    switch (type) {
        case Entity::EntityType::HumanUnit:
            return std::make_unique<HumanUnit>(type, level);
        case Entity::EntityType::Townhall:
            return std::make_unique<Townhall>(type, level);
        case Entity::EntityType::Castle:
            return std::make_unique<Castle>(level);
        default:
            return std::make_unique<Entity>(type, level);
    }
}

std::unique_ptr<Entity> CreateEntity(char type, int level) {
    return CreateEntity(Entity::char_to_entity_type(type), level);
};

}  // namespace konkr
