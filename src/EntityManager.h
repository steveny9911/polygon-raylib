#include "Entity.h"
#include <map>
#include <memory>
#include <utility>
#include <vector>

typedef std::vector<std::shared_ptr<Entity>> EntityVector;
typedef std::map<std::string, EntityVector> EntityMap;

class EntityManager
{
private:
  EntityVector m_entities;
  EntityVector m_entitiesToAdd;
  EntityMap m_entityMap;
  size_t m_totalEntities = 0; // total entity *EVER* created; unique IDs

  void removeDeadEntities(EntityVector &vec);

public:
  EntityManager() {};
  void update();

  std::shared_ptr<Entity> addEntity(const std::string &tag);

  EntityVector &getEntities();
  EntityVector &getEntities(const std::string &tag);
};

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
{
  std::shared_ptr<Entity> e = std::make_shared<Entity>(tag, m_totalEntities++);
  m_entitiesToAdd.push_back(e);

  return e;
}

EntityVector &EntityManager::getEntities() {
  return m_entities;
}

EntityVector &EntityManager::getEntities(const std::string &tag)
{
  return m_entityMap[tag];
}

void EntityManager::update()
{
  for (auto e : m_entitiesToAdd)
  {
    m_entities.push_back(e);
    m_entityMap[e->tag()].push_back(e);
  }

  removeDeadEntities(m_entities);

  for (auto &[tag, entityVec] : m_entityMap)
  {
    removeDeadEntities(entityVec);
  }

  m_entitiesToAdd.clear();
}

void EntityManager::removeDeadEntities(EntityVector &vec)
{
  std::erase_if(m_entities, [](const std::shared_ptr<Entity> &entity)
                { return !entity->isActive(); });
}
