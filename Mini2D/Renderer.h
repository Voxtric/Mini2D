#pragma once

#include <SFML\Graphics.hpp>

#include "Destroyable.h"

///<summary>Extension of the SFML Sprite class</summary>
class Renderer : public sf::Sprite, public Destroyable
{
public:
  Renderer(const std::string& spritePath);
  Renderer(sf::Texture& spriteTexture);

  bool canRender(const sf::RenderTarget* renderTarget) const;
  void render(sf::RenderTarget* window);
  void render(sf::RenderTarget* window, const sf::RenderStates& renderStates);
};