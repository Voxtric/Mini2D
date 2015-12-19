#include "Light.h"

Light::Light(sf::Vector2f position, sf::Color color, float scale,
  unsigned int size)
{
  m_position = position;
  m_color = color;
  m_scale = 1.0f;
  m_occluderFBO.create(size, size, false);
  m_shadowMapFBO.create(size, 1, false);
}

sf::RenderTexture* Light::getOccluderFBO()
{
  return &m_occluderFBO;
}

sf::RenderTexture* Light::getShadowMapFBO()
{
  return &m_shadowMapFBO;
}

sf::Vector2f Light::getPosition() const
{
  return m_position;
}

sf::Color Light::getColor() const
{
  return m_color;
}

float Light::getScale() const
{
  return m_scale;
}

unsigned int Light::getSize() const
{
  return m_occluderFBO.getSize().x; //x or y will do.
}