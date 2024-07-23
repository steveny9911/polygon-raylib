#include <unordered_map>
#include <memory>
#include <raylib-cpp.hpp>
#include "EntityManager.h"
#include "Action.h"

float SPEED = 10.0f;

typedef std::unordered_map<KeyboardKey, ActionName> ActionMap;

class Engine
{
private:
  raylib::Window m_window;
  EntityManager m_entityManager;
  ActionMap m_actionMap;

  size_t m_currentFrame = 0;
  bool m_paused = false;
  bool m_running = true;

  bool isRunning();
  void setPaused(bool paused);

  void sMovement();
  void sUserInput();
  void sDraw();
  void sCollision();

  void sDoAction(const Action &action);
  void registerAction(KeyboardKey input, const ActionName actionName);
  void registerActions();

  void sEnemySpawner();

  std::shared_ptr<Entity> m_player;
  void spawnPlayer();
  void spawnEnemy();
  void spawnBullet(std::shared_ptr<Entity> entity, const Vector2 &mousePos);

public:
  void run();

  Engine()
  {
    m_window = raylib::Window();
    m_window.Init(1080, 720, "Polygon Survivour");
    SetTargetFPS(120);

    registerActions();
    spawnPlayer();
  };
};

bool Engine::isRunning()
{
  return m_running & !m_window.ShouldClose();
}

void Engine::run()
{
  SetTargetFPS(60);
  while (isRunning())
  {
    m_entityManager.update();

    if (!m_paused)
    {
      sMovement();

      m_currentFrame++;
    }

    sUserInput();

    sDraw();
  }
}

void Engine::setPaused(bool paused)
{
  m_paused = paused;
}

void Engine::registerAction(KeyboardKey input, const ActionName actionName)
{
  m_actionMap[input] = actionName;
}

void Engine::registerActions()
{
  registerAction(KEY_W, ActionName::UP);
  registerAction(KEY_A, ActionName::LEFT);
  registerAction(KEY_S, ActionName::DOWN);
  registerAction(KEY_D, ActionName::RIGHT);
  registerAction(KEY_P, ActionName::PAUSE);
}

void Engine::spawnPlayer()
{
  auto player = m_entityManager.addEntity("Player");
  Vector2 position = m_window.GetSize() / 2.0f;
  Vector2 velocity = {0.0, 0.0};

  player->addComponent<CTransform>(position, velocity, 0.0);
  player->addComponent<CShape>(std::make_unique<Ring>(80.0f, 190.0f, RED));
  // player->addComponent<CShape>();
  // player->getComponent<CShape>().shape = std::make_unique<Ring>(80.0f, 190.0f, RED);
  player->addComponent<CInput>();

  // // player->cTransform = std::make_shared<CTransform>();
  // player->cShape = std::make_shared<ShapeRing>(80.0f, 190.0f, RED);
  // player->cInput = std::make_shared<CInput>();

  m_player = player;
}

void Engine::sDraw()
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

  EndDrawing();
}

void Engine::sUserInput()
{
  KeyboardKey input = KEY_NULL;
  while (input = static_cast<KeyboardKey>(GetKeyPressed()))
  {
    if (!m_actionMap.contains(input))
      return;

    sDoAction(Action(m_actionMap.at(input), ActionType::START));
  }

  for (auto &[key, _] : m_actionMap)
  {
    if (IsKeyUp(key))
      sDoAction(Action(m_actionMap.at(key), ActionType::END));
  }
}

void Engine::sMovement()
{
  auto &playerInput = m_player->getComponent<CInput>();
  auto &playerTransform = m_player->getComponent<CTransform>();

  playerTransform.velocity = Vector2Zero();

  if (playerInput.up)
    playerTransform.velocity.y = -1;
  if (playerInput.down)
    playerTransform.velocity.y = 1;
  if (playerInput.left)
    playerTransform.velocity.x = -1;
  if (playerInput.right)
    playerTransform.velocity.x = 1;

  playerTransform.velocity = Vector2Scale(Vector2Normalize(playerTransform.velocity), SPEED);

  for (auto e : m_entityManager.getEntities())
  {
    if (playerTransform.velocity.x != 0 || playerTransform.velocity.y != 0)
    {
      playerTransform.position.x += playerTransform.velocity.x;
      playerTransform.position.y += playerTransform.velocity.y;
    }
  }

  // if player moves to wall stop it
  /*
  if (0 > m_player->cTransform->position.x - m_player->cCollission->radius ||
      m_window.GetWidth() < m_player->cTransform->position.x + m_player->cCollission->radius)
  {
    m_player->cTransform->position.x -= m_player->cTransform->velocity.x;
  }

  if (0 > m_player->cTransform->position.y - m_player->cCollission->radius ||
      m_window.GetHeight() < m_player->cTransform->position.y + m_player->cCollission->radius)
  {
    m_player->cTransform->position.y -= m_player->cTransform->velocity.y;
  }
  */
}

void Engine::sDoAction(const Action &action)
{
  auto &playerInput = m_player->getComponent<CInput>();

  if (action.type() == ActionType::START)
  {
    if (action.name() == ActionName::UP)
      playerInput.up = true;
    else if (action.name() == ActionName::LEFT)
      playerInput.left = true;
    else if (action.name() == ActionName::DOWN)
      playerInput.down = true;
    else if (action.name() == ActionName::RIGHT)
      playerInput.right = true;
    else if (action.name() == ActionName::PAUSE)
      setPaused(!m_paused);
  }
  else if (action.type() == ActionType::END)
  {
    if (action.name() == ActionName::UP)
      playerInput.up = false;
    else if (action.name() == ActionName::LEFT)
      playerInput.left = false;
    else if (action.name() == ActionName::DOWN)
      playerInput.down = false;
    else if (action.name() == ActionName::RIGHT)
      playerInput.right = false;
  }
}
