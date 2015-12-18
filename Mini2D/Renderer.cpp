#include "Renderer.h"

#include "Game.h"

Renderer::Renderer(const std::string& spritePath)
{
  setTexture(IMAGE.getTexture(spritePath));
}

Renderer::Renderer(sf::Texture& spriteTexture)
{
  setTexture(spriteTexture);
}

bool Renderer::canRender(const sf::RenderTarget* renderTarget) const
{
  return true;
}

void Renderer::render(sf::RenderTarget* window)
{
  window->draw(*this);
}

void Renderer::render(sf::RenderTarget* window,
  const sf::RenderStates& renderStates)
{
  window->draw(*this, renderStates);
}