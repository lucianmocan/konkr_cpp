// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for
// details.

#include "world/human_unit.h"

#include "rendering/sprite_sheet.h"

namespace konkr {
void HumanUnit::IncreaseLevel() {
  const int max_level =
      SpriteSheet::GetInstance().GetEntitySpriteArraySize(type()) - 1;
  if (level() >= max_level) {
    return;
  }
  setLevel(level() + 1);
}

void HumanUnit::DecreaseLevel() {
  if (level() <= 0) {
    return;
  }
  setLevel(level() - 1);
}
}  // namespace konkr
