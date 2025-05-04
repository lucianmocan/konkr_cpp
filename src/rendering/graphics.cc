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

// Transform
Transform::Transform(const sf::Transform& sfml_transform) {
  const float* mat = sfml_transform.getMatrix();
  for (int i = 0; i < 12; ++i) m_matrix[i] = mat[i];
  m_matrix[12] = 0.f;
  m_matrix[13] = 0.f;
  m_matrix[14] = 0.f;
  m_matrix[15] = 1.f;
}

Transform::~Transform() = default;

Vector2f Transform::transformPoint(Vector2f point) const {
  return {m_matrix[0] * point.x + m_matrix[4] * point.y + m_matrix[12],
          m_matrix[1] * point.x + m_matrix[5] * point.y + m_matrix[13]};
}

// CircleShape
CircleShape::CircleShape(float radius, int pointCount) {
  if (radius < 0.0f) radius = 0.0f;
  if (pointCount < 3) pointCount = 3;
  circle_shape_.setRadius(radius);
  circle_shape_.setPointCount(pointCount);
}
CircleShape::~CircleShape() = default;
CircleShape::CircleShape(const CircleShape& c) = default;

void CircleShape::set_origin(Vector2f origin) {
  circle_shape_.setOrigin({origin.x, origin.y});
}

void CircleShape::set_position(Vector2f position) {
  circle_shape_.setPosition({position.x, position.y});
}

void CircleShape::set_fill_color(const Color& color) {
  circle_shape_.setFillColor(color.color_);
}

Font::Font(const std::string& path) {
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

Transform CircleShape::get_transform() const {
  return circle_shape_.getTransform();
}

FloatRect CircleShape::get_global_bounds() const {
  sf::FloatRect bounds = circle_shape_.getGlobalBounds();
  return FloatRect({bounds.position.x, bounds.position.y},
    {bounds.size.x, bounds.size.y});
}

std::size_t CircleShape::get_point_count() const {
  return circle_shape_.getPointCount();
}

Vector2f CircleShape::get_point(std::size_t index) const {
  sf::Vector2f v = circle_shape_.getPoint(index);
  return Vector2f({v.x, v.y});
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
Color Color::Yellow =
    Color(sf::Color::Yellow.r, sf::Color::Yellow.g, sf::Color::Yellow.b);
Color Color::Black =
    Color(sf::Color::Black.r, sf::Color::Black.g, sf::Color::Black.b);

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
  sprite->sprite_.setTextureRect(
      sf::IntRect({rect.pos.x, rect.pos.y}, {rect.size.x, rect.size.y}));
  return sprite;
}

}  // namespace konkr