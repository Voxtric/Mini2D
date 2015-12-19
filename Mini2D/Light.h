#pragma once

#include <SFML\Graphics.hpp>

class Light
{
public:
  Light(sf::Vector2f position, sf::Color color, float scale, unsigned int size);

  sf::RenderTexture* getOccluderFBO();
  sf::RenderTexture* getShadowMapFBO();
  sf::Vector2f getPosition() const;
  sf::Color getColor() const;
  float getScale() const;
  unsigned int getSize() const;

private:
  sf::RenderTexture m_occluderFBO;
  sf::RenderTexture m_shadowMapFBO;

  sf::Vector2f m_position;
  sf::Color m_color;
  float m_scale;
  unsigned int m_size;
};