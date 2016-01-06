//Necessary to get the program to run without the console in the background.
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <iostream>
#include <Mini2D\Game.h>

class Test : public Ticker
{
public:
  Test()
  {
    newLight();
  }

  void tick()
  {
    m_light->setPosition(INPUT.getMouseWorldPosition());
    if (INPUT.getMousePressed(sf::Mouse::Left))
    {
      newLight();
    }
  }

private:
  Light* m_light;

  void newLight()
  {
    sf::Color color = sf::Color(rand() % 256, rand() % 256, rand() % 256, 255);
    m_light = new Light(sf::Vector2f(200.0f, 200.0f), color, 1.0f, 512);
    RENDER.addLight(m_light);
  }
};

int main()
{
  srand((unsigned int)std::time(NULL));
  Game* game = new Game(120, 60, true);
  game->openWindow("Light Hunter", 1080, 760);

  for (unsigned int i = 0; i < 15; ++i)
  {
    Renderer* renderer = new Renderer("sprites/ship.png");
    RENDER.addOccluder(renderer);
    RENDER.addRenderer(renderer);
    renderer->setPosition(rand() % 1080, rand() % 770);
    renderer->setRotation(rand() % 360);
  }
  Test test;

  game->mainLoop();

  return 0;
}