#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/ContextSettings.hpp>

#include <vector>
#include <iostream>

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "konkr");
    window.setFramerateLimit(144);

    sf::Texture texture;
    if (!texture.loadFromFile("../../assets/spritesheet.png"))
    {
        return -1;
    }

    sf::IntRect spriteRect({990, 0}, {32, 48});
    sf::Sprite sprite(texture, spriteRect);

    sprite.setScale({0.75f, 0.75f});


    // We're simply abusing a `CircleShape` here,
    // since a circle defined by 6 points IS a hexagon!
    sf::CircleShape hexagon(25, 6);
    hexagon.setOutlineColor(sf::Color::Black);
    hexagon.setOutlineThickness(5);
    hexagon.setFillColor(sf::Color::White);
    hexagon.setOrigin({25, 25});

    sf::Vector2f selectedPos = {-1, -1};
    bool isSelected = false;
    sf::Vector2f previousHexagonPosition = {-1, -1};

    // Drag and drop variables
    bool isDragging = false;
    sf::Vector2f dragOffset;
    bool isSpriteAttached = false; // Track if the sprite is attached to a hexagon
    sf::Vector2f attachedHexagonPosition = {-1, -1}; // Position of the hexagon the sprite is attached to

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()){
                window.close();
            } else if (event->is<sf::Event::MouseButtonPressed>()) {
                if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                    // Check if the mouse click is within the sprite's bounds
                    if (sprite.getGlobalBounds().contains(worldPos)) {
                        isDragging = true;
                        isSpriteAttached = false; // Detach from any previous hexagon
                        attachedHexagonPosition = {-1, -1};
                        dragOffset = sprite.getPosition() - worldPos;
                    }

                    isSelected = false;

                    for (int y = 0; y < 10; ++y) {
                        for (int x = 0; x < 10; ++x) {
                            // The horizontal/vertical offsets of 50 and 40
                            // might feel off, but that's due to the way SFML
                            // calculates the circle outline
                            hexagon.setPosition({(y % 2 ? 75 : 50) + x * 50.f, 50 + y * 40.f});
                            if (hexagon.getGlobalBounds().contains(worldPos)) {
                                selectedPos = hexagon.getPosition();
                                isSelected = true;
                                break;
                            }
                        }
                        if (isSelected) {
                            break;
                        }
                    }
                }
            } else if (event->is<sf::Event::MouseButtonReleased>()) {
                if (event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left) {
                    isDragging = false;
                    sf::Vector2f newAttachedHexagonPosition = {-1, -1};

                    // Check if the sprite is released over a valid hexagon
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                    for (int y = 0; y < 10; ++y) {
                        for (int x = 0; x < 10; ++x) {
                            hexagon.setPosition({(y % 2 ? 75 : 50) + x * 50.f, 50 + y * 40.f});
                            if (hexagon.getGlobalBounds().contains(worldPos)) {
                                // Attach the sprite to this hexagon
                                isSpriteAttached = true;
                                newAttachedHexagonPosition = hexagon.getPosition();
                                break;
                            }
                        }
                        if (isSpriteAttached) {
                            break;
                        }
                    }
                    if (isSpriteAttached && newAttachedHexagonPosition != attachedHexagonPosition) {
                        previousHexagonPosition = attachedHexagonPosition;
                        attachedHexagonPosition = newAttachedHexagonPosition;
                    } else {
                        isSpriteAttached = false;
                        attachedHexagonPosition = {-1, -1};
                    }
                }
            } else if (event->is<sf::Event::MouseMoved>()) {
                if (isDragging) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
                    sprite.setPosition(worldPos + dragOffset);
                }
            }
        }

        window.clear(sf::Color::White);

        for (int y = 0; y < 10; ++y) {
            for (int x = 0; x < 10; ++x) {
                // The horizontal/vertical offsets of 50 and 40
                // might feel off, but that's due to the way SFML
                // calculates the circle outline

                hexagon.setPosition({(y % 2 ? 75 : 50) + x * 50.f, 50 + y * 40.f});
                sf::Vector2f currentHexagonPosition = hexagon.getPosition();
                if (currentHexagonPosition == attachedHexagonPosition) {
                    hexagon.setFillColor(sf::Color::Green);
                }
                else if (currentHexagonPosition == previousHexagonPosition) {
                     hexagon.setFillColor(sf::Color(x * 25, y * 25, 0));
                }
                else {
                    hexagon.setFillColor(sf::Color(x * 25, y * 25, 0));
                }

                window.draw(hexagon);

                // Center the sprite inside the hexagon
                sf::FloatRect hexagonBounds = hexagon.getLocalBounds();
                sf::FloatRect spriteBounds = sprite.getLocalBounds();

                // If the sprite is attached to a hexagon, position it in the center
                if (isSpriteAttached && attachedHexagonPosition == hexagon.getPosition()) {
                    sprite.setPosition({
                        hexagon.getPosition().x - spriteBounds.getCenter().x * 0.75f,
                        hexagon.getPosition().y - spriteBounds.getCenter().y * 0.75f
                    });
                    window.draw(sprite);
                } else if (isDragging) {
                    // If dragging, draw the sprite at its current position
                    window.draw(sprite);
                } else {
                    // Otherwise, don't draw the sprite (it's not attached or being dragged)
                    if (!isSpriteAttached) {
                        sprite.setPosition({-100, -100}); // Move off-screen
                    }
                }
            }
        }
        window.display();
    }
}
