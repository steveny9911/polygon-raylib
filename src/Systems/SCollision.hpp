#include "System.hpp"
#include "../Colliders/ColliderCircle.hpp"

class SCollision : public System
{
public:
  SCollision(EntityManager &manager) : System(manager) {};

  void update() override
  {
    for (auto entityA : m_entityManager.getEntities())
    {
      if (!entityA->hasComponent<CTransform>() || !entityA->hasComponent<CCollision>())
        continue;

      CTransform &transformA = entityA->getComponent<CTransform>();

      for (auto entityB : m_entityManager.getEntities())
      {
        // Avoid duplicate checks and self-collision
        if (entityA->id() >= entityB->id())
          continue;

        if (!entityB->hasComponent<CTransform>() || !entityB->hasComponent<CCollision>())
          continue;

        CCollision &collisionA = entityA->getComponent<CCollision>();

        CTransform &transformB = entityB->getComponent<CTransform>();
        CCollision &collisionB = entityB->getComponent<CCollision>();
        ColliderCircle *colliderB = dynamic_cast<ColliderCircle *>(entityB->getComponent<CCollision>().collider.get());

        if (collisionA.collider->isCollision(transformA.position, transformB.position, colliderB->radius))
        {
          collisionA.onCollision(entityB);
          collisionB.onCollision(entityA);
        }
      }
    }
  }
};
