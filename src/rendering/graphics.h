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

#include <SFML/Graphics/RenderWindow.hpp>
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

class Texture;
class Sprite;
class CircleShape;
class RenderTarget;
class Color;

class Graphics {
 public:
  static std::unique_ptr<Texture> LoadTexture(const std::string& file_path);
  static std::unique_ptr<Sprite> CreateSprite(const Texture& texture,
                                              const IntRect& rect);
};

class Texture {
 public:
  Texture();
  ~Texture();

  Texture(Texture&&) noexcept;
  Texture& operator=(Texture&&) noexcept;
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;

 private:
  friend class Graphics;
  friend class Sprite;
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

class Sprite {
 public:
  Sprite();
  Sprite(const Texture& texture);
  ~Sprite();

  Sprite(Sprite&&) noexcept;
  Sprite& operator=(Sprite&&) noexcept;
  Sprite(const Sprite&) = delete;
  Sprite& operator=(Sprite&) = delete;

  void setOrigin(Vector2f origin);
  void setPosition(Vector2f position);

 private:
  friend class Graphics;
  friend class RenderTarget;
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

class CircleShape {
 public:
  CircleShape(float radius, int pointCount);
  ~CircleShape();

  CircleShape(CircleShape&&) noexcept;
  CircleShape& operator=(CircleShape&&) noexcept;
  CircleShape(const CircleShape&) = delete;
  CircleShape& operator=(const CircleShape&) = delete;

  void setOrigin(Vector2f origin);
  void setPosition(Vector2f position);
  void setFillColor(const Color& color);

 private:
  friend class RenderTarget;
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

class RenderTarget {
 public:
  RenderTarget(Vector2u size, const std::string& title);
  ~RenderTarget();

  sf::RenderWindow& getWindow();

  RenderTarget(RenderTarget&&) noexcept;
  RenderTarget& operator=(RenderTarget&&) noexcept;
  RenderTarget(const RenderTarget&) = delete;
  RenderTarget& operator=(const RenderTarget&) = delete;

  void draw(const CircleShape& shape);
  void draw(const Sprite& sprite);
  Vector2u getSize() const;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

class Color {
 public:
  Color(int r, int g, int b);
  ~Color();

  Color(Color&&) noexcept;
  Color& operator=(Color&&) noexcept;
  Color(const Color&) = delete;
  Color& operator=(const Color&) = delete;

  operator sf::Color() const;

 private:
  friend class CircleShape;
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace konkr

#endif  // KONKR_RENDERING_GRAPHICS_H