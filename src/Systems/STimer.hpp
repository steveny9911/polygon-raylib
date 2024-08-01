#include "System.hpp"

class STimer : public System
{
public:
  STimer(EntityManager &manager) : System(manager) {};

  void update() override
  {
    for (auto e : m_entityManager.getEntities())
    {
      if (!e->hasComponent<CTimer>())
        continue;

      auto &timer = e->getComponent<CTimer>();
      if (!timer.isActive)
        return;

      timer.timeElapsed += GetFrameTime(); // add delta time (seconds)
      if (timer.timeElapsed >= timer.interval)
      {
        timer.onInterval();

        if (timer.repeat)
          timer.timeElapsed = 0.0;
        else
          timer.isActive = false;
      }
    }
  }
};
