#include "System.hpp"

class SDraw : public System
{
public:
  SDraw(EntityManager &manager) : System(manager) {};

  void update() override
  {
    BeginDrawing();
    ClearBackground(BLACK);

    for (auto e : m_entityManager.getEntities())
    {
      if (e->hasComponent<CShape>())
      {
        e->getComponent<CShape>().shape->draw(e->getComponent<CTransform>().position);
      }
    }

    DrawText(TextFormat("Num of Entities: %i", m_entityManager.getEntities().size()), 0, 0, 20, RED);
    DrawText(TextFormat("Elapsed Time: %02.02f ms", GetFrameTime() * 1000), 0, 20, 20, RED);

    EndDrawing();
  }
};
