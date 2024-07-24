#include <iostream>

#include "raylib-cpp.hpp"
#include "Component.hpp"
#include "Shape/Shapes.hpp"

class CShape : public Component
{
public:
  std::unique_ptr<Shape> shape;
  CShape() = default;
  CShape(std::unique_ptr<Shape> shape) : shape(std::move(shape)) {}
};
