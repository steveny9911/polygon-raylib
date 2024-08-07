#pragma once

#include "raylib-cpp.hpp"
#include "Collider.hpp"

class ColliderCircle : public Collider
{
public:
  float radius;

  ColliderCircle() : Collider(ColliderShape::CircleCollider) {};
  ColliderCircle(float radis) : Collider(ColliderShape::CircleCollider), radius(radis) {};

  bool isCollision(Vector2 &selfPos, Vector2 &otherPos, float otherRadius) override
  {
    std::cout << "selfPos:" << selfPos.x << " otherPos:" << otherPos.x << " otherRadius:" << otherRadius << "\n";

    return CheckCollisionCircles(
        selfPos,
        radius,
        otherPos,
        otherRadius);
  }
};
