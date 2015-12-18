#include "CollisionShape.h"

#include <iostream>

#include "Game.h"

CollisionShape::CollisionShape(const Renderer* renderer)
{
  sf::IntRect rect = renderer->getTextureRect();
  sf::Vector2f topLeft = sf::Vector2f((float)rect.left, (float)rect.top);

  sf::Vector2f topRight = topLeft;
  topRight.x += rect.width;

  sf::Vector2f bottomRight = topRight;
  bottomRight.y += rect.height;

  sf::Vector2f bottomLeft = bottomRight;
  bottomLeft.x -= rect.width;

  setPointCount(4);
  setPoint(0, topLeft);
  setPoint(1, topRight);
  setPoint(2, bottomRight);
  setPoint(3, bottomLeft);
  setTransformable(renderer);
}

CollisionShape::CollisionShape(const sf::Transformable* transformable, 
  unsigned int pointCount)
{
  setTransformable(transformable);
  setPointCount(pointCount);
}

void CollisionShape::setTransformable(const sf::Transformable* transformable)
{
  m_transformable = (sf::Transformable*)transformable;
}

const sf::Transform* CollisionShape::getTransform() const
{
  return &m_transformable->getTransform();
}

void CollisionShape::setPointCount(unsigned int pointCount)
{
  m_points.resize(pointCount);
}

unsigned int CollisionShape::getPointCount() const
{
  return m_points.size();
}

void CollisionShape::setPoint(unsigned int index, sf::Vector2f point)
{
  m_points[index] = point;
}

sf::Vector2f CollisionShape::getPoint(unsigned int index) const
{
  return m_points[index];
}