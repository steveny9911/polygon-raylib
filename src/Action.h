#include <string>

enum class ActionType
{
  NONE,
  START,
  END
};
enum class ActionName
{
  NONE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  PAUSE
};

class Action
{
private:
  const ActionName m_name = ActionName::NONE;
  const ActionType m_type = ActionType::NONE;

public:
  Action() {};
  Action(const ActionName name, const ActionType type) : m_name(name), m_type(type) {}

  const ActionName name() const { return m_name; };
  ActionType type() const { return m_type; };
};
