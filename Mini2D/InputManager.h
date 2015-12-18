#pragma once

#include <unordered_map>
#include <SFML\Graphics.hpp>

#define INPUT Game::getActiveGame().getInputManager()

class InputManager
{
public:
  void pollEvents(sf::RenderWindow* window);
  void releaseQueuedInputs();

  void setMouseScreenPosition(sf::RenderWindow& window, unsigned int xPosition, 
  unsigned int yPosition);
  sf::Vector2f getMouseScreenPosition() const;
  sf::Vector2f getMouseWorldPosition() const;
  bool getMouseDown(unsigned int mouseButtonID);
  bool getMousePressed(unsigned int mouseButtonID);

  bool getKeyDown(unsigned int keyID);
  bool getKeyPressed(unsigned int keyID);

private:
  sf::Vector2f m_mouseScreenPosition;
  sf::Vector2f m_mouseWorldPosition;
  std::unordered_map<unsigned int, bool> m_currentKeyState;
  std::unordered_map<unsigned int, bool> m_previousKeyState;
  std::unordered_map<unsigned int, bool> m_currentMouseState;
  std::unordered_map<unsigned int, bool> m_previousMouseState;
  std::vector<unsigned int> m_keyReleaseQueue;
  std::vector<unsigned int> m_mouseReleaseQueue;

  void pressKey(unsigned int keyID);
  void releaseKey(unsigned int keyID);
  bool wasKeyDown(unsigned int keyID);

  void pressMouse(unsigned int mouseButtonID);
  void releaseMouse(unsigned int mouseButtonID);
  bool wasMouseDown(unsigned int mouseButtonID);
};