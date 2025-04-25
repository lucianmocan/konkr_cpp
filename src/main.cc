#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <iostream>
#include <optional>  // Include for std::optional
#include <string>    // Include for std::string
#include <vector>

#include "rendering/sprite_sheet.h"

int main() {
  konkr::SpriteSheet sprite_sheet;

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

  auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "konkr");
  window.setFramerateLimit(144);

  // Get all sprite names
  std::vector<std::string> sprite_names = sprite_sheet.GetAllSpriteNames();
  std::vector<sf::Sprite> sprites_to_draw;
  sprites_to_draw.reserve(sprite_names.size());

  // Create and position sprites
  float current_x = 10.0f;
  float current_y = 10.0f;
  float max_row_height = 0.0f;
  const float padding = 10.0f;

  for (const std::string& name : sprite_names) {
    if (name.contains("pawns")) {
      std::optional<sf::Sprite> sprite_opt = sprite_sheet.CreateSprite(name);
      if (sprite_opt) {
        sf::Sprite sprite = *sprite_opt;  // Dereference the optional
        sf::FloatRect bounds = sprite.getLocalBounds();

        // Check if sprite fits on the current row
        if (current_x + bounds.size.x > window.getSize().x - padding) {
          current_x = padding;
          current_y += max_row_height + padding;
          max_row_height = 0.0f;
        }

        sprite.setPosition({current_x, current_y});
        sprites_to_draw.push_back(sprite);

        // Update position for next sprite
        current_x += bounds.size.x + padding;
        if (bounds.size.y > max_row_height) {
          max_row_height = bounds.size.y;
        }
        std::cout << "Created sprite: " << name << " at position (" << current_x
                  << ", " << current_y << ")" << std::endl;
      } else {
        std::cerr << "Failed to create sprite: " << name << std::endl;
      }
    } else {
      continue;
    }
  }

  // Main game loop
  while (window.isOpen()) {
    // Event handling (simplified)
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      // Add other event handling if needed (e.g., keyboard input to close)
      if (event->is<sf::Event::KeyPressed>()) {
        if (event->getIf<sf::Event::KeyPressed>()->code ==
            sf::Keyboard::Key::Escape) {
          window.close();
        }
      }
    }

    // Drawing
    window.clear(sf::Color::White);  // Clear the window with white background

    // Draw all the created sprites
    for (const auto& sprite : sprites_to_draw) {
      window.draw(sprite);
    }

    window.display();  // Update the window
  }

  return 0;  // Indicate successful execution
}
