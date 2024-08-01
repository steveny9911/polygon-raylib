#include "System.hpp"
#include "../Constants.hpp"

class SMovement : public System
{
private:
  std::shared_ptr<Entity> m_player;

public:
  SMovement(EntityManager &manager, std::shared_ptr<Entity> player) : System(manager), m_player(player) {};

  void update() override
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
};
