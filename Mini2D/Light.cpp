#include "Light.h"

Light::Light(sf::Vector2f position, sf::Color color, float scale,
  unsigned int resolution)
{
  m_position = position;
  m_color = color;
  m_scale = scale;
  m_occluderFBO.create(resolution, resolution, false);
  m_shadowMapFBO.create(resolution, 1, false);
}

sf::RenderTexture* Light::getOccluderFBO()
{
  return &m_occluderFBO;
}

sf::RenderTexture* Light::getShadowMapFBO()
{
  return &m_shadowMapFBO;
}

unsigned int Light::getResolution() const
{
  return m_occluderFBO.getSize().x; //x or y will do.
}

void Light::setPosition(sf::Vector2f position)
{
  m_position = position;
}

void Light::setColor(sf::Color color)
{
  m_color = color;
}

void Light::setScale(float scale)
{
  m_scale = scale;
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