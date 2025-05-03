// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "world/player.h"

#include <random>
#include <string>
#include <vector>

namespace konkr {

std::string GenerateWarriorName() {
  // Prefixes for the warrior names
  // any basic name can do
  std::vector<std::string> prefixes = {
      "Mocan", "Waehren", "Iron",  "Fire",  "Shadow", "Thunder", "Frost",
      "Dark",  "Steel",   "Flame", "Stone", "Wolf",   "Dragon",  "Eagle",
      "Lion",  "Blood",   "Grim",  "Raven", "Lucian", "Antoine",
  };

  // Some stems to make it more interesting
  std::vector<std::string> stems = {
      "axe",    "claw", "blade", "fang",  "heart", "spear", "shield",
      "hammer", "bow",  "arrow", "sword", "fang",  "wing",  "tail",
  };

  // yay a fantasy touch
  std::vector<std::string> suffixes = {"gar", "thar", "dor", "wen",  "ion",
                                       "dar", "gor",  "nor", "thor", "khan"};

  // Randomly select a prefix, stem, and suffix
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> prefix_dist(0, prefixes.size() - 1);
  std::uniform_int_distribution<> stem_dist(0, stems.size() - 1);
  std::uniform_int_distribution<> suffix_dist(0, suffixes.size() - 1);

  std::uniform_int_distribution<> use_suffix(0, 1);
  bool include_suffix = use_suffix(gen);

  std::string name = prefixes[prefix_dist(gen)] + stems[stem_dist(gen)];
  if (include_suffix) {
    name += suffixes[suffix_dist(gen)];
  }

  return name;
}

}  // namespace konkr
