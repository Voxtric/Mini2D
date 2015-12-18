#include "InputManager.h"

#include <iostream>
#include "Game.h"

void InputManager::pollEvents(sf::RenderWindow* window)
{
  for (auto& iterator : m_currentKeyState)
  {
      m_previousKeyState[iterator.first] = iterator.second;
  }
  for (auto& iterator : m_currentMouseState)
  {
    m_previousMouseState[iterator.first] = iterator.second;
  }
  
  sf::Event evnt;
  while (window->pollEvent(evnt))
  {
    switch (evnt.type)
    {
    case sf::Event::Closed:
      window->close();
      std::cout << "Closing window.\n";
      return;
    case sf::Event::KeyPressed:
      pressKey(evnt.key.code);
      break;
    case sf::Event::KeyReleased:
      m_keyReleaseQueue.push_back(evnt.key.code);
      break;
    case sf::Event::MouseButtonPressed:
      pressMouse(evnt.mouseButton.button);
      break;
    case sf::Event::MouseButtonReleased:
      m_mouseReleaseQueue.push_back(evnt.mouseButton.button);
      break;
    case sf::Event::MouseMoved:
      m_mouseScreenPosition.x = (float)evnt.mouseMove.x;
      m_mouseScreenPosition.y = (float)evnt.mouseMove.y;      
      break;
    case sf::Event::Resized:
      {
        sf::Vector2u size = Game::getActiveGame().getWindow()->getSize();
        sf::View view(sf::FloatRect(0.0f, 0.0f, (float)size.x, (float)size.y));
        Game::getActiveGame().getWindow()->setView(view);
      }
      break;
    case sf::Event::TextEntered:
    case::sf::Event::MouseLeft:
    case::sf::Event::MouseEntered:
      break;
    default:
      std::cout << "WARNING: Input polled event " << evnt.type << " but no action could be taken!\n";
      break;
    }
  }
  m_mouseWorldPosition = window->mapPixelToCoords((sf::Vector2i)m_mouseScreenPosition);
}

void InputManager::releaseQueuedInputs()
{
  unsigned int size = m_keyReleaseQueue.size();
  while (size > 0)
  {
    releaseKey(m_keyReleaseQueue[size - 1]);
    m_keyReleaseQueue.pop_back();
    --size;
  }

  size = m_mouseReleaseQueue.size();
  while (size > 0)
  {
    releaseMouse(m_mouseReleaseQueue[size - 1]);
    m_mouseReleaseQueue.pop_back();
    --size;
  }
}

void InputManager::pressKey(unsigned int keyID)
{
  m_currentKeyState[keyID] = true;
}

void InputManager::releaseKey(unsigned int keyID)
{
  m_currentKeyState[keyID] = false;
}

void InputManager::pressMouse(unsigned int mouseButtonID)
{
  m_currentMouseState[mouseButtonID] = true;
}

void InputManager::releaseMouse(unsigned int mouseButtonID)
{
  m_currentMouseState[mouseButtonID] = false;
}

bool InputManager::wasKeyDown(unsigned int keyID)
{
  std::unordered_map<unsigned int, bool>::iterator iterator = m_previousKeyState.find(keyID);
  if (iterator != m_previousKeyState.end())
  {
    return iterator->second;
  }
  return false;
}

bool InputManager::wasMouseDown(unsigned int mouseButtonID)
{
  std::unordered_map<unsigned int, bool>::iterator iterator = m_previousMouseState.find(mouseButtonID);
  if (iterator != m_previousMouseState.end())
  {
    return iterator->second;
  }
  return false;
}

void InputManager::setMouseScreenPosition(sf::RenderWindow& window, unsigned int xPosition, unsigned int yPosition)
{
  sf::Mouse::setPosition(sf::Vector2i(xPosition, yPosition));
}

sf::Vector2f InputManager::getMouseScreenPosition() const
{
  return m_mouseScreenPosition;
}

sf::Vector2f InputManager::getMouseWorldPosition() const
{
  return m_mouseWorldPosition;
}

bool InputManager::getKeyDown(unsigned int keyID)
{
  std::unordered_map<unsigned int, bool>::iterator iterator = m_currentKeyState.find(keyID);
  if (iterator != m_currentKeyState.end())
  {
    return iterator->second;
  }
  return false;
}

bool InputManager::getKeyPressed(unsigned int keyID)
{
  if (getKeyDown(keyID) && !wasKeyDown(keyID))
  {
  return true;
  }
  return false;
}

bool InputManager::getMouseDown(unsigned int mouseButtonID)
{
  std::unordered_map<unsigned int, bool>::iterator iterator = m_currentMouseState.find(mouseButtonID);
  if (iterator != m_currentMouseState.end())
  {
    return iterator->second;
  }
  return false;
}

bool InputManager::getMousePressed(unsigned int mouseButtonID)
{
  if (getMouseDown(mouseButtonID) && !wasMouseDown(mouseButtonID))
  {
    return true;
  }
  return false;
}