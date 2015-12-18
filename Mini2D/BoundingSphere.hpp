#pragma once

#include <SFML\System\Vector2.hpp>

struct BoundingSphere
{
public:
  sf::Vector2f localCentre;
  float radius = 0.0f;

  BoundingSphere() {}

  BoundingSphere(sf::Vector2f localCentre, float radius)
  {
    this->localCentre = localCentre;
    this->radius = radius;
  }
};