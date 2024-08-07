#pragma once

#include <memory>
#include <functional>
#include "raylib-cpp.hpp"
#include "Component.hpp"
#include "../Colliders/Colliders.hpp"

class CCollision : public Component
{
public:
  std::unique_ptr<Collider> collider;
  std::function<void()> onCollision;

  CCollision() = default;
  CCollision(std::unique_ptr<Collider> collider) : collider(std::move(collider)) {};
};
