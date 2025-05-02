// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include <string>
#include <algorithm>
#include <filesystem>

namespace konkr
{

std::string entity_format_display_name(const std::string& sprite_name) {
    std::filesystem::path sprite_path(sprite_name);
    std::string formatted_name = sprite_path.stem().string();
    std::replace(formatted_name.begin(), formatted_name.end(), '_', ' ');
    formatted_name[0] = std::toupper(formatted_name[0]);
    return formatted_name;
}

    
} // namespace konkr
