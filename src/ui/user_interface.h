// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.
//
// ui.h
//
// Declares the UserInterface class, which manages the user interface of the
// game. It includes the home page, level selection screen, and game screen. It
// handles user input and rendering of the UI elements.
//

#ifndef KONKR_RENDERING_USER_INTERFACE_H
#define KONKR_RENDERING_USER_INTERFACE_H

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <memory>

#include "rendering/graphics.h"
#include "rendering/level.h"

namespace konkr {

// Represents the different states of the user interface
// HomePage: The main menu of the game
// LevelSelection: The screen where the player can select a level to play
// Game: The screen where the game is played
enum class UserInterfaceState { HomePage, LevelSelection, Game };

// Handles the user interface of the game
// This includes the home page, level selection screen, and game screen
class UserInterface {
 public:
  UserInterface() = delete;

  UserInterface(RenderTarget& render_target)
      : render_target_(render_target),
        gui_(render_target.get_window()),
        current_state_(UserInterfaceState::HomePage) {
    gui_.setFont("assets/fonts/OCRA/OCRA.ttf");
    SetupHomePage();
  }

  inline UserInterfaceState current_state() const { return current_state_; }

  void HandleEvent(const sf::Event& event);
  void Draw();

  // Switches the current state of the UI
  void SwitchState(UserInterfaceState new_state);

  inline bool is_level_selected() const { return selected_level_ != nullptr; }

  inline std::shared_ptr<Level> selected_level() const {
    return selected_level_;
  }

  void ColorReachableTiles(std::shared_ptr<Tile> tile);

  /**
    @brief Checks if the given tile is clicked.
    @param points Points of the tile to check.
    @param mousePos Position of the mouse when clicked.
    @returns true if tile clicked, false otherwise.
  */
  bool TileClicked(std::shared_ptr<FloatRect> bounds,
    Vector2f mouse_pos);

  /**
    @brief Handles event occuring on the tile map.
    @param event The event.
  */
  void TileMapEvent(const sf::Event& event);

 private:
  // Sets up the GUI for the home page
  // This includes the play and exit buttons
  // The play button switches to the level selection screen
  void SetupHomePage();

  // Sets up the GUI for the level selection screen
  // This includes a grid of level cards, each with a name, category,
  // and a button to select/de-select the level
  // The play button is disabled until a level is selected
  // The play level button switches to the game screen
  void SetupLevelSelection();

  // Sets up the GUI for the game screen
  void SetupGame();

  RenderTarget& render_target_;
  tgui::Gui gui_;
  UserInterfaceState current_state_;
  std::shared_ptr<Level> selected_level_ = nullptr;
  std::vector<std::shared_ptr<Level>> available_levels_;
};

}  // namespace konkr

#endif  // KONKR_RENDERING_USER_INTERFACE_H