#include "RenderManager.h"

#include <sstream>
#include <iostream>

#include "Renderer.h"

void RenderManager::setWindow(const sf::RenderWindow* window)
{
  m_window = (sf::RenderWindow*)window;
}

void RenderManager::setUseLights(bool useLights)
{
  m_useLights = useLights;
}

void RenderManager::drawAll()
{
  m_useLights ? drawWithLights() : drawWithoutLights();
}

void RenderManager::drawWithoutLights()
{
  m_drawCalls = 0;
  m_window->clear(sf::Color(255, 255, 255, 255));

  for (unsigned int i = 0; i < m_renderers.size(); ++i)
  {
    if (m_renderers[i]->isDestroyed())
    {
      m_renderers[i] = m_renderers.back();
      m_renderers.pop_back();
      --i;
    }
    else if (m_renderers[i]->canRender(m_window))
    {
      m_renderers[i]->render(m_window);
    }
  }
}

void RenderManager::drawWithLights()
{
  m_drawCalls = 0;
  m_window->clear(sf::Color(255, 255, 255, 255));
}

void RenderManager::addRenderer(const Renderer* renderer)
{
  m_renderers.push_back((Renderer*)renderer);
}

void RenderManager::addOccluder(const Renderer* occluder)
{
  m_occluders.push_back((Renderer*)occluder);
}

void RenderManager::addLight(const Light* light)
{
  m_lights.push_back((Light*)light);
}