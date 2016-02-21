#include <iostream>
#include "game.hpp"
#include "window/sdl/sdlwindow.hpp"
using namespace std;

int main(int argc, char *argv[]) {
  SDLErrorWindow errorWindow;
  try {
    Game game;

    game.run();
  } catch (Error &error) {
    errorWindow.displayError(error);
  }
	return 0;
}
