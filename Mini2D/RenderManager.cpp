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
  m_blackOut.loadFromFile("shaders/black_out.frag", sf::Shader::Fragment);
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

  //drawRenderers(m_window);
  if (m_useLights)
  {
    drawLights(m_window);
    drawOccluders(m_window, &m_blackOut);
  }
}

void RenderManager::drawRenderers(sf::RenderTarget* target)
{
  for (unsigned int i = 0; i < m_renderers.size(); ++i)
  {
    if (m_renderers[i]->isDestroyed())
    {
      delete m_renderers[i];
      m_renderers[i] = m_renderers.back();
      m_renderers.pop_back();
      --i;
    }
    else if (m_renderers[i]->canRender(target))
    {
      m_renderers[i]->render(target);
      ++m_drawCalls;
    }
  }
}

void RenderManager::drawOccluders(sf::RenderTarget* target, sf::Shader* shader)
{
  for (unsigned int i = 0; i < m_occluders.size(); ++i)
  {
    if (m_occluders[i]->isDestroyed())
    {
      delete m_occluders[i];
      m_occluders[i] = m_occluders.back();
      m_occluders.pop_back();
      --i;
    }
    else if (m_occluders[i]->canRender(target))
    {
      m_occluders[i]->render(target, shader);
      ++m_drawCalls;
    }
  }
}

void RenderManager::drawLights(sf::RenderTarget* target)
{  
  for (unsigned int i = 0; i < m_lights.size(); ++i)
  {
    if (m_lights[i]->isDestroyed())
    {
      delete m_lights[i];
      m_lights[i] = m_lights.back();
      m_lights.pop_back();
      --i;
    }
    else if (m_lights[i]->canRender(target))
    {
      //Stage one
      sf::RenderTexture* occludersFBO = m_lights[i]->getOccluderFBO();
      occludersFBO->clear(sf::Color(0, 0, 0, 0));
      sf::View view = occludersFBO->getView();
      view.setCenter(m_lights[i]->getPosition());
      occludersFBO->setView(view);
      drawOccluders(occludersFBO, &m_vertexYAxisFlip);

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