// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "ui/user_interface.h"

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Panel.hpp>

#include "rendering/graphics.h"
#include "rendering/level.h"

namespace konkr {

void UserInterface::ColorReachableTiles(std::shared_ptr<Tile> tile) {
  auto reachable = tile->GetNeighboringTilesGridPosition();
  for (auto t : reachable) {
    auto rtile = selected_level_->tiles().at(t.x).at(t.y);
    rtile->set_reachability(true);
  }
}

bool UserInterface::TileClicked(std::shared_ptr<FloatRect> bounds,
                                Vector2f mouse_pos) {
  return bounds->contains(mouse_pos);
}

void UserInterface::TileMapEvent(const sf::Event& event) {
  std::shared_ptr<Level::Tiles> tiles =
      std::make_shared<Level::Tiles>(selected_level_->tiles());

  if (event.is<sf::Event::MouseButtonPressed>() &&
      event.getIf<sf::Event::MouseButtonPressed>()->button ==
          sf::Mouse::Button::Left) {
    if (!tiles->empty()) {
      for (size_t row = 0; row < tiles->size(); ++row) {
        const auto& tile_row = tiles->at(row);

        for (size_t col = 0; col < tile_row.size(); ++col) {
          const auto& tile_opt = tile_row[col];
          if (!tile_opt) continue;
          sf::Vector2i mouse_pos =
              sf::Mouse::getPosition(render_target_.get_window());
          sf::Vector2f world_pos =
              render_target_.get_window().mapPixelToCoords(mouse_pos);

          if (TileClicked(tile_opt->get_bounds(), Vector2f(world_pos.x, world_pos.y))) {
            if (tile_opt->is_reachable()) {
              std::cerr << "Atteignable!" << std::endl;
            }
            else if (tile_opt->get_owner() == selected_level_->get_current_player()->id() && tile_opt->entity() != nullptr) {
              std::cerr << "À moi!" << std::endl;
              ColorReachableTiles(tile_opt);
            }
          }
        }
      }
    }
  }
}

void UserInterface::HandleEvent(const sf::Event& event) {
  gui_.handleEvent(event);

  if (current_state_ == UserInterfaceState::Game) {
    TileMapEvent(event);
  }
}

void UserInterface::Draw() { gui_.draw(); }

void UserInterface::SwitchState(UserInterfaceState new_state) {
  current_state_ = new_state;
  gui_.removeAllWidgets();
  switch (current_state_) {
    case UserInterfaceState::HomePage:
      SetupHomePage();
      break;
    case UserInterfaceState::LevelSelection:
      if (available_levels_.empty()) {
        available_levels_ = Level::GetAvailableLevels("assets/levels");
      }
      selected_level_ = nullptr;
      SetupLevelSelection();
      break;
    case UserInterfaceState::Game:
      SetupGame();
      break;
  }
}

void UserInterface::SetupHomePage() {
  auto play_button = tgui::Button::create("Play");
  play_button->setPosition("(&.width - 200) / 2", "(&.height - 60) / 2");
  play_button->setSize(200, 60);
  play_button->onClick(
      [this] { SwitchState(UserInterfaceState::LevelSelection); });
  gui_.add(play_button);

  auto exit_button = tgui::Button::create("Exit");
  exit_button->setSize(200, 60);
  exit_button->setPosition("(&.width - 200) / 2", "(&.height - 60) / 2 + 80");
  exit_button->onClick([this] { render_target_.get_window().close(); });
  gui_.add(exit_button);
}

void UserInterface::SetupLevelSelection() {
  auto grid_panel = tgui::Panel::create();
  grid_panel->setPosition("(&.width - 800) / 2", "50");
  grid_panel->setSize(800, "(&.height - 200)");
  grid_panel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  gui_.add(grid_panel);

  const float card_width = 220;
  const float card_height = 100;
  const float padding = 20;
  float current_x = padding;
  float current_y = padding;

  for (const auto& level : available_levels_) {
    auto card = tgui::Panel::create();
    card->setPosition(current_x, current_y);
    card->setSize(card_width, card_height);
    card->getRenderer()->setBorders(1);
    card->getRenderer()->setBorderColor(tgui::Color::Black);
    card->getRenderer()->setBackgroundColor(
        (selected_level_ && selected_level_ == level)
            ? tgui::Color(200, 200, 255)
            : tgui::Color::White);

    // Level name
    auto name_label = tgui::Label::create(level->name());
    name_label->setPosition(10, 10);
    name_label->setTextSize(16);
    name_label->getRenderer()->setTextColor(tgui::Color::Black);
    card->add(name_label);

    // Category name in blue
    auto category_label = tgui::Label::create(level->category());
    category_label->setPosition(10, 40);
    category_label->setTextSize(12);
    category_label->getRenderer()->setTextColor(tgui::Color::Blue);
    card->add(category_label);

    auto selectButton = tgui::Button::create("Select");
    selectButton->setSize(90, 30);
    selectButton->setPosition(card_width - 100, card_height - 40);

    if (selected_level_ && selected_level_ == level) {
      selectButton->setText("Deselect");
      selectButton->getRenderer()->setBackgroundColor(tgui::Color::Blue);
      selectButton->getRenderer()->setTextColor(tgui::Color::White);
      selectButton->getRenderer()->setBorderColor(tgui::Color::White);
    } else {
      selectButton->setEnabled(true);
      selectButton->getRenderer()->setBackgroundColor(
          tgui::Color(220, 220, 220));
      selectButton->getRenderer()->setTextColor(tgui::Color::Black);
      selectButton->getRenderer()->setBorderColor(tgui::Color::Black);
    }

    selectButton->onClick([this, level] {
      if (selected_level_ == level) {
        selected_level_ = nullptr;  // Deselects if already selected
      } else {
        selected_level_ = level;
        if (selected_level_ && !selected_level_->is_loaded())
          selected_level_->Load();
      }
      SetupLevelSelection();
    });
    card->add(selectButton);

    grid_panel->add(card);

    current_x += card_width + padding;
    if (current_x + card_width > 800) {  // Wraps to next row
      current_x = padding;
      current_y += card_height + padding;
    }
  }

  auto startButton = tgui::Button::create("Play Level");
  startButton->setSize(200, 60);
  startButton->setPosition("(&.width - 200) / 2", "(&.height - 60) / 2 + 250");
  startButton->setEnabled(is_level_selected());
  startButton->onClick([this] {
    if (is_level_selected()) SwitchState(UserInterfaceState::Game);
  });
  gui_.add(startButton);

  auto backButton = tgui::Button::create("Back");
  backButton->setSize(200, 60);
  backButton->setPosition("(&.width - 200) / 2", "(&.height - 60) / 2 + 320");
  backButton->onClick([this] { SwitchState(UserInterfaceState::HomePage); });
  gui_.add(backButton);

  auto exitButton = tgui::Button::create("Exit");
  exitButton->setSize(200, 60);
  exitButton->setPosition("(&.width - 200) / 2", "(&.height - 60) / 2 + 390");
  exitButton->onClick([this] { render_target_.get_window().close(); });
  gui_.add(exitButton);
}

void UserInterface::SetupGame() {
  if (!selected_level_) {
    auto errorLabel = tgui::Label::create(
        "Failed to load level: " +
        (selected_level_ ? selected_level_->name() : "Unknown"));
    errorLabel->setPosition("(&.width - width) / 2", "(&.height - 60) / 2");
    errorLabel->setTextSize(24);
    errorLabel->getRenderer()->setBackgroundColor(tgui::Color::White);
    errorLabel->getRenderer()->setPadding(20);
    errorLabel->getRenderer()->setBorders(1);
    errorLabel->getRenderer()->setTextColor(tgui::Color::Red);
    gui_.add(errorLabel);

    auto backButton = tgui::Button::create("Back");
    backButton->setSize(200, 60);
    backButton->setPosition("(&.width - 200) / 2", "(&.height - 60) / 2 + 100");
    backButton->onClick(
        [this] { SwitchState(UserInterfaceState::LevelSelection); });
    gui_.add(backButton);
    return;
  }

  auto infoPanel = tgui::Panel::create();
  infoPanel->setPosition(10, 10);
  infoPanel->setSize(400, 60);
  infoPanel->getRenderer()->setBackgroundColor(tgui::Color(30, 30, 30, 200));
  infoPanel->getRenderer()->setBorders(1);
  infoPanel->getRenderer()->setBorderColor(tgui::Color::White);

  // Level name
  auto levelLabel =
      tgui::Label::create("Playing level: " + selected_level_->name());
  levelLabel->setTextSize(16);
  levelLabel->getRenderer()->setTextColor(tgui::Color::White);
  levelLabel->setPosition("(&.width - width) / 2", 4);
  infoPanel->add(levelLabel);

  // Category name
  auto worldLabel =
      tgui::Label::create("World: " + selected_level_->category());
  worldLabel->setTextSize(14);
  worldLabel->getRenderer()->setTextColor(tgui::Color::Yellow);
  worldLabel->setPosition("(&.width - width) / 2", 32);
  infoPanel->add(worldLabel);

  gui_.add(infoPanel);
  auto backButton = tgui::Button::create("Back");
  backButton->setSize(200, 60);
  backButton->setPosition(20, "(&.height) - 80");  // bottom left corner
  backButton->onClick(
      [this] { SwitchState(UserInterfaceState::LevelSelection); });
  gui_.add(backButton);
  auto exitButton = tgui::Button::create("Exit");
  exitButton->setSize(200, 60);
  exitButton->setPosition("(&.width) - 220",
                          "(&.height) - 80");  // bottom right corner
  exitButton->onClick([this] { render_target_.get_window().close(); });
  gui_.add(exitButton);

  // display current player

  auto player = selected_level_->get_current_player();

  // Create a panel for the player info
  auto playerPanel = tgui::Panel::create();
  playerPanel->setSize(400, 40);
  playerPanel->setPosition("(&.width) - 420", 10);
  playerPanel->getRenderer()->setBackgroundColor(tgui::Color(30, 30, 30, 200));
  playerPanel->getRenderer()->setBorders(1);
  playerPanel->getRenderer()->setBorderColor(tgui::Color::White);

  // "Current player:" label
  auto playerTextLabel = tgui::Label::create("Current player:");
  playerTextLabel->setPosition(10, 8);
  playerTextLabel->setTextSize(16);
  playerTextLabel->getRenderer()->setTextColor(tgui::Color::White);
  playerPanel->add(playerTextLabel);

  // Player name label in yellow
  auto playerNameLabel = tgui::Label::create(player->name());
  playerNameLabel->setPosition(200, 8);
  playerNameLabel->setTextSize(16);
  playerNameLabel->getRenderer()->setTextColor(tgui::Color::Yellow);
  playerPanel->add(playerNameLabel);

  gui_.add(playerPanel);
}

}  // namespace konkr
