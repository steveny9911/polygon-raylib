#pragma once

#include "raylib-cpp.hpp"

enum ColliderShape
{
  CircleCollider,
  RectangleCollider,
  TriangleCollider
};

class Collider
{
public:
  ColliderShape colliderShape;
  virtual bool isCollision(Vector2 &selfPos, Vector2 &otherPos, float otherRadius) = 0;

  Collider() = default;
  Collider(ColliderShape colliderShape) : colliderShape(colliderShape) {};

  virtual ~Collider() = default;
};