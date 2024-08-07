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

        CTransform &transformB = entityB->getComponent<CTransform>();
        ColliderCircle *colliderB = dynamic_cast<ColliderCircle *>(entityB->getComponent<CCollision>().collider.get());
        // std::unique_ptr<ColliderCircle> colliderB(static_cast<ColliderCircle *>(colliderBRaw));

        std::cout << entityA->tag() << " isCollision " << entityB->tag() << " " << GetTime() <<"\n";

        if (entityA->getComponent<CCollision>().collider->isCollision(transformA.position, transformB.position, colliderB->radius))
        {
          std::cout << "Collision!" << "\n";
          // entityA->getComponent<CCollision>().onCollision(entityB);
        }


      }
    }
  }
};
