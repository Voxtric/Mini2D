#include "Light.h"

#include "Game.h"

Light::Light(const std::string& lightMask, sf::Color color) :
  Renderer("Sprites/Lights/" + lightMask + ".png")
{
  m_color = color;
  m_lightMap = new sf::RenderTexture();
  m_lightMap->create(GAME.getWindow()->getSize().x, GAME.getWindow()->getSize().y);

  if (lightMask == "sphere_mask" || lightMask == "dual_cone_mask")
  {
    setOrigin(SPHERE_MASK_ORIGIN);
  }
  else if (lightMask == "cone_mask")
  {
    setOrigin(22.0f, 128.0f);
  }
}

void Light::setColor(sf::Color color)
{
  m_color = color;
}

sf::Color Light::getColor() const
{
  return m_color;
}

sf::RenderTexture* Light::getLightMap() const
{
  return m_lightMap;
}

void Light::refreshLightMap()
{
  if (m_lightMap->getSize() != Game::getActiveGame().getWindow()->getSize())
  {
    m_lightMap->create(GAME.getWindow()->getSize().x, GAME.getWindow()->getSize().y);
  }
}