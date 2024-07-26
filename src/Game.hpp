#include <unordered_map>
#include <memory>
#include <chrono>
#include <raylib-cpp.hpp>
#include "EntityManager.hpp"
#include "Components/Components.hpp"
#include "Action.hpp"

float SPEED = 10.0f;
double BULLET_INTERVAL = 1.0;

typedef std::unordered_map<KeyboardKey, ActionName> ActionMap;

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
  void setPaused(bool paused);

  void sMovement();
  void sUserInput();
  void sDraw();
  void sUpdate();
  void sCollision();
  void sTimer();
  void sLifespan();

  void sDoAction(const Action &action);
  void registerAction(KeyboardKey input, const ActionName actionName);
  void registerActions();

  double m_prevTime;

  std::shared_ptr<Entity> m_player;
  void spawnPlayer();
  void spawnEnemies();

public:
  void run();

  Game()
  {
    m_window = raylib::Window();
    m_window.Init(1080, 720, "Polygon Survivour");
    SetTargetFPS(120);

    registerActions();
    spawnPlayer();
    spawnEnemies();
  };
};

bool Game::isRunning()
{
  return m_running & !m_window.ShouldClose();
}

void Game::run()
{
  SetTargetFPS(60);

  while (isRunning())
  {
    m_entityManager.update();

    if (!m_paused)
    {
      sMovement();
      sTimer();
      sLifespan();

      m_currentFrame++;
    }

    sUserInput();
    sDraw();
  }
}

void Game::setPaused(bool paused)
{
  m_paused = paused;
}

void Game::registerAction(KeyboardKey input, const ActionName actionName)
{
  m_actionMap[input] = actionName;
}

void Game::registerActions()
{
  registerAction(KEY_W, ActionName::UP);
  registerAction(KEY_A, ActionName::LEFT);
  registerAction(KEY_S, ActionName::DOWN);
  registerAction(KEY_D, ActionName::RIGHT);
  registerAction(KEY_P, ActionName::PAUSE);
}

void Game::sDraw()
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
  DrawText(TextFormat("Elapsed Time: %02.02f ms", GetFrameTime()*1000), 0, 20, 20, RED);

  EndDrawing();
}

void Game::sTimer()
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

void Game::sLifespan()
{
  for (auto e : m_entityManager.getEntities())
  {

    if (!e->hasComponent<CLifespan>())
      continue;;
    
    auto &lifespan = e->getComponent<CLifespan>();
    if (GetTime() - lifespan.createdAt > lifespan.duration)
    {
      e->destroy();
    }
  }
}

void Game::sUserInput()
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

void Game::sMovement()
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
    auto &transform = e->getComponent<CTransform>();

    if (transform.velocity.x != 0 || transform.velocity.y != 0)
    {
      transform.position.x += transform.velocity.x;
      transform.position.y += transform.velocity.y;
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

void Game::sDoAction(const Action &action)
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

// =======================================================
// =======================================================

void Game::spawnPlayer()
{
  auto player = m_entityManager.addEntity("Player");
  Vector2 position = m_window.GetSize() / 2.0f;
  Vector2 velocity = {0.0, 0.0};

  player->addComponent<CTransform>(position, velocity);
  player->addComponent<CShape>(std::make_unique<Ring>(0.0f, 100.0f, RED));
  player->addComponent<CInput>();
  player->addComponent<CHealth>();
  player->addComponent<CTimer>(1.0, true, [this](){
    auto bullet = m_entityManager.addEntity("Bullet" + std::to_string(GetTime()));

    Vector2 target = GetMousePosition();
    Vector2 origin = m_player->getComponent<CTransform>().position;
    Vector2 velocity = Vector2Scale(Vector2Normalize(Vector2Subtract(target, origin)), SPEED);
    bullet->addComponent<CTransform>(origin, velocity);

    bullet->addComponent<CShape>(std::make_unique<Ring>(0.0f, 10.0f, WHITE));
    bullet->addComponent<CLifespan>(1.0, GetTime());
  });

  m_player = player;
}

void Game::spawnEnemies()
{
  int totalEnemies = 50;
  int numEnemies = 0;
  auto enemySpawner = m_entityManager.addEntity("EnemySpawner");
  auto spawnEnemy = [this, numEnemies, totalEnemies, enemySpawner]() mutable {
    if (numEnemies > totalEnemies)
    {
      enemySpawner->destroy();
    }

    auto enemy = m_entityManager.addEntity("Enemy" + std::to_string(numEnemies));
    enemy->addComponent<CLifespan>(10.0, GetTime());
    
    Vector2 target = m_player->getComponent<CTransform>().position;
    Vector2 origin = {0, 0};
    Vector2 velocity = Vector2Scale(Vector2Normalize(Vector2Subtract(target, origin)), SPEED);
    enemy->addComponent<CTransform>(origin, velocity);

    enemy->addComponent<CShape>(std::make_unique<Ring>(0.0f, 10.0f, BLUE));

    numEnemies += 1;
  };
  enemySpawner->addComponent<CTimer>(0.1, true, spawnEnemy);
}
