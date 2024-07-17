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
  CName(std::string &name) : name(name) {};
};

class CInput : public Component
{
public:
  bool up = false;
  bool left = false;
  bool right = false;
  bool down = false;

  CInput() {}
};

class CTransform
{
public:
  Vector2 position = {0.0, 0.0}; // center
  Vector2 velocity = {0.0, 0.0};
  float angle = 0;

  CTransform(const Vector2 &p, const Vector2 &v, float a)
      : position(p), velocity(v), angle(a) {}
};

class CShape
{
public:
  Color color;
  float rotation = 0.0f;
  virtual void draw(const Vector2 &position) = 0;

  CShape() {}
  CShape(Color color) :color(color)
  {
  }
};

class ShapeRing : public CShape
{
public:
  float innerRadius;
  float outterRadius;

  ShapeRing(float inner, float outter, Color color)
      : CShape(color), innerRadius(inner), outterRadius(outter)
  {
  }

  void draw(const Vector2 &position) override
  {
    DrawRing(position, innerRadius, outterRadius, 0.0f, 360.0f, 0.0f, color);
  }
};

class ShapeRectangle : public CShape
{
  float width;
  float height;

  ShapeRectangle(float w, float h, Color c)
      : CShape(), width(w), height(h) {}
};

class ShapePoly : public CShape
{
  int sides;
  float radius;
};

class CCollision : public Component
{
public:
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
