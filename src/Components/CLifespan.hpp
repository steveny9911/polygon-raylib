#include <iostream>

#include "raylib-cpp.hpp"
#include "Component.hpp"

class CLifespan : public Component
{
public:
  double createdAt;
  double duration;

  CLifespan() = default;
  CLifespan(double duration, double createdAt) 
  : duration(duration), createdAt(createdAt) {}
};
