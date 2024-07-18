#include "Components.h"
#include <memory>
#include <string>
#include <tuple>

typedef std::tuple<
    CTransform,
    CName,
    CInput,
    CShape,
    CCollision>
    ComponentTuple;

class Entity
{
private:
  const size_t m_id = 0;
  const std::string m_tag = "DEFAULT";

  bool m_active = true;
  ComponentTuple m_components;

public:
  std::shared_ptr<CName> cName;
  std::shared_ptr<CInput> cInput;
  std::shared_ptr<CCollision> cCollision;

  Entity(const std::string &tag, size_t id) : m_tag(tag), m_id(id) {}

  bool isActive() const { return m_active; }
  const std::string &tag() const { return m_tag; }
  const size_t id() const { return m_id; }
  void destroy() { m_active = false; }

  template <typename T>
  T &getComponent() { return std::get<T>(m_components); }

  template <typename T>
  const T &getComponent() const { return std::get<T>(m_components); }

  template <typename T>
  bool hasComponent() const { return getComponent<T>().has; }

  template <typename T, typename... TArgs>
  T &addComponent(TArgs &&...mArgs)
  {
    auto &component = getComponent<T>();
    component = T(std::forward<TArgs>(mArgs)...);
    component.has = true;
    return component;
  }

  template <typename T>
  void removeComponent() { getComponent<T>() = T(); }
};
