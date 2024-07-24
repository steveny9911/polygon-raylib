#include "raylib-cpp.hpp"
#include "Component.hpp"

class CTransform : public Component
{
public:
  Vector2 position = {0.0f, 0.0f}; // center
  Vector2 velocity = {0.0f, 0.0f};
  float angle = 0.0f;

  CTransform() = default;
  CTransform(const Vector2 &position, const Vector2 &velocity, float angle = 0.0f)
      : position(position), velocity(velocity), angle(angle) {}
};
