#include "Destroyable.h"

void Destroyable::destroy()
{
  m_destroyed = true;
}

bool Destroyable::isDestroyed()
{
  return m_destroyed;
}