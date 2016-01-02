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

bool Light::canRender(const sf::RenderTarget* renderTarget) const
{
  bool canRender = false;
  sf::View view = renderTarget->getView();
  sf::Vector2f size = view.getSize();
  sf::Vector2f position = view.getCenter() - (size / 2.0f);
  sf::FloatRect viewRect(position, size);
  canRender = viewRect.intersects(getGlobalBounds());
  return canRender;
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

sf::FloatRect Light::getGlobalBounds() const
{
  sf::FloatRect rect;
  float size = (float)getResolution() * m_scale;
  rect.top = m_position.y - (size / 2.0f);
  rect.left = m_position.x - (size / 2.0f);
  rect.height = size;
  rect.width = size;
  return rect;
}