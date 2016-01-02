#pragma once

#include <SFML\Graphics.hpp>

#include "Destroyable.h"

class Light : public Destroyable
{
public:
  Light(sf::Vector2f position, sf::Color color, float scale,
    unsigned int resolution);

  sf::RenderTexture* getOccluderFBO();
  sf::RenderTexture* getShadowMapFBO();
  unsigned int getResolution() const;
  bool canRender(const sf::RenderTarget* renderTarget) const;

  void setPosition(sf::Vector2f position);
  void setColor(sf::Color color);
  void setScale(float scale);

  sf::Vector2f getPosition() const;
  sf::Color getColor() const;
  float getScale() const;
  sf::FloatRect getGlobalBounds() const;

private:
  sf::RenderTexture m_occluderFBO;
  sf::RenderTexture m_shadowMapFBO;

  sf::Vector2f m_position;
  sf::Color m_color;
  float m_scale;
};