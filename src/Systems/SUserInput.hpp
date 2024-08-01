#include "System.hpp"
#include "../Action.hpp"
#include "../Game.h"

class SUserInput : public System
{
private:
  ActionMap &m_actionMap;
  std::shared_ptr<Entity> m_player;
  Game *game;

  void doAction(const Action &action)
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
        game->setPaused(!game->isPaused());
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

public:
  SUserInput(EntityManager &manager, Game *game, ActionMap &actionMap, std::shared_ptr<Entity> player)
      : System(manager), game(game), m_actionMap(actionMap), m_player(player) {};

  void update() override
  {
    KeyboardKey input = KEY_NULL;
    while (input = static_cast<KeyboardKey>(GetKeyPressed()))
    {
      if (!m_actionMap.contains(input))
        return;

      doAction(Action(m_actionMap.at(input), ActionType::START));
    }

    for (auto &[key, _] : m_actionMap)
    {
      if (IsKeyUp(key))
        doAction(Action(m_actionMap.at(key), ActionType::END));
    }
  }
};
