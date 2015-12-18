#pragma once

#include <SFML\Graphics.hpp>

#define RENDER Game::getActiveGame().getRenderManager()

class Renderer;

class RenderManager
{
public:
  void setWindow(const sf::RenderWindow* window);
  void drawWithoutLights();
  void addRenderer(const Renderer* renderer);

private:
  unsigned int m_drawCalls = 0;
  std::vector<Renderer*> m_renderers;
  sf::RenderWindow* m_window = nullptr;
};