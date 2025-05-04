// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "rendering/graphics.h"

#include <SFML/Graphics.hpp>
#include <algorithm>

namespace konkr {

// Texture
Texture::Texture() = default;
Texture::~Texture() = default;

// Sprite
Sprite::Sprite(const Texture& texture)
    : texture_(&texture), sprite_(texture.texture_) {}
Sprite::~Sprite() = default;

void Sprite::setOrigin(Vector2f origin) {
  sprite_.setOrigin({origin.x, origin.y});
}

void Sprite::setPosition(Vector2f position) {
  sprite_.setPosition({position.x, position.y});
}

// Transform
Transform::Transform(const sf::Transform& sfml_transform) {
  const float* mat = sfml_transform.getMatrix();
  for (int i = 0; i < 12; ++i) m_matrix[i] = mat[i];
  m_matrix[12] = 0.f; m_matrix[13] = 0.f; m_matrix[14] = 0.f; m_matrix[15] = 1.f;
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

void CircleShape::setOrigin(Vector2f origin) {
  circle_shape_.setOrigin({origin.x, origin.y});
}

void CircleShape::setPosition(Vector2f position) {
  circle_shape_.setPosition({position.x, position.y});
}

void CircleShape::setFillColor(const Color& color) {
  circle_shape_.setFillColor(color.color_);
}

Transform CircleShape::getTransform() const {
  return circle_shape_.getTransform();
}

std::size_t CircleShape::getPointCount() const {
  return circle_shape_.getPointCount();
}

Vector2f CircleShape::getPoint(std::size_t index) const {
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

void RenderTarget::draw(const Sprite& sprite) { window_.draw(sprite.sprite_); }

Vector2u RenderTarget::getSize() const {
  auto size = window_.getSize();
  return {size.x, size.y};
}

sf::RenderWindow& RenderTarget::getWindow() { return window_; }

// Color
Color::Color(int r, int g, int b) : color_(r, g, b) {}
Color::~Color() = default;
Color::operator sf::Color() const { return color_; }

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
      {rect.pos.x, rect.pos.y}, {rect.size.width, rect.size.height}));
  return sprite;
}

}  // namespace konkr