#pragma once

#include <Mini2D\Game.h>

class PlayerController : public Ticker
{
public:
  PlayerController();

  void tick();

private:
  Light* m_light = nullptr;
  sf::Vector2f m_position;
};