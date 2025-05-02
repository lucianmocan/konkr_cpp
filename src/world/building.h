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
class Building : public Entity {
 public:
  using Entity::Entity;

  void AddWalls();
};
}  // namespace konkr

#endif  // KONKR_WORLD_BUILDING_H