#pragma once

#include "Destroyable.h"

class Ticker : public Destroyable
{
public:
  Ticker();
  virtual void tick() = 0;
};