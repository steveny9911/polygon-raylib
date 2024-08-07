#pragma once

#include "raylib-cpp.hpp"

class Shape
{
public:
  Color color;
  float rotation = 0.0f;
  virtual void draw(const Vector2 &position) {};

  Shape() = default;
  Shape(Color color) : color(color) {}
};

#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "ShapeRing.hpp"

#endif
