#include "EmergencyLight.h"

EmergencyLight::EmergencyLight()
{
  m_light = new Light("dual_cone_mask", sf::Color(255, 0, 0, 255));
  RENDER.addLight(m_light);
}

void EmergencyLight::tick()
{
  m_light->rotate(ROTATION_SPEED);
}

void EmergencyLight::setPosition(sf::Vector2f position)
{
  m_light->setPosition(position);
}