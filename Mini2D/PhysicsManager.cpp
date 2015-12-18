#include "PhysicsManager.h"

#include <iostream>

void PhysicsManager::addCollidable(const CollisionObject* collisionObject)
{
  m_collisionObjects.push_back((CollisionObject*)collisionObject);
}

void PhysicsManager::removeCollidable(const CollisionObject* collisionObject)
{
  for (unsigned int i = 0; i < m_collisionObjects.size(); ++i)
  {
    if (m_collisionObjects[i] == collisionObject)
    {
      m_collisionObjects[i] = m_collisionObjects.back();
      m_collisionObjects.pop_back();
      break;
    }
  }
}

bool PhysicsManager::rayCast(sf::Vector2f startPosition, sf::Vector2f direction,
  float distance, HitInfo& hitInfo)
{
  sf::Vector2f endPosition = startPosition + (direction * distance);
  return rayCast(startPosition, endPosition, hitInfo);
}

bool PhysicsManager::rayCast(sf::Vector2f startPosition, 
  sf::Vector2f endPosition, HitInfo& hitInfo)
{
  std::vector<HitInfo> allHits;
  bool hasHit = lineCast(startPosition, endPosition, allHits);
  if (hasHit)
  {
    hitInfo.hitDistanceFromOrigin = (float)INT_MAX;
    for (unsigned int i = 0; i < allHits.size(); ++i)
    {
      if (allHits[i].hitDistanceFromOrigin < 
        hitInfo.hitDistanceFromOrigin)
      {
        hitInfo = allHits[i];
      }
    }
  }
  return hasHit;
}

bool PhysicsManager::lineCast(sf::Vector2f startPosition, 
  sf::Vector2f endPosition, std::vector<HitInfo>& hitInfo)
{
  //Find all colliders that could be along the line
  for (unsigned int i = 0; i < m_collisionObjects.size(); ++i)
  {
    CollisionObject* collisionObject = m_collisionObjects[i];
    CollisionShape* collisionShape = collisionObject->getCollisionShape();
    sf::Transform transform = *collisionShape->getTransform();
    BoundingSphere boundingSphere = collisionObject->getBoundingSphere();
    sf::Vector2f worldCentre = transform * boundingSphere.localCentre;

    if (lineIntersectsCircle(startPosition, endPosition, worldCentre, 
      boundingSphere.radius))
    {
      unsigned int pointCount = collisionShape->getPointCount();
      for (unsigned int j = 0; j < pointCount; ++j)
      {
        unsigned int j2 = (j + 1) % pointCount;
        sf::Vector2f colliderLineStart;
        colliderLineStart = transform * collisionShape->getPoint(j);
        sf::Vector2f colliderLineEnd;
        colliderLineEnd = transform * collisionShape->getPoint(j2);

        //All info needed is here by this point.
        sf::Vector2f intersection;
        if (lineIntersectsLine(startPosition, endPosition,
          colliderLineStart, colliderLineEnd, intersection))
        {
          //Calculate distance from start point to the hit point.
          //sqrt((x1 - x2)^2 + (y1 - y2)^2)
          float x = startPosition.x - intersection.x;
          x *= x;
          float y = startPosition.y - intersection.y;
          y *= y;
          float hitDistanceFromOrigin = sqrtf(x + y);

          //Only add it to the list of hit points if it is the closest
          //hit.
          if (hitInfo.size() == 0 ||
            hitInfo.back().collider != collisionObject)
          {
            hitInfo.emplace_back(intersection,
            collisionObject, hitDistanceFromOrigin);
          }
          else if (hitInfo.back().hitDistanceFromOrigin >
            hitDistanceFromOrigin)
          {
            HitInfo* hit = &hitInfo.back();
            hit->hitPoint = intersection;
            hit->hitDistanceFromOrigin = hitDistanceFromOrigin;
          }
        }
      }
    }
  }

  return hitInfo.size() > 0;
}

bool PhysicsManager::circleCast(sf::Vector2f centre, float radius,
  std::vector<HitInfo>& hitInfo)
{
  for (unsigned int i = 0; i < m_collisionObjects.size(); ++i)
  {
    CollisionObject* collisionObject = m_collisionObjects[i];
    CollisionShape* collisionShape = collisionObject->getCollisionShape();
    sf::Transform transform = *collisionShape->getTransform();
    BoundingSphere boundingSphere = collisionObject->getBoundingSphere();
    sf::Vector2f worldCentre = transform * boundingSphere.localCentre;

    float maxDistanceSquared = radius + boundingSphere.radius;
    maxDistanceSquared *= maxDistanceSquared;
    float x = centre.x - worldCentre.x;
    x *= x;
    float y = centre.y - worldCentre.y;
    y *= y;
    float actualDistanceSquared = x + y;

    if (actualDistanceSquared < maxDistanceSquared)
    {
      for (unsigned int j = 0; j < collisionShape->getPointCount(); ++j)
      {
        unsigned int j2 = (j + 1) % collisionShape->getPointCount();
        sf::Vector2f pointA = transform * collisionShape->getPoint(j);
        sf::Vector2f pointB = transform * collisionShape->getPoint(j2);
        if (lineIntersectsCircle(pointA, pointB, centre, radius))
        {
          sf::Vector2f hitPoint;
          hitPoint = closestPointOnLine(pointA, pointB, centre);
          float distance = calculateDistance(centre, hitPoint);
          if (hitInfo.size() == 0 ||
            hitInfo.back().collider != collisionObject)
          {
            hitInfo.emplace_back(hitPoint,
            collisionObject, distance);
          }
          else if (hitInfo.back().hitDistanceFromOrigin >
            distance)
          {
            HitInfo* hit = &hitInfo.back();
            hit->hitPoint = hitPoint;
            hit->hitDistanceFromOrigin = distance;
          }
        }
      }
    }
  }
  return hitInfo.size() > 0;
}

bool PhysicsManager::lineIntersectsLine(sf::Vector2f lineAStart, 
  sf::Vector2f lineAEnd,  sf::Vector2f lineBStart, sf::Vector2f lineBEnd,
  sf::Vector2f& intersection) const
{
  bool lineIntersects = true;
  sf::Vector2f lineADifference = lineAEnd - lineAStart;
  sf::Vector2f lineBDifference = lineBEnd - lineBStart;
  float f = perpDot(lineADifference, lineBDifference);
  if (f != 0.0f)
  {
    sf::Vector2f c = lineBEnd - lineAEnd;
    float aa = perpDot(lineADifference, c);
    float bb = perpDot(lineBDifference, c);

    if (f < 0.0f)
    {
      lineIntersects = !(aa > 0.0f || bb > 0.0f || aa < f || bb < f);
    }
    else
    {
      lineIntersects = !(aa < 0.0f || bb < 0.0f || aa > f || bb > f);
    }

    if (lineIntersects)
    {
      //Calculate the actual intersection.
      float out = 1.0f - (aa / f);
      intersection = (lineBDifference * out) + lineBStart;
    }
  }
  return lineIntersects;
}

bool PhysicsManager::lineIntersectsCircle(sf::Vector2f lineStart,
  sf::Vector2f lineEnd, sf::Vector2f circleCentre, float circleRadius) const
{ 
  sf::Vector2f d = lineEnd - lineStart;
  sf::Vector2f f = lineStart - circleCentre;

  float a = dot(d, d);
  float b = dot(f * 2.0f, d);
  float c = dot(f, f) - (circleRadius * circleRadius);

  bool intersects = false;
  float discriminant = (b * b) - (4 * a * c);
  if (discriminant >= 0.0f)
  {
    discriminant = sqrtf(discriminant);
    float t1 = (-b - discriminant) / (2 * a);
    float t2 = (-b + discriminant) / (2 * a);
    if ((t1 >= 0.0f && t1 <= 1.0f) || (t2 >= 0.0f && t2 <= 1.0f))
    {
      intersects = true;
    }
  }
  return intersects;
}

bool PhysicsManager::pointInCircle(sf::Vector2f point,
  sf::Vector2f circleCentre, float radius) const
{
  //Calculate distance from start point to the hit point.
  //sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))
  float x = point.x - circleCentre.x;
  x *= x;
  float y = point.y - circleCentre.y;
  y *= y;
  float distance = sqrtf(x + y);
  return distance < radius;
}

sf::Vector2f PhysicsManager::closestPointOnLine(sf::Vector2f lineStart,
  sf::Vector2f lineEnd, sf::Vector2f point)
{
  float APx = point.x - lineStart.x;
  float APy = point.y - lineStart.y;
  float ABx = lineEnd.x - lineStart.x;
  float ABy = lineEnd.y - lineStart.y;
  float magAB2 = (ABx * ABx) + (ABy * ABy);
  float ABdotAP = (ABx * APx) + (ABy * APy);
  float t = ABdotAP / magAB2;

  sf::Vector2f returnValue;
  if (t < 0.0f)
  {
    returnValue = lineStart;
  }
  else if (t > 1.0f)
  {
    returnValue = lineEnd;
  }
  else
  {
    returnValue = lineStart + sf::Vector2f(ABx * t, ABy * t);
  }
  return returnValue;
}

float PhysicsManager::dot(sf::Vector2f pointA, sf::Vector2f pointB) const
{
  return (pointA.x * pointB.x) + (pointA.y * pointB.y);
}

float PhysicsManager::perpDot(sf::Vector2f pointA, sf::Vector2f pointB) const
{
  return (pointA.y * pointB.x) - (pointA.x * pointB.y);
}

float PhysicsManager::calculateDistance(sf::Vector2f pointA, sf::Vector2f pointB) const
{
  return sqrtf(calculateDistanceSquared(pointA, pointB));
}

float PhysicsManager::calculateDistanceSquared(sf::Vector2f pointA, sf::Vector2f pointB) const
{
  float x = pointA.x - pointB.x;
  x *= x;
  float y = pointA.y - pointB.y;
  y *= y;
  return x + y;
}