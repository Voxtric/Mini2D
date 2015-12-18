#pragma once

#include <SFML\Graphics.hpp>

#include "HitInfo.hpp"

#define PHYSICS Game::getActiveGame().getPhysicsManager()

class PhysicsManager
{
public:
  void addCollidable(const CollisionObject* collider);
  void removeCollidable(const CollisionObject* collider);

  bool rayCast(sf::Vector2f startPosition, sf::Vector2f direction, 
  float distance, HitInfo& hitInfo);
  bool rayCast(sf::Vector2f startPosition, sf::Vector2f endPosition,
  HitInfo& hitInfo);
  bool lineCast(sf::Vector2f startPostion, sf::Vector2f endPosition,
  std::vector<HitInfo>& hitInfo);
  bool circleCast(sf::Vector2f centre, float radius, 
  std::vector<HitInfo>& hitInfo);   

private:
  std::vector<CollisionObject*> m_collisionObjects;

  bool lineIntersectsLine(sf::Vector2f lineAStart, sf::Vector2f lineAEnd,
  sf::Vector2f lineBStart, sf::Vector2f lineBEnd, 
  sf::Vector2f& intersection) const;
  bool lineIntersectsCircle(sf::Vector2f lineStart, sf::Vector2f lineEnd,
  sf::Vector2f circleCentre, float circleRadius) const;
  bool pointInCircle(sf::Vector2f point, sf::Vector2f circleCentre, 
  float radius) const;

  sf::Vector2f closestPointOnLine(sf::Vector2f lineStart, 
  sf::Vector2f lineEnd, sf::Vector2f point);

  float dot(sf::Vector2f pointA, sf::Vector2f pointB) const;
  float perpDot(sf::Vector2f pointA, sf::Vector2f pointB) const;
  float calculateDistance(sf::Vector2f pointA, sf::Vector2f pointB) const;
  float calculateDistanceSquared(sf::Vector2f pointA, 
  sf::Vector2f pointB) const;
};    
