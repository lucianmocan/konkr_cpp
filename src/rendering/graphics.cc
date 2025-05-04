// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "rendering/graphics.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <algorithm>

namespace konkr {

// Texture
Texture::Texture() = default;
Texture::~Texture() = default;

// Sprite
Sprite::Sprite(const Texture& texture)
    : texture_(&texture), sprite_(texture.texture_) {}
Sprite::~Sprite() = default;

void Sprite::set_origin(Vector2f origin) {
  sprite_.setOrigin({origin.x, origin.y});
}

void Sprite::set_position(Vector2f position) {
  sprite_.setPosition({position.x, position.y});
}

// CircleShape
CircleShape::CircleShape(float radius, int pointCount) {
  if (radius < 0.0f) radius = 0.0f;
  if (pointCount < 3) pointCount = 3;
  circle_shape_.setRadius(radius);
  circle_shape_.setPointCount(pointCount);
}
CircleShape::~CircleShape() = default;

void CircleShape::set_origin(Vector2f origin) {
  circle_shape_.setOrigin({origin.x, origin.y});
}

void CircleShape::set_position(Vector2f position) {
  circle_shape_.setPosition({position.x, position.y});
}

void CircleShape::set_fill_color(const Color& color) {
  circle_shape_.setFillColor(color.color_);
}

Font::Font(const std::string& path)
{
  if (font_.openFromFile(path)) {
    loaded_ = true;
  }
}
Font::~Font() = default;


Text::Text(const Font& font, const std::string& text, unsigned int size)
  : text_(sf::Text(font.font_, text, size)) {}
Text::~Text() = default;

void Text::set_fill_color(const Color& color) {
  text_.setFillColor(color.color_);
}

void Text::set_position(Vector2f position) {
  text_.setPosition({position.x, position.y});
}

void Text::set_outline_color(const Color& color) {
  text_.setOutlineColor(color.color_);
}

void Text::set_outline_thickness(float thickness) {
  text_.setOutlineThickness(thickness);
}

void Text::set_origin(Vector2f origin) {
  text_.setOrigin({origin.x, origin.y});
}

FloatRect Text::get_local_bounds() const {
  sf::FloatRect bounds = text_.getLocalBounds();
  return FloatRect({bounds.position.x, bounds.position.y},
                   {bounds.size.x, bounds.size.y});
}

// RenderTarget
RenderTarget::RenderTarget(Vector2u size, const std::string& title)
    : window_(sf::VideoMode({size.x, size.y}), title) {}
RenderTarget::~RenderTarget() = default;

void RenderTarget::draw(const CircleShape& shape) {
  window_.draw(shape.circle_shape_);
}

void RenderTarget::draw(const Text& text) { window_.draw(text.text_); }

void RenderTarget::draw(const Sprite& sprite) { window_.draw(sprite.sprite_); }

Vector2u RenderTarget::get_size() const {
  auto size = window_.getSize();
  return {size.x, size.y};
}

sf::RenderWindow& RenderTarget::get_window() { return window_; }

// Color
Color::Color(int r, int g, int b) : color_(r, g, b) {}
Color::~Color() = default;
Color::operator sf::Color() const { return color_; }
Color Color::Yellow = Color(sf::Color::Yellow.r,
            sf::Color::Yellow.g, sf::Color::Yellow.b);
Color Color::Black = Color(sf::Color::Black.r,
            sf::Color::Black.g, sf::Color::Black.b); 

// Graphics
std::unique_ptr<Texture> Graphics::LoadTexture(const std::string& file_path) {
  auto texture = std::make_unique<Texture>();
  if (!texture->texture_.loadFromFile(file_path)) {
    return nullptr;
  }
  return texture;
}

std::unique_ptr<Sprite> Graphics::CreateSprite(const Texture& texture,
                                               const IntRect& rect) {
  auto sprite = std::make_unique<Sprite>(texture);
  sprite->sprite_.setTextureRect(sf::IntRect(
      {rect.pos.x, rect.pos.y}, {rect.size.x, rect.size.y}));
  return sprite;
}

}  // namespace konkr