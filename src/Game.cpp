#include <memory>
#include <chrono>
#include <raylib-cpp.hpp>
#include "Constants.hpp"
#include "EntityManager.hpp"
#include "Components/Components.h"
#include "Systems/Systems.hpp"
#include "Action.hpp"

Game::Game()
{
  m_window = raylib::Window();
  m_window.Init(800, 450, "Polygon Survivour");
  SetTargetFPS(120);

  registerActions();
  spawnPlayer();
  spawnEnemies();
}

bool Game::isRunning()
{
  return m_running & !m_window.ShouldClose();
}

bool Game::isPaused()
{
  return m_paused;
}

void Game::run()
{
  SLifespan sLifespan(m_entityManager);
  STimer sTimer(m_entityManager);
  SMovement sMovement(m_entityManager, m_player);
  SUserInput sUserInput(m_entityManager, this, m_actionMap, m_player);
  SDraw sDraw(m_entityManager);
  SCollision sCollision(m_entityManager);

  SetTargetFPS(60);

  while (isRunning())
  {
    m_entityManager.update();

    if (!m_paused)
    {
      sMovement.update();
      sTimer.update();
      sLifespan.update();
      sCollision.update();

      m_currentFrame++;
    }

    sUserInput.update();
    sDraw.update();
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

// =======================================================
// =======================================================

void Game::spawnPlayer()
{
  auto player = m_entityManager.addEntity("Player");
  Vector2 position = m_window.GetSize() / 2.0f;
  Vector2 velocity = {0.0, 0.0};

  float radius = 20.0f;

  player->addComponent<CTransform>(position, velocity);
  player->addComponent<CShape>(std::make_unique<ShapeRing>(0.0f, radius, RED));
  player->addComponent<CInput>();
  player->addComponent<CHealth>();
  std::function<void()> spawnBullet = [this]()
  {
    float bulletRadius = 10.0f;
    auto bullet = m_entityManager.addEntity("Bullet" + std::to_string(GetTime()));

    Vector2 target = GetMousePosition();
    Vector2 origin = m_player->getComponent<CTransform>().position;
    Vector2 velocity = Vector2Scale(Vector2Normalize(Vector2Subtract(target, origin)), SPEED);
    bullet->addComponent<CTransform>(origin, velocity);

    bullet->addComponent<CShape>(std::make_unique<ShapeRing>(0.0f, bulletRadius, WHITE));
    bullet->addComponent<CLifespan>(20.0, GetTime());

    std::function<void(std::shared_ptr<Entity>)> bulletCollision = [bullet](std::shared_ptr<Entity> other)
    {
      if (other->tag() == "Enemy")
      {
        other->destroy();
        bullet->destroy();
      }
    };
    bullet->addComponent<CCollision>(std::make_unique<ColliderCircle>(bulletRadius), bulletCollision);
  };

  player->addComponent<CTimer>(1.0, true, spawnBullet);
  std::function<void(std::shared_ptr<Entity>)> playerCollision = [this](std::shared_ptr<Entity> other)
  {
    if (other->tag() == "Enemy")
    {
      this->setPaused(true);
    }
  };
  player->addComponent<CCollision>(std::make_unique<ColliderCircle>(radius), playerCollision);

  m_player = player;
}

void Game::spawnEnemies()
{
  enum Direction
  {
    UP,
    DOWN,
    LEFT,
    RIGHT
  };

  int totalEnemies = 100;
  int numEnemies = 0;
  float radius = 20.0f;

  const float stepSize = GetScreenHeight() / 10;
  Direction direction = Direction::RIGHT;
  Vector2 spawnOrigin{0.0f, 0.0f};

  auto enemySpawner = m_entityManager.addEntity("EnemySpawner");
  std::function<void()> spawnEnemy = [=]() mutable
  {
    if (numEnemies > totalEnemies)
    {
      enemySpawner->destroy();
    }

    auto enemy = m_entityManager.addEntity("Enemy");
    enemy->addComponent<CLifespan>(10.0, GetTime());

    switch (direction)
    {
    case Direction::UP:
      if (spawnOrigin.y - stepSize >= 0.0f)
      {
        spawnOrigin.y -= stepSize;
      }
      else
      {
        spawnOrigin.y = 0.0f;
        direction = RIGHT;
      }
      break;
    case Direction::DOWN:
      if (spawnOrigin.y + stepSize <= GetScreenHeight())
      {
        spawnOrigin.y += stepSize;
      }
      else
      {
        spawnOrigin.y = GetScreenHeight();
        direction = LEFT;
      }
      break;
    case Direction::LEFT:
      if (spawnOrigin.x - stepSize >= 0.0f)
      {
        spawnOrigin.x -= stepSize;
      }
      else
      {
        spawnOrigin.x = 0.0f;
        direction = UP;
      }
      break;
    case Direction::RIGHT:
      if (spawnOrigin.x + stepSize <= GetScreenWidth())
      {
        spawnOrigin.x += stepSize;
      }
      else
      {
        spawnOrigin.x = GetScreenWidth();
        direction = DOWN;
      }
      break;
    default:
      break;
    }

    Vector2 target = m_player->getComponent<CTransform>().position;
    Vector2 velocity = Vector2Scale(Vector2Normalize(Vector2Subtract(target, spawnOrigin)), SPEED / 3);
    enemy->addComponent<CTransform>(spawnOrigin, velocity);

    enemy->addComponent<CShape>(std::make_unique<ShapeRing>(0.0f, 20.0f, BLUE));
    enemy->addComponent<CCollision>(std::make_unique<ColliderCircle>(radius), [](std::shared_ptr<Entity> other) {});

    numEnemies += 1;
  };
  enemySpawner->addComponent<CTimer>(0.3f, true, spawnEnemy);
}
