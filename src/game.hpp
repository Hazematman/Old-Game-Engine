#ifndef GAME_HPP
#define GAME_HPP
#include <memory>
#include "window/window.hpp"
#include "input/sdlinput.hpp"
#include "graphics/renderer.hpp"
// Need extern c to link in c++
extern "C"
{
#include "wren.h"
}

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
    WrenVM *vm;
    bool running;
    std::unique_ptr<Window> window;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<SDLInput> input;

    void windowResize(int width, int height);
    void initWren();
};

#endif
