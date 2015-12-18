#include "Ticker.h"
#include "Game.h"

Ticker::Ticker()
{
  Game::getActiveGame().addTicker(this);
}