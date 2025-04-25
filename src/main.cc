#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <iostream>
#include <optional>  // Include for std::optional
#include <string>    // Include for std::string
#include <vector>

#include "rendering/level.h"
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

  // Main game loop
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      if (event->is<sf::Event::KeyPressed>()) {
        if (event->getIf<sf::Event::KeyPressed>()->code ==
            sf::Keyboard::Key::Escape) {
          window.close();
        }
      }
    }

    window.clear(sf::Color::White);

    window.display();  // Update the window
  }

  return 0;
}
