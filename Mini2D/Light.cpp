#include "Light.h"

Light::Light(sf::Vector2f position, sf::Color color, unsigned int size)
{
  m_position = position;
  m_color = color;
  m_size = size;
}

sf::RenderTexture* Light::getOccluderFBO()
{
  return &m_occluderFBO;
}

sf::Vector2f Light::getPosition() const
{
  return m_position;
}

sf::Color Light::getColor() const
{
  return m_color;
}

unsigned int Light::getSize() const
{
  return m_size;
}