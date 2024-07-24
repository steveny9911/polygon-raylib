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

class Ring : public Shape
{
public:
  float innerRadius;
  float outterRadius;

  Ring() = default;
  Ring(float inner, float outter, Color color)
      : Shape(color), innerRadius(inner), outterRadius(outter)
  {
  }

  void draw(const Vector2 &position) override
  {
    DrawRing(position, innerRadius, outterRadius, 0.0f, 360.0f, 0.0f, color);
  }
};
