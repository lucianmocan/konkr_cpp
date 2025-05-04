// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for
// details.
//
// townhall.h
//
// TODO: add .h description

#ifndef KONKR_WORLD_BUILDING_H
#define KONKR_WORLD_BUILDING_H

#include "world/entity.h"

namespace konkr {
/**
  @class A building is an entity that can be put on the map (either by the
  player or by the game in the case of townhalls), but cannot be moved once
  placed.
*/
class Building : public Entity {
 public:
  using Entity::Entity;
};
}  // namespace konkr

#endif  // KONKR_WORLD_BUILDING_H