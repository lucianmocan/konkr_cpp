// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for
// details.
//
// human_unit.h
//
// TODO: add .h description

#ifndef KONKR_WORLD_HUMAN_UNIT_H
#define KONKR_WORLD_HUMAN_UNIT_H

#include "world/entity.h"

namespace konkr {

/**
  @class An entity that can be moved by a player.
*/
class HumanUnit : public Entity {
 public:
  using Entity::Entity;

  void IncreaseLevel() override;
  void DecreaseLevel() override;

  inline void set_upkeep_cost(int upkeep_cost) { upkeep_cost_ = upkeep_cost; }

 private:
  int upkeep_cost_ = 2;
};

}  // namespace konkr

#endif  // KONKR_WORLD_HUMAN_UNIT_H