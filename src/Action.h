#include <string>

class Action
{
private:
  const std::string m_name = "DEFAULT_NAME";
  const std::string m_type = "DEFAULT_TYPE";

public:
  Action() {};
  Action(const std::string &name, const std::string &type) : m_name(name), m_type(type) {}

  const std::string &name() const { return m_name; };
  const std::string &type() const { return m_type; };
};
