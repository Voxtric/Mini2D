#include "RenderManager.h"

#include "Renderer.h"

RenderManager::RenderManager()
{
  m_vertexYAxisFlip.loadFromFile("shaders/vertex_y_axis_flip.vert",
    sf::Shader::Vertex);
  m_shadowMapGenerate.loadFromFile("shaders/shadow_map_generate.frag",
    sf::Shader::Fragment);
  m_shadowMapRender.loadFromFile("shaders/shadow_map_render.frag",
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
  m_drawCalls = 0;
  m_window->clear(sf::Color(0, 0, 0, 255));
  m_useLights ? drawWithLights() : drawWithoutLights();
}

void RenderManager::drawWithoutLights()
{
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
  for (unsigned int i = 0; i < m_lights.size(); ++i)
  {
    //State one
    sf::RenderTexture* occludersFBO = m_lights[i]->getOccluderFBO();
    occludersFBO->clear(sf::Color(0, 0, 0, 0));
    sf::View view = occludersFBO->getView();
    view.setCenter(m_lights[i]->getPosition());
    occludersFBO->setView(view);

    for (unsigned int j = 0; j < m_occluders.size(); ++j)
    {
      if (m_occluders[j]->isDestroyed())
      {
        m_occluders[j] = m_occluders.back();
        m_occluders.pop_back();
        --j;
      }
      else if (m_occluders[j]->canRender(occludersFBO))
      {
        m_occluders[j]->render(occludersFBO, &m_vertexYAxisFlip);
        ++m_drawCalls;
      }
    }

    //Stage two
    sf::RenderTexture* shadowMapFBO = m_lights[i]->getShadowMapFBO();
    shadowMapFBO->clear(sf::Color(0, 0, 0, 0));
    float size = (float)m_lights[i]->getResolution(); //Used in stage three too.
    m_shadowMapGenerate.setParameter("resolution", sf::Vector2f(size, size));
    float scale = m_lights[i]->getScale();
    m_shadowMapGenerate.setParameter("scale", scale);
    sf::Sprite occludersTex = sf::Sprite(occludersFBO->getTexture());
    shadowMapFBO->draw(occludersTex, &m_shadowMapGenerate);

    //Stage three
    m_shadowMapRender.setParameter("resolution", sf::Vector2f(size, size));
    sf::Sprite shadowMapTex = sf::Sprite(shadowMapFBO->getTexture());
    float finalSize = size * scale;
    float x = m_lights[i]->getPosition().x - (finalSize / 2.0f);
    float y = m_lights[i]->getPosition().y - (finalSize / 2.0f) + finalSize;
    shadowMapTex.setPosition(x, y);
    shadowMapTex.setScale(scale, -finalSize);
    shadowMapTex.setColor(m_lights[i]->getColor());

    sf::RenderStates states;
    states.shader = &m_shadowMapRender;
    states.blendMode = sf::BlendAdd;
    m_window->draw(shadowMapTex, states);
  }
  drawWithoutLights();
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