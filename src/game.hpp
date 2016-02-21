#ifndef GAME_HPP
#define GAME_HPP
#include <memory>
#include "window/sdl/sdlwindow.hpp"
#include "input/sdlinput.hpp"

/* Struct to representing the current settings
 * of the engine.
 */
struct Settings {
  std::string windowTitle;
  int windowWidth;
  int windowHeight;
};

/* Holds the entire state of the engine
 * including loaded data, window, renderer
 * scripting vm, etc.
 */
class Game {
  public:
    Game();

    void run();
    void applySettings();
  private:
    Settings settings;
    bool running;
    std::unique_ptr<Window> window;
    std::unique_ptr<SDLInput> input;
};

#endif
