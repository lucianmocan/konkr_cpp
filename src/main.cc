#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <iostream>
#include <optional>  // Include for std::optional
#include <string>    // Include for std::string
#include <vector>

#include "rendering/color_palette.h"
#include "rendering/level.h"
#include "rendering/level_renderer.h"
#include "rendering/sprite_sheet.h"
#include "world/entity.h"
#include "world/human_unit.h"
#include "world/townhall.h"

int main() {
  konkr::SpriteSheet& sprite_sheet = konkr::SpriteSheet::GetInstance();

  const std::filesystem::path atlas_json_path = "assets/atlas.json";
  const std::filesystem::path atlas_png_path = "assets/atlas.png";

  std::cout << "Attempting to load sprite definitions from: " << atlas_json_path
            << std::endl;

  if (sprite_sheet.LoadSpriteDefinitions(atlas_json_path)) {
    std::cout << "Successfully loaded sprite definitions." << std::endl;
  } else {
    std::cerr << "Failed to load sprite definitions." << std::endl;
    return -1;
  }

  std::cout << "Attempting to load texture from: " << atlas_png_path
            << std::endl;
  if (sprite_sheet.LoadFromFile(atlas_png_path)) {
    std::cout << "Successfully loaded texture." << std::endl;
  } else {
    std::cerr << "Failed to load texture." << std::endl;
    return -1;
  }

  // Discover available levels
  const std::filesystem::path levels_dir = "assets/levels";
  std::vector<konkr::Level> levels =
      konkr::Level::GetAvailableLevels(levels_dir);

  if (levels.empty()) {
    std::cerr << "No levels found in " << levels_dir << std::endl;
    return -1;
  }

  if (sprite_sheet.LoadEntitySpriteMappings("assets/entity_sprites.json")) {
    std::cout << "Successfully loaded entity sprite mappings." << std::endl;
  } else {
    std::cerr << "Failed to load entity sprite mappings." << std::endl;
    return -1;
  }

  // We can now load the first level
  konkr::Level level = std::move(levels.front());

  if (!level.Load()) {
    std::cerr << "Failed to load level: " << level.name() << std::endl;
    return -1;
  }

  auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "konkr");
  window.setFramerateLimit(144);

  konkr::LevelRenderer renderer;

  // Main game loop
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      if (event->is<sf::Event::KeyPressed>()) {
        auto key = event->getIf<sf::Event::KeyPressed>()->code;
        if (key == sf::Keyboard::Key::Escape) {
          window.close();
        }
        if (key == sf::Keyboard::Key::U) {
          // Finds and upgrades the first HumanUnit in the level
          for (auto& row : level.tiles()) {
            for (auto& tile_opt : row) {
              if (tile_opt && tile_opt->entity()) {
                auto* human = dynamic_cast<konkr::HumanUnit*>(tile_opt->entity().get());
                if (human) {
                  human->IncreaseLevel();
                  std::cout << "Upgraded a HumanUnit!" << std::endl;
                  goto upgraded; // breaks out of both loops
                }
              }
            }
          }
        }
        if (key == sf::Keyboard::Key::T) {
          // Finds and upgrades the first Townhall in the level
          for (auto& row : level.tiles()) {
            for (auto& tile_opt : row) {
              if (tile_opt && tile_opt->entity()) {
                auto* thall = dynamic_cast<konkr::Townhall*>(tile_opt->entity().get());
                if (thall) {
                  thall->IncreaseLevel();
                  std::cout << "Upgraded a Townhall!" << std::endl;
                  goto upgraded; // breaks out of both loops
                }
              }
            }
          }
        }
        upgraded:;
      }
    }

    sf::Color OceanBlue(50, 120, 200);
    window.clear(konkr::ColorPalette::OceanBlue);

    renderer.Render(window, level, 50.0f);

    window.display();  // Update the window
  }

  return 0;
}
