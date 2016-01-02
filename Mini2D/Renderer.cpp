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
  bool canRender = false;
  sf::View view = renderTarget->getView();
  sf::Vector2f size = view.getSize();
  sf::Vector2f position = view.getCenter() - (size / 2.0f);
  sf::FloatRect viewRect(position, size);
  canRender = viewRect.intersects(getGlobalBounds());
  return canRender;
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