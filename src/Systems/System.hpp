#pragma once

#include "../EntityManager.hpp"

class System
{
protected:
  EntityManager &m_entityManager;

public:
  System(EntityManager &manager) : m_entityManager(manager) {}
  virtual ~System() = default;

  virtual void update() = 0;
};
