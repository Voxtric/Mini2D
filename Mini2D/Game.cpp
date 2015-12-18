#include "Game.h"

#define GLEW_STATIC

#include <iostream>
#include <sstream>
#include <GL\glew.h>

Game* Game::s_activeGame = nullptr;

Game::Game(unsigned int frameRate, unsigned int tickRate, bool useLights)
{
  m_targetFrameRate = frameRate;
  m_targetTickRate = tickRate;
  m_renderManager.setUseLights(useLights);

  m_textManager.addTextToDisplay(&m_ticksInLastSecond, 
    sf::Vector2f(0.0f, 0.0f));
  m_textManager.addTextToDisplay(&m_framesInLastSecond, 
    sf::Vector2f(0.0f, 16.0f));
}

Game::~Game()
{
  if (m_window != nullptr)
  {
    m_window->close();
    delete m_window;
    m_window = nullptr;
  }
  s_activeGame = nullptr;
}

void Game::mainLoop()
{
  if (m_window == nullptr)
  {
    std::cout << "ERROR: Cannot start main game loop without a window!\n";
    return;
  }

  std::cout << "Beginning main game loop.\n";

  sf::Clock tickClock;
  unsigned int lastTick = tickClock.getElapsedTime().asMilliseconds();
  float tickDifference = 0.0f;
  unsigned int ticksElapsed = 0;
  const float TICKS_PER_SECOND = 1000.0f / m_targetTickRate;

  sf::Clock frameClock;
  unsigned int lastFrame = frameClock.getElapsedTime().asMilliseconds();
  float frameDifference = 0.0f;
  unsigned int framesElapsed = 0;
  const float FRAMES_PER_SECOND = 1000.0f / m_targetFrameRate;

  int updateTarget = 0;

  while (m_window->isOpen())
  {
    //Perform all per tick operations.
    unsigned int timeSinceTick = tickClock.getElapsedTime().asMilliseconds() - lastTick;
    if (tickDifference + (float)timeSinceTick >= TICKS_PER_SECOND)
    {
      lastTick = tickClock.getElapsedTime().asMilliseconds();
      m_inputManager.pollEvents(m_window);
      tickAll();
      m_inputManager.releaseQueuedInputs();
      tickDifference += (float)timeSinceTick - TICKS_PER_SECOND;
      ++ticksElapsed;
    }

    //Perform all per frame operations.
    if (m_targetFrameRate == 0)
    {
      lastFrame = frameClock.getElapsedTime().asMilliseconds();
      drawAll();
      ++framesElapsed;
    }
    else if (frameDifference <= FRAMES_PER_SECOND)  
    {
      unsigned int timeSinceFrame = frameClock.getElapsedTime().asMilliseconds() - lastFrame;
      if (frameDifference + (float)timeSinceFrame >= FRAMES_PER_SECOND)
      {
        frameDifference += (float)timeSinceFrame - FRAMES_PER_SECOND;
        lastFrame = frameClock.getElapsedTime().asMilliseconds();
        drawAll();
        ++framesElapsed;
      }
    }
    else
    {
      //Ensure frames get dropped completely rather than just delayed.
      frameDifference = 0.0f;    
    }

    //Updates debug information once a second.
    if (frameClock.getElapsedTime().asMilliseconds() >= updateTarget)
    {
      updateTarget += 1000;

      std::stringstream tickStream;
      tickStream << "TPS: " << ticksElapsed;
      m_ticksInLastSecond = tickStream.str();
      ticksElapsed = 0;

      std::stringstream frameStream;
      frameStream << "FPS: " << framesElapsed;
      m_framesInLastSecond = frameStream.str();
      framesElapsed = 0;
    }
  }
  s_activeGame = nullptr;
}

void Game::openWindow(const std::string& windowTitle, unsigned int pixelWidth, 
  unsigned int pixelHeight)
{
  s_activeGame = this;
  m_window = new sf::RenderWindow(sf::VideoMode(pixelWidth, pixelHeight),
    windowTitle, sf::Style::Close);
  m_window->setKeyRepeatEnabled(false);
  std::cout << "Opening new window \"" << windowTitle << 
    "\" with dimensions of " << pixelWidth << " by " << pixelHeight << ".\n";
  m_renderManager.setWindow(m_window);
}

sf::RenderWindow* Game::getWindow() const
{
  return m_window;
}

InputManager& Game::getInputManager()
{
  return m_inputManager;
}

void Game::drawAll()
{
  m_renderManager.drawAll();
  m_textManager.drawText(m_window);
  m_window->display();
}

void Game::tickAll()
{
  for (unsigned int i = 0; i < m_tickers.size(); ++i)
  {
    if (m_tickers[i]->isDestroyed())
    {
      m_tickers[i] = m_tickers.back();
      m_tickers.pop_back();
      --i;
    }
    else
    {
      m_tickers[i]->tick();
    }
  }
}

void Game::addTicker(const Ticker* tickable)
{
  m_tickers.push_back((Ticker*)tickable);
}

TextManager& Game::getTextManager()
{
  return m_textManager;
}

PhysicsManager& Game::getPhysicsManager()
{
  return m_physicsManager;
}

TextureManager& Game::getTextureManager()
{
  return m_textureManager;
}

RenderManager& Game::getRenderManager()
{
  return m_renderManager;
}

//Static function for getting whatever game is currently in its main loop.
Game& Game::getActiveGame()
{
  return *s_activeGame;
}