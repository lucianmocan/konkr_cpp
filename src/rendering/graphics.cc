// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.

#include "rendering/graphics.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <algorithm>  // for std::clamp

namespace konkr {
struct Texture::Impl {
  sf::Texture texture;
  Impl() = default;
};

struct Sprite::Impl {
  sf::Sprite sprite;
  // Use only the constructor that takes a Texture.
  Impl(const Texture& texture) : sprite(texture.impl_->texture) {}
};

struct CircleShape::Impl {
  sf::CircleShape circle_shape;
};

struct RenderTarget::Impl {
  sf::RenderWindow window;
  Impl(Vector2u size, const std::string& title)
      : window(sf::VideoMode({size.x, size.y}), title) {}
};

struct Color::Impl {
  sf::Color color;
  Impl(int r, int g, int b) : color(r, g, b) {}

  operator sf::Color() const { return color; };
};

// Texture
Texture::Texture() : impl_(std::make_unique<Impl>()) {}
Texture::~Texture() = default;
Texture::Texture(Texture&&) noexcept = default;
Texture& Texture::operator=(Texture&&) noexcept = default;

// Sprite
Sprite::Sprite(const Texture& texture)
    : impl_(std::make_unique<Impl>(texture)) {}
Sprite::~Sprite() = default;
Sprite::Sprite(Sprite&&) noexcept = default;
Sprite& Sprite::operator=(Sprite&&) noexcept = default;

void Sprite::setOrigin(Vector2f origin) {
  impl_->sprite.setOrigin({origin.x, origin.y});
}

void Sprite::setPosition(Vector2f position) {
  impl_->sprite.setPosition({position.x, position.y});
}

// CircleShape
CircleShape::CircleShape(float radius, int pointCount)
    : impl_(std::make_unique<Impl>()) {
  if (radius < 0.0f) radius = 0.0f;    // Ensure non-negative radius
  if (pointCount < 3) pointCount = 3;  // Ensure valid polygon
  impl_->circle_shape.setRadius(radius);
  impl_->circle_shape.setPointCount(pointCount);
}
CircleShape::~CircleShape() = default;
CircleShape::CircleShape(CircleShape&&) noexcept = default;
CircleShape& CircleShape::operator=(CircleShape&&) noexcept = default;

void CircleShape::setOrigin(Vector2f origin) {
  impl_->circle_shape.setOrigin({origin.x, origin.y});
}

void CircleShape::setPosition(Vector2f position) {
  impl_->circle_shape.setPosition({position.x, position.y});
}

void CircleShape::setFillColor(const Color& color) {
  impl_->circle_shape.setFillColor(color.impl_->color);
}

// RenderTarget
RenderTarget::RenderTarget(Vector2u size, const std::string& title)
    : impl_(std::make_unique<Impl>(size, title)) {}
RenderTarget::~RenderTarget() = default;
RenderTarget::RenderTarget(RenderTarget&&) noexcept = default;
RenderTarget& RenderTarget::operator=(RenderTarget&&) noexcept = default;

void RenderTarget::draw(const CircleShape& shape) {
  impl_->window.draw(shape.impl_->circle_shape);
}

void RenderTarget::draw(const Sprite& sprite) {
  impl_->window.draw(sprite.impl_->sprite);
}

Vector2u RenderTarget::getSize() const {
  auto size = impl_->window.getSize();
  return {size.x, size.y};
}

// Color
Color::Color(int r, int g, int b) : impl_(std::make_unique<Impl>(r, g, b)) {}
Color::~Color() = default;
Color::Color(Color&&) noexcept = default;
Color& Color::operator=(Color&&) noexcept = default;
Color::operator sf::Color() const { return impl_->color; }

// Graphics
std::unique_ptr<Texture> Graphics::LoadTexture(const std::string& file_path) {
  auto texture = std::make_unique<Texture>();
  if (!texture->impl_->texture.loadFromFile(file_path)) {
    return nullptr;
  }
  return texture;
}

std::unique_ptr<Sprite> Graphics::CreateSprite(const Texture& texture,
                                               const IntRect& rect) {
  auto sprite = std::make_unique<Sprite>(texture);
  sprite->impl_->sprite.setTextureRect(sf::IntRect(
      {rect.pos.x, rect.pos.y}, {rect.size.width, rect.size.height}));
  return sprite;
}

sf::RenderWindow& RenderTarget::getWindow() { return impl_->window; }

}  // namespace konkr