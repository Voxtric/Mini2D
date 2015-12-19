#pragma once

#include <SFML\Graphics.hpp>

#include "Light.h"

#define RENDER Game::getActiveGame().getRenderManager()

class Renderer;

class RenderManager
{
public:
  RenderManager();

  void setWindow(const sf::RenderWindow* window);
  void setUseLights(bool useLights);
  void drawAll();
  void addRenderer(const Renderer* renderer);
  void addOccluder(const Renderer* occluder);
  void addLight(const Light* light);

private:
  bool m_useLights = false;
  unsigned int m_drawCalls = 0;

  std::vector<Renderer*> m_renderers;
  std::vector<Renderer*> m_occluders;
  std::vector<Light*> m_lights;

  sf::RenderWindow* m_window = nullptr;
  sf::Shader m_vertexYAxisFlip;
  sf::Shader m_shadowMapGenerate;
  sf::Shader m_shadowMapRender;

  void drawWithoutLights();
  void drawWithLights();
};