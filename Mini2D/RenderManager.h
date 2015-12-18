#pragma once

#include "Light.h"

#define RENDER Game::getActiveGame().getRenderManager()

class RenderManager
{
public:
  ~RenderManager();

  void setWindow(const sf::RenderWindow* window, bool updateLightComponents);
  void refreshLightComponents();
  void refreshLightMap();
  void drawWithLights();
  void drawWithoutLights();

  void addRenderer(const Renderer* renderer, bool blocksLight);
  void addLight(const Light* light);

private:
  unsigned int m_drawCalls = 0;

  std::vector<Light*> m_lights;
  std::vector<Renderer*> m_renderers;
  std::vector<Renderer*> m_lightBlockers;

  sf::RenderWindow* m_window = nullptr;
  sf::RenderTexture* m_normalRender = nullptr;
  sf::RenderTexture* m_shadowMap = nullptr;

  sf::Shader m_lightGiverShader;
  sf::Shader m_lightBlockerShader;

  sf::Shader* m_postProcessShader = nullptr;
  std::map<unsigned int, sf::Shader*> m_postProcessors;

  void createPostProcessShader(unsigned int lightCount);
  void setActivePostProcessShader(unsigned int lightCount);
};