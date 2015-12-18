#pragma once

#include <SFML\System\Vector2.hpp>

#include "CollisionObject.h"

struct HitInfo
{
public:
  sf::Vector2f hitPoint;
  CollisionObject* collider = nullptr;
  float hitDistanceFromOrigin = 0.0f;

  HitInfo() {}

  HitInfo(sf::Vector2f hitPoint, CollisionObject* collider, float hitDistanceFromOrigin)
  {
  this->hitPoint = hitPoint;
  this->collider = collider;
  this->hitDistanceFromOrigin = hitDistanceFromOrigin;
  }
};