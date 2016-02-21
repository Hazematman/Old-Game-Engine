#ifndef SDLWINDOW_HPP
#define SDLWINDOW_HPP
#include <SDL2/SDL.h>
#include <string>
#include "../window.hpp"

/* SDL window with opengl capabilites
 */
class SDLWindow : public Window {
  public:
    SDLWindow(const std::string &title);
    ~SDLWindow();

    void setResolution(int width, int height);
    void getResolution(int &width, int &height);
    void setTitle(const std::string title);
    void display();

    SDL_Window *getHandle();
  private:
    SDL_Window *screen;
    SDL_GLContext context;
    int width,height;
    void checkSDLError();
};

class SDLErrorWindow : ErrorWindow {
  public:
    void displayError(Error &error);
};

#endif
