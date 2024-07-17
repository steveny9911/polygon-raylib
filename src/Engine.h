#include <map>
#include <memory>
#include <raylib-cpp.hpp>
#include "EntityManager.h"

float SPEED = 10.0f;

class Engine
{
private:
  raylib::Window m_window;

  EntityManager entityManager;

  size_t m_currentFrame = 0;
  bool m_paused = false;
  bool m_running = true;

  bool isRunning();
  void setPaused(bool paused);

  void sMovement();
  void sUserInput();
  void sDraw();
  void sCollision();

  void sEnemySpawner();

  std::shared_ptr<Entity> m_player;
  void spawnPlayer();
  void spawnEnemy();
  void spawnBullet(std::shared_ptr<Entity> entity, const Vector2 &mousePos);

public:
  raylib::Window &window();

  void run();
  void update();
  void draw();
  void quit();

  Engine()
  {
    m_window = raylib::Window();
    m_window.Init(1080, 720, "Polygon Survivour");
    SetTargetFPS(120);

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
    entityManager.update();

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

void Engine::spawnPlayer()
{
  auto player = entityManager.addEntity("Player");
  Vector2 position = m_window.GetSize() / 2.0f;
  Vector2 velocity = {0.0, 0.0};

  player->cTransform = std::make_shared<CTransform>(position, velocity, 0.0);
  player->cShape = std::make_shared<ShapeRing>(80.0f, 190.0f, RED);
  player->cInput = std::make_shared<CInput>();

  m_player = player;
}

void Engine::sDraw()
{
  BeginDrawing();
  ClearBackground(BLACK);

  for (auto e : entityManager.getEntities())
  {
    if (e->cShape)
    {
      e->cShape->draw(e->cTransform->position);
    }
  }

  EndDrawing();
}

void Engine::sUserInput()
{
  m_player->cInput->up = IsKeyDown(KEY_W);
  m_player->cInput->down = IsKeyDown(KEY_S);
  m_player->cInput->left = IsKeyDown(KEY_A);
  m_player->cInput->right = IsKeyDown(KEY_D);

  m_paused = IsKeyPressed(KEY_P);
}

void Engine::sMovement()
{
  m_player->cTransform->velocity = Vector2Zero();

  // 8 directions total
  // up only
  if (m_player->cInput->up && !m_player->cInput->down && !m_player->cInput->left && !m_player->cInput->right)
    m_player->cTransform->velocity.y = -SPEED;

  // down only
  else if (m_player->cInput->down && !m_player->cInput->up && !m_player->cInput->left && !m_player->cInput->right)
    m_player->cTransform->velocity.y = SPEED;

  // left
  else if (m_player->cInput->left && !m_player->cInput->right && !m_player->cInput->up && !m_player->cInput->down)
    m_player->cTransform->velocity.x = -SPEED;

  // right
  else if (!m_player->cInput->left && m_player->cInput->right && !m_player->cInput->up && !m_player->cInput->down)
    m_player->cTransform->velocity.x = SPEED;

  // up right
  else if (!m_player->cInput->left && m_player->cInput->right && m_player->cInput->up && !m_player->cInput->down)
    m_player->cTransform->velocity = Vector2{SPEED * cosf(PI / 4.0f), -SPEED * sinf(PI / 4.0f)};

  // up left
  else if (m_player->cInput->left && !m_player->cInput->right && m_player->cInput->up && !m_player->cInput->down)
    m_player->cTransform->velocity = Vector2{-SPEED * cosf(PI / 4.0f), -SPEED * sinf(PI / 4.0f)};

  // down right
  else if (!m_player->cInput->left && m_player->cInput->right && !m_player->cInput->up && m_player->cInput->down)
    m_player->cTransform->velocity = Vector2{SPEED * cosf(PI / 4.0f), SPEED * sinf(PI / 4.0f)};

  // down left
  else if (m_player->cInput->left && !m_player->cInput->right && !m_player->cInput->up && m_player->cInput->down)
    m_player->cTransform->velocity = Vector2{-SPEED * cosf(PI / 4.0f), SPEED * sinf(PI / 4.0f)};

  for (auto e : entityManager.getEntities())
  {
    // Sample movement speed update
    e->cTransform->position.x += e->cTransform->velocity.x;
    e->cTransform->position.y += e->cTransform->velocity.y;
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
