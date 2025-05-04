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
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <string>
#include <vector>

namespace konkr {

template <typename T>
struct Position {
  T x;
  T y;
  Position() : x(0), y(0) {}
  Position(T x, T y) : x(x), y(y) {}
};

template <typename T>
struct Size {
  T x;
  T y;
  Size() : x(0), y(0) {}
  Size(T x, T y) : x(x), y(y) {}
};

template <typename T>
struct Rect {
  Position<T> pos;
  Size<T> size;
  Rect() : pos(), size() {}
  Rect(Position<T> pos, Size<T> sz) : pos(pos), size(sz) {}
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

template <typename T>
struct Vector2 {
  T x;
  T y;
  Vector2(T x, T y) : x(x), y(y) {}
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned int>;

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

  void set_origin(Vector2f origin);
  void set_position(Vector2f position);

 private:
  friend class Graphics;
  friend class RenderTarget;
  const Texture* texture_;
  sf::Sprite sprite_;
};

class Transform {
 public:
  Transform(float a00, float a01, float a02, float a10, float a11, float a12,
            float a20, float a21, float a22);
  Transform(const sf::Transform& sfml_transform);  // Conversion constructor
  ~Transform();

  Vector2f transformPoint(Vector2f point) const;

 private:
  std::array<float, 16> m_matrix;
};

class CircleShape {
 public:
  CircleShape(float radius, int pointCount);
  CircleShape(const CircleShape& c);
  ~CircleShape();

  void set_origin(Vector2f origin);
  void set_position(Vector2f position);
  void set_fill_color(const class Color& color);

  Transform get_transform() const;
  std::size_t get_point_count() const;
  Vector2f get_point(std::size_t index) const;

 private:
  friend class RenderTarget;
  sf::CircleShape circle_shape_;
};

class Color {
 public:
  Color(int r, int g, int b);
  ~Color();

  static Color Yellow;
  static Color Black;

  operator sf::Color() const;

 private:
  friend class CircleShape;
  friend class Text;
  sf::Color color_;
};

class Font {
 public:
  Font() = default;
  Font(const std::string& path);
  ~Font();

  bool is_loaded() const { return loaded_; }

 private:
  friend class Text;
  sf::Font font_;
  bool loaded_ = false;
};

class Text {
 public:
  Text(const Font& font, const std::string& text, unsigned int size);
  ~Text();

  void set_fill_color(const Color& color);
  void set_position(Vector2f position);
  void set_outline_color(const Color& color);
  void set_outline_thickness(float thickness);
  void set_origin(Vector2f origin);
  FloatRect get_local_bounds() const;

 private:
  friend class RenderTarget;
  sf::Text text_;
};

class RenderTarget {
 public:
  RenderTarget(Vector2u size, const std::string& title);
  ~RenderTarget();

  void draw(const CircleShape& shape);
  void draw(const Sprite& sprite);
  void draw(const Text& text);
  Vector2u get_size() const;
  sf::RenderWindow& get_window();

 private:
  sf::RenderWindow window_;
};

class Graphics {
 public:
  static std::unique_ptr<Texture> LoadTexture(const std::string& file_path);
  static std::unique_ptr<Sprite> CreateSprite(const Texture& texture,
                                              const IntRect& rect);
};

}  // namespace konkr

#endif  // KONKR_RENDERING_GRAPHICS_H