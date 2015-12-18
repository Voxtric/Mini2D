#pragma once

#include "InputManager.h"
#include "TextManager.h"
#include "PhysicsManager.h"
#include "TextureManager.h"
#include "RenderManager.h"
#include "Ticker.h"

#define GAME Game::getActiveGame()

class Game
{
public:
  Game(unsigned int frameRate, unsigned int tickRate);
  ~Game();

  void mainLoop();
  void openWindow(const std::string& windowTitle, unsigned int pixelWidth,
  unsigned int pixelHeight);

  void addTicker(const Ticker* ticker);

  sf::RenderWindow* getWindow() const;
  InputManager& getInputManager();
  TextManager& getTextManager();
  PhysicsManager& getPhysicsManager();
  TextureManager& getTextureManager();
  RenderManager& getRenderManager();

  static Game& getActiveGame();

private:
  unsigned int m_targetTickRate = 0;
  unsigned int m_targetFrameRate = 0;
  std::string m_ticksInLastSecond = "0";
  std::string m_framesInLastSecond = "0";
  std::vector<Ticker*> m_tickers;

  sf::RenderWindow* m_window = nullptr;

  InputManager m_inputManager;
  TextManager m_textManager;
  PhysicsManager m_physicsManager;
  TextureManager m_textureManager;
  RenderManager m_renderManager;

  static Game* s_activeGame;

  void drawAll();
  void tickAll();
};