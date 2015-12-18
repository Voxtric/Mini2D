#pragma once

#include "Renderer.h"

class Light : public Renderer
{
public:
  Light(const std::string& lightMask, sf::Color color);
  
  void setColor(sf::Color color);
  sf::Color getColor() const;
  sf::RenderTexture* getLightMap() const;
  void refreshLightMap();

private:
  const sf::Vector2f SPHERE_MASK_ORIGIN = sf::Vector2f(256.0f, 256.0f);
  const sf::Vector2f CONE_MASK_ORIGIN = sf::Vector2f(22.0f, 128.0f);

  sf::Color m_color = sf::Color::White;
  sf::RenderTexture* m_lightMap = nullptr;
};