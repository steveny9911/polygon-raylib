#include <iostream>
#include <functional>

#include "raylib-cpp.hpp"
#include "Component.hpp"

class CTimer : public Component
{
public:
  bool isActive = true;
  double timeElapsed;
  double interval;
  bool repeat = false;
  std::function<void()> onInterval;

  CTimer() = default;
  CTimer(double interval, bool repeat, std::function<void()> callback = nullptr)
      : interval(interval), repeat(repeat), onInterval(callback) {}
};
