//Necessary to get the program to run without the console in the background.
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <iostream>
#include <Mini2D\Game.h>

int main()
{
  srand((unsigned int)std::time(NULL));
  Game* game = new Game(120, 60, true);
  game->openWindow("Light Hunter", 1080, 760);

  Renderer* renderer = new Renderer("Sprites/ship.png");
  RENDER.addRenderer(renderer);

  game->mainLoop();

  return 0;
}