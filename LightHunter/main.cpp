//Necessary to get the program to run without the console in the background.
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <iostream>
#include <Mini2D\Game.h>
#include <Mini2D\Ticker.h>

#include "PlayerController.h"
#include "EmergencyLight.h"

class Test : public Ticker
{
public:
  void tick()
  {
    if (INPUT.getMousePressed(sf::Mouse::Left))
    {
      m_light = new Light("sphere_mask", sf::Color(rand() % 256, rand() % 256, rand() % 256, 255));
      sf::Vector2f pos = INPUT.getMouseWorldPosition();
      m_light->setPosition(pos);
      RENDER.addLight(m_light);
    }
  }

private:
  Light* m_light = nullptr;
};

int main()
{
  srand((unsigned int)std::time(NULL));
  Game* game = new Game(120, 60, true);
  game->openWindow("Light Hunter", 1080, 760);

  Renderer* renderer = new Renderer("Sprites/Arenas/test.png");
  RENDER.addRenderer(renderer, true);
  Test* test = new Test();
  PlayerController* player = new PlayerController();
  EmergencyLight emergencyLight;
  emergencyLight.setPosition(sf::Vector2f(850.0f, 170.0f));

  game->mainLoop();

  return 0;
}