#pragma once
class Destroyable
{
public:
  void destroy();
  bool isDestroyed();

private:
  bool m_destroyed = false;
};

