#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
  PointLight(sf::Vector2f position, float rotation, sf::Color color, float scale,
    unsigned int resolution);

  void setRotation(float rotation);
  float getRotation() const;

private:
  float m_rotation = 0.0f;
};