#include "PlayerController.h"

#include <iostream>

PlayerController::PlayerController()
{
  m_position = sf::Vector2f(50.0f, 50.0f);
  m_light = new Light("cone_mask", sf::Color(255, 255, 255, 255));
  RENDER.addLight(m_light);
}

void PlayerController::tick()
{
  const float SPEED = 2.0f;
  if (INPUT.getKeyDown(sf::Keyboard::D))
  {
    m_position.x += SPEED;
  }
  else if (INPUT.getKeyDown(sf::Keyboard::A))
  {
    m_position.x -= SPEED;
  }
  if (INPUT.getKeyDown(sf::Keyboard::S))
  {
    m_position.y += SPEED;
  }
  else if (INPUT.getKeyDown(sf::Keyboard::W))
  {
    m_position.y -= SPEED;
  }

  m_light->setPosition(m_position);
  float x = INPUT.getMouseWorldPosition().x - m_position.x;
  float y = INPUT.getMouseWorldPosition().y - m_position.y;
  float angle = atan2(y, x) * (float)(180 / (atan(1) * 4));
  m_light->setRotation(angle);

  sf::View view = GAME.getWindow()->getView();
  view.setCenter(m_position);
  GAME.getWindow()->setView(view);
}