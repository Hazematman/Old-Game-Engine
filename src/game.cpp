#include "game.hpp"
#include "error.hpp"
#include "window/sdl/sdlwindow.hpp"
using namespace std;

#define DEFAULT_WINDOW_TITLE "Eigenspace"

Game::Game() : window(new SDLWindow(DEFAULT_WINDOW_TITLE)) {
  running = true;
}

void Game::run() {
  while(running) {

    window->display();
  }
}
