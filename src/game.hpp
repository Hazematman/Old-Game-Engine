#ifndef GAME_HPP
#define GAME_HPP
#include <memory>
#include "window/window.hpp"
#include "input/sdlinput.hpp"
#include "graphics/renderer.hpp"
#include "loader/loader.hpp"
#include "gui/console.hpp"
// Need extern c to link in c++

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
    ~Game();

    void run();
    void applySettings();
  private:
    Settings settings;
    bool running;
    std::unique_ptr<Window> window;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<SDLInput> input;
    std::shared_ptr<Font> defaultFont;
    std::unique_ptr<Console> console;
    Loader loader;

    void windowResize(int width, int height);
};

#endif
