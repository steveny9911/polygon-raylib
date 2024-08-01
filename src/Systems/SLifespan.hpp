#include "System.hpp"

class SLifespan : public System
{
public:
  SLifespan(EntityManager &manager) : System(manager) {};

  void update() override
  {
    for (auto e : m_entityManager.getEntities())
    {
      if (!e->hasComponent<CLifespan>())
        continue;

      auto &lifespan = e->getComponent<CLifespan>();
      if (GetTime() - lifespan.createdAt > lifespan.duration)
      {
        e->destroy();
      }
    }
  }
};
