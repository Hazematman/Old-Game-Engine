#ifndef GAME_HPP
#define GAME_HPP
#include <memory>
#include "window/window.hpp"

/* Holds the entire state of the engine
 * including loaded data, window, renderer
 * scripting vm, etc.
 */
class Game {
  public:
    Game();

    void run();
  private:
    bool running;
    std::unique_ptr<Window> window;
};

#endif
