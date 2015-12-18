#include "RenderManager.h"
#include <sstream>

RenderManager::~RenderManager()
{
  if (m_normalRender != nullptr)
  {
    delete m_normalRender;
    m_normalRender = nullptr;
  }
  if (m_shadowMap != nullptr)
  {
    delete m_shadowMap;
    m_shadowMap = nullptr;
  }
  for (auto& iterator : m_postProcessors)
  {
    delete iterator.second;
  }
}

void RenderManager::setWindow(const sf::RenderWindow* window, 
  bool updateLightComponents)
{
  m_window = (sf::RenderWindow*)window;
  if (updateLightComponents)
  {
    refreshLightComponents();
  }
}

void RenderManager::refreshLightComponents()
{
  if (m_normalRender != nullptr)
  {
    delete m_normalRender;
  }
  m_normalRender = new sf::RenderTexture();
  m_normalRender->create(m_window->getSize().x, m_window->getSize().y);

  if (m_shadowMap != nullptr)
  {
    delete m_shadowMap;
  }
  m_shadowMap = new sf::RenderTexture();
  m_shadowMap->create(m_window->getSize().x, m_window->getSize().y);

  m_lightGiverShader.loadFromFile("Shaders/Lights/normal.vert", "Shaders/Lights/lightGiver.frag");
  m_lightGiverShader.setParameter("windowSize", (sf::Vector2f)m_window->getSize());
  m_lightGiverShader.setParameter("shadowMap", m_shadowMap->getTexture());
  m_lightBlockerShader.loadFromFile("Shaders/Lights/normal.vert", "Shaders/Lights/lightBlocker.frag");
}

void RenderManager::drawWithLights()
{
  m_drawCalls = 0;
  m_window->clear();
  m_normalRender->clear(sf::Color(180, 180, 180, 255));
  m_normalRender->setView(m_window->getView());

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
      m_renderers[i]->render(m_normalRender);
    }
  }

  sf::Sprite sprite = sf::Sprite(m_normalRender->getTexture());
  sprite.setPosition(m_window->mapPixelToCoords(sf::Vector2i(0, 0)));
  m_window->draw(sprite, m_postProcessShader);
}

void RenderManager::drawWithoutLights()
{
  m_drawCalls = 0;
  m_window->clear();

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

void RenderManager::createPostProcessShader(unsigned int lightCount)
{
  const std::string vertexShader = \
    "void main()" \
    "{" \
    "  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;" \
    "  gl_Position.y *= -1.0;" \
    "  gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;" \
    "  gl_FrontColor = gl_Color;" \
    "}";

  const std::string fragShaderStart = \
    "void main()" \
    "{" \
    "  vec4 normalRenderPixel = texture2D(texture, gl_TexCoord[0].xy);" \
    "  vec4 totalPixelValue = vec4(0.0, 0.0, 0.0, 0.0);";

  const std::string fragShaderEnd = \
    "  gl_FragColor = normalRenderPixel * totalPixelValue;" \
    "}";

  std::stringstream fragmentShader;
  fragmentShader << "uniform sampler2D texture;\n";
  for (unsigned int i = 0; i < lightCount; ++i)
  {
    fragmentShader << "  uniform sampler2D lightMap" << i << ";";
  }
  fragmentShader << fragShaderStart;
  for (unsigned int i = 0; i < lightCount; ++i)
  {
    fragmentShader << "  totalPixelValue += texture2D(lightMap" << i <<
      ", gl_TexCoord[0].xy);";
  }
  fragmentShader << fragShaderEnd;

  sf::Shader* shader = new sf::Shader();
  shader->loadFromMemory(vertexShader, fragmentShader.str());
  m_postProcessors.emplace(lightCount, shader);
}

void RenderManager::setActivePostProcessShader(unsigned int lightCount)
{
  auto iterator = m_postProcessors.find(lightCount);
  if (iterator == m_postProcessors.end())
  {
    createPostProcessShader(lightCount);
    setActivePostProcessShader(lightCount);
  }
  else
  {
    m_postProcessShader = iterator->second;
  }
}

void RenderManager::refreshLightMap()
{
  m_shadowMap->clear(sf::Color(255, 255, 255, 255));
  m_shadowMap->setView(m_window->getView());

  for (unsigned int i = 0; i < m_lightBlockers.size(); ++i)
  {
    if (m_lightBlockers[i]->isDestroyed())
    {
      m_lightBlockers[i] = m_lightBlockers.back();
      m_lightBlockers.pop_back();
      --i;
    }
    else if (m_lightBlockers[i]->canRender(m_shadowMap))
    {
      m_lightBlockers[i]->render(m_shadowMap, &m_lightBlockerShader);
    }
  }

  unsigned int lightsUsed = 0;
  std::vector<unsigned int> lightIndices(m_lights.size());
  for (unsigned int i = 0; i < m_lights.size(); ++i)
  {
    if (m_lights[i]->isDestroyed())
    {
      m_lights[i] = m_lights.back();
      m_lights.pop_back();
      --i;
    }
    else if (m_lights[i]->canRender(m_lights[i]->getLightMap()))
    {
      m_lights[i]->refreshLightMap();
      m_lights[i]->getLightMap()->clear(sf::Color(0, 0, 0, 255));
      m_lights[i]->getLightMap()->setView(m_window->getView());

      sf::Vector2f lightPositionForShader = sf::Vector2f(m_lights[i]->getPosition().x, m_window->getSize().y - m_lights[i]->getPosition().y) -
        sf::Vector2f(m_window->mapPixelToCoords(sf::Vector2i(0, 0)).x, m_window->getSize().y - m_window->mapPixelToCoords(sf::Vector2i(0, m_window->getSize().y)).y);
      m_lightGiverShader.setParameter("lightPosition", lightPositionForShader);
      m_lightGiverShader.setParameter("lightColor", m_lights[i]->getColor());
      m_lights[i]->render(m_lights[i]->getLightMap(), &m_lightGiverShader);
      lightIndices[lightsUsed] = i;
      ++lightsUsed;
    }
  }

  setActivePostProcessShader(lightsUsed);
  for (unsigned int i = 0; i < lightsUsed; ++i)
  {
    std::stringstream stream;
    stream << "lightMap" << i;
    m_postProcessShader->setParameter(stream.str(), m_lights[lightIndices[i]]->
      getLightMap()->getTexture());
  }
}

void RenderManager::addRenderer(const Renderer* renderer, bool blocksLight)
{
  if (blocksLight)
  {
    m_lightBlockers.push_back((Renderer*)renderer);
  }
  else
  {
    m_renderers.push_back((Renderer*)renderer);
  }
}

void RenderManager::addLight(const Light* light)
{
  m_lights.push_back((Light*)light);
}