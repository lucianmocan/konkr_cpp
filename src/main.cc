#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <iostream>
#include <memory>
#include <optional>  // Include for std::optional
#include <string>    // Include for std::string
#include <vector>

#include "rendering/color_palette.h"
#include "rendering/graphics.h"
#include "rendering/level.h"
#include "rendering/level_renderer.h"
#include "rendering/sprite_sheet.h"
#include "ui/user_interface.h"
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

  if (sprite_sheet.LoadEntitySpriteMappings("assets/entity_sprites.json")) {
    std::cout << "Successfully loaded entity sprite mappings." << std::endl;
  } else {
    std::cerr << "Failed to load entity sprite mappings." << std::endl;
    return -1;
  }

  konkr::RenderTarget render_target({1920u, 1080u}, "konkr");
  render_target.get_window().setFramerateLimit(144);
  konkr::UserInterface ui(render_target);

  konkr::LevelRenderer renderer;

  // Main game loop
  while (render_target.get_window().isOpen()) {
    while (const std::optional event = render_target.get_window().pollEvent()) {
      ui.HandleEvent(*event);
      if (event->is<sf::Event::Closed>()) {
        render_target.get_window().close();
      }
    }

    render_target.get_window().clear(konkr::ColorPalette::OceanBlue);

    // Draw the level if in Game state
    if (ui.current_state() == konkr::UserInterfaceState::Game &&
        ui.is_level_selected()) {
      renderer.Render(render_target, ui.selected_level(), 50.0f);
    }

    ui.Draw();
    // {
    //   const auto& sprite_defs = sprite_sheet.GetAllSpriteNames();
    //   float x = 100.0f;
    //   float y = 70.0f;
    //   float spacing = 80.0f;

    //   for (const auto& name : sprite_defs) {
    //     if (name.find("palisade") != std::string::npos) {
    //       auto sprite = sprite_sheet.CreateSprite(name);
    //       if (sprite) {
    //         sprite->setPosition({x, y});
    //         render_target.draw(*sprite);
    //       }
    //       x+= spacing;
    //     }
    //   }
    // }

    render_target.get_window().display();  // Update the window
  }

  return 0;
}
