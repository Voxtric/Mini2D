#pragma once

#include <vector>
#include <SFML\Graphics.hpp>

#include "Renderer.h"

class CollisionShape
{
public:
  CollisionShape(const Renderer* renderer);
  CollisionShape(const sf::Transformable* transformable, unsigned int pointCount);

  void setTransformable(const sf::Transformable* transformable);
  const sf::Transform* getTransform() const;
  void setPointCount(unsigned int pointCount);
  unsigned int getPointCount() const;
  void setPoint(unsigned int index, sf::Vector2f point);
  sf::Vector2f getPoint(unsigned int index) const;

private:
  std::vector<sf::Vector2f> m_points;
  sf::Transformable* m_transformable = nullptr;
};