// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for
// details.
//
// townhall.h
//
// TODO: add .h description

#ifndef KONKR_WORLD_TOWNHALL_H
#define KONKR_WORLD_TOWNHALL_H

#include <memory>
#include <vector>

#include "world/building.h"
#include "world/entity.h"

namespace konkr {
/**
  @class The townhall is the core of a player's realm.
*/
class Townhall : public Building {
 public:
  using Building::Building;

  void IncreaseLevel() override;
  void DecreaseLevel() override;

  inline int money() const { return money_; }
  inline void set_money(int money) {
    money_ = money;
    if (money >= 10 * level()) {
      IncreaseLevel();
    } else if (money < 10 * level()) {
      DecreaseLevel();
    }
  }

 private:
  int money_ = 10;
};
}  // namespace konkr

#endif  // KONKR_WORLD_TOWNHALL_H