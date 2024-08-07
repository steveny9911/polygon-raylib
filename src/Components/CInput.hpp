#include "Component.hpp"

class CInput : public Component
{
public:
  bool up = false;
  bool left = false;
  bool right = false;
  bool down = false;

  CInput() = default;
};
