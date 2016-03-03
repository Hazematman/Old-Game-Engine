#include <iostream>
#include <fstream>
#include <sstream>
#include "game.hpp"
#include "window/sdl/sdlwindow.hpp"
#include "graphics/gl/glrenderer.hpp"
#include "binding/binding.hpp"
#include "error.hpp"
#include "gui/console.hpp"
using namespace std;

#define DEFAULT_WINDOW_TITLE "Eigenspace"
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

Game::Game() : 
  window(new SDLWindow(DEFAULT_WINDOW_TITLE)),
  renderer(new GLRenderer),
  input(new SDLInput),
  loader(renderer)
{
  running = true;

  settings.windowTitle = DEFAULT_WINDOW_TITLE;
  settings.windowWidth = DEFAULT_WINDOW_WIDTH;
  settings.windowHeight = DEFAULT_WINDOW_HEIGHT;

  defaultFont = loader.loadFont("defaultfont.fnt");
  console.reset(new Console(renderer, input, defaultFont,
        glm::vec4(0.2,0.2,0.2,1.0), glm::vec4(0.4,0.4,0.4,1.0), glm::vec4(0.8,0.8,0.8,1.0)));
  console->setRunCallback(runString);
  setLogFunction(bind(&Console::print, console.get(), placeholders::_1));

  input->setKeyDownCallback(keyDownCallback);
  input->setKeyUpCallback(keyUpCallback);
  input->setMouseButtonDownCallback(mouseDownCallback);
  input->setMouseButtonUpCallback(mouseUpCallback);
  input->setMouseMovedCallback(mouseMovedCallback);

  input->setResizeCallback(bind(&Game::windowResize, this, placeholders::_1, placeholders::_2));

  renderer->setRenderDimensions(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

  bindingInit();

  callInit();
}

Game::~Game() {
  bindingFree();
}

void Game::run() {
  Box box(renderer->getBlankTexture(), glm::vec2(100,100),glm::vec2(100,100),glm::vec4(1.0,0.0,0.0,1.0));
  while(running) {
    input->processInput();
    if(input->getQuit()) {
      running = false;
    }

    //callRun(0);    

    renderer->clearColour();
    renderer->clearDepth();

    renderer->setDepthTest(false);
    renderer->drawBox(box);

    console->draw();

    window->display();
  }
}

void Game::applySettings() {
  window->setResolution(settings.windowWidth, settings.windowHeight);
  window->setTitle(settings.windowTitle);
}

void Game::windowResize(int width, int height) {
  renderer->setRenderDimensions(width, height);
}
