#include "raylib-cpp.hpp"
#include "Shapes.hpp"

class ShapeRing : public Shape
{
public:
  float innerRadius;
  float outterRadius;

  ShapeRing() = default;
  ShapeRing(float inner, float outter, Color color)
      : Shape(color), innerRadius(inner), outterRadius(outter)
  {
  }

  void draw(const Vector2 &position) override
  {
    DrawRing(position, innerRadius, outterRadius, 0.0f, 360.0f, 0.0f, color);
  }
};
