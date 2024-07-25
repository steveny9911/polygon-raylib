#include <iostream>

#include "raylib-cpp.hpp"
#include "Component.hpp"

class CIntervalTimer : public Component
{
public:
  double prevTime = 0.0;
  double interval = 0.0;

  CIntervalTimer() = default;
  CIntervalTimer(double interval) : interval(interval) {}
};
