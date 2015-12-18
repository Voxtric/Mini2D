#include "CollisionObject.h"

#include "Game.h"

CollisionObject::CollisionObject(const CollisionShape* collisionShape)
{
  m_collisionShape = (CollisionShape*)collisionShape;
  updateBoundingSphere();
  PHYSICS.addCollidable(this);
}

CollisionObject::CollisionObject(const CollisionShape* collisionShape,
  const Ticker* tickable)
{
  m_collisionShape = (CollisionShape*)collisionShape;
  updateBoundingSphere();
  m_tickable = (Ticker*)tickable;
  PHYSICS.addCollidable(this);
}

void CollisionObject::setCollisionShape(const CollisionShape* collisionShape)
{
  m_collisionShape = (CollisionShape*)collisionShape;
  updateBoundingSphere();
}

void CollisionObject::updateBoundingSphere()
{
  sf::Vector2f totalPosition = sf::Vector2f(0.0f, 0.0f);
  for (unsigned int i = 0; i < m_collisionShape->getPointCount(); ++i)
  {
  totalPosition += m_collisionShape->getPoint(i);
  }
  float count = (float)m_collisionShape->getPointCount();
  m_boundingSphere.localCentre = totalPosition / count;

  sf::Vector2f pointA = m_boundingSphere.localCentre;
  sf::Vector2f pointB = m_collisionShape->getPoint(0);
  float x = (pointB.x - pointA.x) * (pointB.x - pointA.x);
  float y = (pointB.y - pointA.y) * (pointB.y - pointA.y);
  m_boundingSphere.radius = sqrtf(x + y);
}

Ticker* CollisionObject::getAttachedTickable() const
{
  return m_tickable;
}

CollisionShape* CollisionObject::getCollisionShape() const
{
  return m_collisionShape;
}

BoundingSphere CollisionObject::getBoundingSphere() const
{
  return m_boundingSphere;
}