// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for
// details.
//
// bandit.h
//
// TODO: add .h description

#ifndef KONKR_WORLD_BANDIT_H
#define KONKR_WORLD_BANDIT_H

#include "world/entity.h"

namespace konkr {
/**
  @class A bandit is automatically moved by the game on the map.
*/
class Bandit : public Entity {
 public:
  using Entity::Entity;

 private:
  int upkeep_cost = -1;
};

}  // namespace konkr

#endif