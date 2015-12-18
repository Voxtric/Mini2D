#pragma once

#include "CollisionShape.h"
#include "Ticker.h"
#include "BoundingSphere.hpp"

class CollisionObject
{
public:
  CollisionObject(const CollisionShape* collisionShape);
  CollisionObject(const CollisionShape* collisionShape, const Ticker* tickable);
  void setCollisionShape(const CollisionShape* collisionShape);
  void updateBoundingSphere();

  Ticker* getAttachedTickable() const;
  CollisionShape* getCollisionShape() const;  
  BoundingSphere getBoundingSphere() const;

private:
  Ticker* m_tickable = nullptr;
  CollisionShape* m_collisionShape = nullptr;
  BoundingSphere m_boundingSphere;
};