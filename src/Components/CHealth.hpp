#include "Component.hpp"

class CHealth : public Component
{
private:
  int health = 0;

public:
  CHealth() = default;
  CHealth(int health) : health(health) {}

  void decrement()
  {
    if (health <= 0)
      return;

    health -= 1;
  }

  void increment() { health += 1; }
};
