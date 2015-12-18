#pragma once

#include <Mini2D\Game.h>

class EmergencyLight : public Ticker
{
public:
  EmergencyLight();

  void tick();
  void setPosition(sf::Vector2f position);

private:
  const float ROTATION_SPEED = 2.5f;

  Light* m_light = nullptr;
};