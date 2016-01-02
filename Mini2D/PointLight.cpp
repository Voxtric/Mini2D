#include "PointLight.h"

PointLight::PointLight(sf::Vector2f position, float rotation,
  sf::Color color, float scale, unsigned int resolution) :
  Light(position, color, scale, resolution)
{
  m_rotation = rotation;
}

void PointLight::setRotation(float rotation)
{
  m_rotation = rotation;
}

float PointLight::getRotation() const
{
  return m_rotation;
}