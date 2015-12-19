#include "RenderManager.h"

#include "Renderer.h"

RenderManager::RenderManager()
{
  m_vertexYAxisFlip.loadFromFile("shaders/vertex_y_axis_flip.vert",
    sf::Shader::Vertex);
  m_shadowMapGen.loadFromFile("shaders/shadow_map_gen.frag",
    sf::Shader::Fragment);
}

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
    sf::RenderTexture* occludersFBO = m_lights[i]->getOccluderFBO();
    occludersFBO->clear(sf::Color(0, 0, 0, 0));
    sf::View view = occludersFBO->getView();
    view.setCenter(m_lights[i]->getPosition());
    occludersFBO->setView(view);

    for (unsigned int j = 0; j < m_occluders.size(); ++j)
    {
      if (m_occluders[i]->isDestroyed())
      {
        m_occluders[i] = m_occluders.back();
        m_occluders.pop_back();
        --i;
      }
      else if (m_occluders[i]->canRender(occludersFBO))
      {
        m_occluders[i]->render(occludersFBO, &m_vertexYAxisFlip);
        ++m_drawCalls;
      }
    }

    sf::RenderTexture* shadowMapFBO = m_lights[i]->getShadowMapFBO();
    shadowMapFBO->clear(sf::Color(0, 0, 0, 0));
    float size = (float)m_lights[i]->getSize();
    m_shadowMapGen.setParameter("resolution", sf::Vector2f(size, size));
    sf::Sprite occludersTex = sf::Sprite(occludersFBO->getTexture());
    shadowMapFBO->draw(occludersTex, &m_shadowMapGen);

    //Render the shadow map frame buffer
    /*
    m_window->clear(sf::Color(200, 200, 200, 255));
    sf::Sprite shadowMapTex = sf::Sprite(shadowMapFBO->getTexture());
    m_window->draw(shadowMapTex);
    */

    //Render the occluders frame buffer
    /*
    m_window->clear(sf::Color(200, 200, 200, 255));
    sf::Sprite occludersTex = sf::Sprite(occludersFBO->getTexture());
    occludersTex.setOrigin(m_lights[i]->getSize() / 2.0f, m_lights[i]->getSize() / 2.0f);
    occludersTex.setPosition(m_lights[i]->getPosition());
    m_window->draw(occluderMap);
    ++m_drawCalls;
    */
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