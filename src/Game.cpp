#include <memory>
#include <chrono>
#include <raylib-cpp.hpp>
#include "Constants.hpp"
#include "EntityManager.hpp"
#include "Components/Components.hpp"
#include "Systems/Systems.hpp"
#include "Action.hpp"

Game::Game()
{
  m_window = raylib::Window();
  m_window.Init(1080, 720, "Polygon Survivour");
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

  SetTargetFPS(60);

  while (isRunning())
  {
    m_entityManager.update();

    if (!m_paused)
    {
      sMovement.update();
      sTimer.update();
      sLifespan.update();

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

  player->addComponent<CTransform>(position, velocity);
  player->addComponent<CShape>(std::make_unique<Ring>(0.0f, 100.0f, RED));
  player->addComponent<CInput>();
  player->addComponent<CHealth>();
  player->addComponent<CTimer>(1.0, true, [this]()
                               {
    auto bullet = m_entityManager.addEntity("Bullet" + std::to_string(GetTime()));

    Vector2 target = GetMousePosition();
    Vector2 origin = m_player->getComponent<CTransform>().position;
    Vector2 velocity = Vector2Scale(Vector2Normalize(Vector2Subtract(target, origin)), SPEED);
    bullet->addComponent<CTransform>(origin, velocity);

    bullet->addComponent<CShape>(std::make_unique<Ring>(0.0f, 10.0f, WHITE));
    bullet->addComponent<CLifespan>(1.0, GetTime()); });

  m_player = player;
}

void Game::spawnEnemies()
{
  int totalEnemies = 50;
  int numEnemies = 0;
  auto enemySpawner = m_entityManager.addEntity("EnemySpawner");
  auto spawnEnemy = [this, numEnemies, totalEnemies, enemySpawner]() mutable
  {
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
