#include <SDL2/SDL_image.h>
#include "sdlwindow.hpp"
using namespace std;

#define ERROR_TYPE "SDL Error"

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

#define BITS_PER_PIXEL 8
#define DEPTH_BUFFER_SIZE 24
#define STENCIL_BUFFER_SIZE 8
#define MAJOR_GL_VERSION 3
#define MINOR_GL_VERSION 3

SDLWindow::SDLWindow(const string &title) {
  if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    throw Error(ERROR_TYPE, "Could not initalize SDL");
  }
  checkSDLError();

  // Init SDL_Image for image loading
  int flags = IMG_INIT_JPG | IMG_INIT_PNG;
  if((IMG_Init(flags)&flags) != flags) {
    throw Error(ERROR_TYPE, IMG_GetError());
  }

  // Initalize OpengGL stuff for the window
  // TODO find a way to abstract this to support vulkan
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, BITS_PER_PIXEL);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, BITS_PER_PIXEL);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, BITS_PER_PIXEL);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, BITS_PER_PIXEL);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, STENCIL_BUFFER_SIZE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEPTH_BUFFER_SIZE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, MAJOR_GL_VERSION);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, MINOR_GL_VERSION);

  // Create SDL window
  screen = SDL_CreateWindow(title.c_str(),
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      DEFAULT_WIDTH, DEFAULT_HEIGHT,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  checkSDLError();

  // Create OpenGL 3.3 context
  context = SDL_GL_CreateContext(screen);
  checkSDLError();

  // Set any values initally
  width = DEFAULT_WIDTH;
  height = DEFAULT_HEIGHT;
}

SDLWindow::~SDLWindow() {
  SDL_Quit();
}

void SDLWindow::setResolution(int width, int height) {
  this->width = width;
  this->height = height;
  SDL_SetWindowSize(screen, width, height);
}

void SDLWindow::getResolution(int &width, int &height) {
  width = this->width;
  height = this->height;
}

void SDLWindow::setTitle(const string title) {
  SDL_SetWindowTitle(screen, title.c_str());
}

void SDLWindow::display() {
  SDL_GL_SwapWindow(screen);
}

SDL_Window *SDLWindow::getHandle() {
  return screen;
}

void SDLWindow::checkSDLError() {
  const char *errorMessage = SDL_GetError();
	
  // Make sure the message isn't empty
  if(*errorMessage != '\0') {
    // OH NO SDL ERROR
    // Ye who enter abandon all hope
    throw Error(ERROR_TYPE, errorMessage);
  }
}

void SDLErrorWindow::displayError(Error &error) {
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, 
      error.getType().c_str(), error.getMessage().c_str(), NULL);
}
