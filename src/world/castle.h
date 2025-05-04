// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for
// details.
//
// castle.h
//
// TODO: add .h description

#ifndef KONKR_WORLD_CASTLE_H
#define KONKR_WORLD_CASTLE_H

#include "world/building.h"

namespace konkr {
class Castle : public Building {
 public:
  using Building::Building;

  inline int upkeep_cost() const { return upkeep_cost_; }

  private:
  int buy_cost = 20;
  int upkeep_cost_ = 2;
};
}  // namespace konkr

#endif  // KONKR_WORLD_CASTLE_H
