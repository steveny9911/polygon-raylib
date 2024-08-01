#include <memory>
#include <chrono>
#include <raylib-cpp.hpp>
#include "Constants.hpp"
#include "EntityManager.hpp"
#include "Action.hpp"

class Game
{
private:
  raylib::Window m_window;
  EntityManager m_entityManager;
  ActionMap m_actionMap;

  size_t m_currentFrame = 0;
  bool m_paused = false;
  bool m_running = true;

  bool isRunning();

  void registerAction(KeyboardKey input, const ActionName actionName);
  void registerActions();

  std::shared_ptr<Entity> m_player;
  void spawnPlayer();
  void spawnEnemies();

public:
  void run();
  bool isPaused();
  void setPaused(bool paused);

  Game();
};
