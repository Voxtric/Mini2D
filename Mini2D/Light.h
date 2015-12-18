#pragma once

#include <SFML\Graphics.hpp>

class Light
{
public:
  Light(sf::Vector2f position, sf::Color color, unsigned int size);

  sf::RenderTexture* getOccluderFBO();
  sf::Vector2f getPosition() const;
  sf::Color getColor() const;
  unsigned int getSize() const;

private:
  sf::RenderTexture m_occluderFBO;
  sf::Vector2f m_position;
  sf::Color m_color;
  unsigned int m_size;
};