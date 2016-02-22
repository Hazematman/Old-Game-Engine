#include <iostream>
#include "game.hpp"
#include "window/sdl/sdlwindow.hpp"
#include "graphics/gl/glrenderer.hpp"
#include "error.hpp"
using namespace std;

#define DEFAULT_WINDOW_TITLE "Eigenspace"
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

void buttonDown(MouseButton button, int x, int y) {
  cout << button << " " << x << " " << y << endl;
}

void keyDown(string &key) {
  cout << key << endl;
}

Game::Game() : 
  window(new SDLWindow(DEFAULT_WINDOW_TITLE)),
  renderer(new GLRenderer),
  input(new SDLInput)
{
  running = true;

  settings.windowTitle = DEFAULT_WINDOW_TITLE;
  settings.windowWidth = DEFAULT_WINDOW_WIDTH;
  settings.windowHeight = DEFAULT_WINDOW_HEIGHT;

  input->setMouseButtonDownCallback(buttonDown);
  input->setKeyDownCallback(keyDown);
}

void Game::run() {
  while(running) {
    input->processInput();
    if(input->getQuit()) {
      running = false;
    }

    window->display();
  }
}

void Game::applySettings() {
  window->setResolution(settings.windowWidth, settings.windowHeight);
  window->setTitle(settings.windowTitle);
}
