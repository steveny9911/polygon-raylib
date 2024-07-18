#include "raylib-cpp.hpp"
#include <iostream>

class Component
{
public:
  bool has = false;
};

class CName : public Component
{
public:
  std::string name;

  CName() = default;
  CName(std::string &name) : name(name) {}
};

class CInput : public Component
{
public:
  bool up = false;
  bool left = false;
  bool right = false;
  bool down = false;

  CInput() = default;
};

class CTransform : public Component
{
public:
  Vector2 position = {0.0, 0.0}; // center
  Vector2 velocity = {0.0, 0.0};
  float angle = 0;

  CTransform() = default;
  CTransform(const Vector2 &p, const Vector2 &v, float a)
      : position(p), velocity(v), angle(a) {}
};



class Shape {
public:
  Color color;
  float rotation = 0.0f;
  virtual void draw(const Vector2 &position) {};

  Shape() = default;
  Shape(Color color) :color(color) {}
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

class Rect : public Shape
{
public:
  float width;
  float height;

  Rect() = default;
  Rect(float w, float h, Color c)
      : Shape(), width(w), height(h) {}
};

class ShapePoly : public Shape
{
public:
  int sides;
  float radius;

  ShapePoly() = default;
};

class CShape : public Component
{
public:
  std::unique_ptr<Shape> shape;
  CShape() = default;
  CShape(std::unique_ptr<Shape> shape) : shape(std::move(shape)) {}
};

class CCollision : public Component
{
public:
  CCollision() = default;
};

class CollisionTriangle : public CCollision
{
};

class CollisionCircle : public CCollision
{
  float radius;

};

class CollisionRectangle : public CCollision
{
};
