// Copyright 2025 Lucian Mocan, Antoine Waehren. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for details.
//
// graphics.h
//
// Provides a graphics abstraction layer using SFML for rendering textures,
// sprites, shapes, and managing render targets.

#ifndef KONKR_RENDERING_GRAPHICS_H
#define KONKR_RENDERING_GRAPHICS_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <string>
#include <vector>

namespace konkr {

struct Position {
  int x;
  int y;
};

struct Size {
  int width;
  int height;
};

template <typename T>
struct Vector2 {
  T x;
  T y;
  Vector2(T x, T y) : x(x), y(y) {}
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned int>;

struct IntRect {
  Position pos;
  Size size;

  IntRect(Position pos, Size sz) : pos(pos), size(sz) {}
  IntRect() : pos{0, 0}, size{0, 0} {}
};

class Texture {
 public:
  Texture();
  ~Texture();

 private:
  friend class Graphics;
  friend class Sprite;
  sf::Texture texture_;
};

class Sprite {
 public:
  Sprite(const Texture& texture);
  ~Sprite();

  void setOrigin(Vector2f origin);
  void setPosition(Vector2f position);

 private:
  friend class Graphics;
  friend class RenderTarget;
  const Texture* texture_;
  sf::Sprite sprite_;
};

class Transform {
 public:
  Transform(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22);
  Transform(const sf::Transform& sfml_transform); // Conversion constructor
  ~Transform();

  Vector2f transformPoint(Vector2f point) const;

 private:
  std::array<float, 16> m_matrix;
};

class CircleShape {
 public:
  CircleShape(float radius, int pointCount);
  CircleShape(CircleShape&& c);
  ~CircleShape();

  void setOrigin(Vector2f origin);
  void setPosition(Vector2f position);
  void setFillColor(const class Color& color);
  Transform getTransform() const;
  std::size_t getPointCount() const;
  Vector2f getPoint(std::size_t index) const;

 private:
  friend class RenderTarget;
  sf::CircleShape circle_shape_;
};

class RenderTarget {
 public:
  RenderTarget(Vector2u size, const std::string& title);
  ~RenderTarget();

  void draw(const CircleShape& shape);
  void draw(const Sprite& sprite);
  Vector2u getSize() const;
  sf::RenderWindow& getWindow();

 private:
  sf::RenderWindow window_;
};

class Color {
 public:
  Color(int r, int g, int b);
  ~Color();

  operator sf::Color() const;

 private:
  friend class CircleShape;
  sf::Color color_;
};

class Graphics {
 public:
  static std::unique_ptr<Texture> LoadTexture(const std::string& file_path);
  static std::unique_ptr<Sprite> CreateSprite(const Texture& texture,
                                              const IntRect& rect);
};

}  // namespace konkr

#endif  // KONKR_RENDERING_GRAPHICS_H