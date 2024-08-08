#pragma once

#include <memory>
#include <functional>
#include "raylib-cpp.hpp"
#include "Component.hpp"
#include "../Colliders/Colliders.hpp"

class Entity;

class CCollision : public Component
{
public:
  std::unique_ptr<Collider> collider;
  std::function<void(std::shared_ptr<Entity>)> onCollision;

  CCollision() = default;
  CCollision(std::unique_ptr<Collider> collider,
             std::function<void(std::shared_ptr<Entity>)> callback = nullptr) : collider(std::move(collider)), onCollision(callback) {};
};
