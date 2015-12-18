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
  m_window->clear(sf::Color(200, 200, 200, 255));

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
      ++m_drawCalls;
    }
  }
}

void RenderManager::drawWithLights()
{
  m_drawCalls = 0;
  
  for (unsigned int i = 0; i < m_lights.size(); ++i)
  {
    sf::RenderTexture* occluderFBO = m_lights[i]->getOccluderFBO();
    occluderFBO->clear(sf::Color(0, 0, 0, 0));
    sf::View view = occluderFBO->getView();
    view.setCenter(m_lights[i]->getPosition());
    occluderFBO->setView(view);

    for (unsigned int j = 0; j < m_occluders.size(); ++j)
    {
      if (m_occluders[i]->isDestroyed())
      {
        m_occluders[i] = m_occluders.back();
        m_occluders.pop_back();
        --i;
      }
      else if (m_occluders[i]->canRender(occluderFBO))
      {
        m_occluders[i]->render(occluderFBO);
        ++m_drawCalls;
      }
    }

    m_window->clear(sf::Color(200, 200, 200, 255));
    sf::Sprite occluderMap = sf::Sprite(occluderFBO->getTexture());
    occluderMap.setScale(1.0f, -1.0f);
    occluderMap.setOrigin(m_lights[i]->getSize() / 2.0f, m_lights[i]->getSize() / 2.0f);
    occluderMap.setPosition(m_lights[i]->getPosition());
    m_window->draw(occluderMap);
    ++m_drawCalls;
  }
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