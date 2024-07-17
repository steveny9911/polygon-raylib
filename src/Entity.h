#include "Components.h"
#include <memory>
#include <string>

class Entity {
private:
  const size_t m_id = 0;
  const std::string m_tag = "DEFAULT";
  bool m_active = true;

public:
  std::shared_ptr<CTransform> cTransform;
  std::shared_ptr<CName> cName;
  std::shared_ptr<CShape> cShape;
  std::shared_ptr<CInput> cInput;
  std::shared_ptr<CCollision> cCollision;

  bool isActive() const { return m_active; };
  const std::string &tag() const { return m_tag; };
  const size_t id() const { return m_id; };
  void destroy() { m_active = false; };

  Entity(const std::string &tag, size_t id) : m_tag(tag), m_id(id) { };
};
