#include "sdlinput.hpp"
using namespace std;

#define MAX_INPUT_SIZE 2048

SDLInput::SDLInput() {
  quit = false;
  focus = true;
  gui = false;
  keys = (uint8_t*)SDL_GetKeyboardState(NULL);
}

void SDLInput::processInput() {
  SDL_Event e;
  while(SDL_PollEvent(&e)) {
    switch(e.type) {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_WINDOWEVENT:
        switch(e.window.event) {
          case SDL_WINDOWEVENT_FOCUS_GAINED:
            focus = true;
            break;
          case SDL_WINDOWEVENT_FOCUS_LOST:
            focus = false;
            break;
        }
        break;
      case SDL_TEXTINPUT:
        addInput(string(e.text.text));
        break;
      case SDL_KEYDOWN:
        if(KeyDownCallback) {
          string key = SDL_GetKeyName(e.key.keysym.sym);
          KeyDownCallback(key);
        }
        switch(e.key.keysym.sym) {
          case SDLK_BACKSPACE:
            addInput("\b");
            break;
        }
        break;
      case SDL_KEYUP:
        if(keyUpCallback) {
          string key = SDL_GetKeyName(e.key.keysym.sym);
          keyUpCallback(key);
        }
        break;
      case SDL_MOUSEMOTION:
        if(mouseMovedCallback) {
          mouseMovedCallback(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
        }
        break;
      case SDL_MOUSEBUTTONUP:
        if(mouseButtonUpCallback) {
          mouseButtonUpCallback((MouseButton)e.button.button, 
              e.button.x, e.button.y);
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        if(mouseButtonDownCallback) {
          mouseButtonDownCallback((MouseButton)e.button.button,
              e.button.x, e.button.y);
        }
        break;
    }
  }
}

void SDLInput::setKeyDownCallback(function<void(string&)> callback) {
  keyUpCallback = callback;
}

void SDLInput::setKeyUpCallback(function<void(string&)> callback) {
  KeyDownCallback = callback;
}

void SDLInput::setMouseMovedCallback(function<void(int,int,int,int)> callback) {
  mouseMovedCallback = callback;
}

void SDLInput::setMouseButtonUpCallback(function<void(MouseButton,int,int)> callback) {
  mouseButtonUpCallback = callback;
}

void SDLInput::setMouseButtonDownCallback(function<void(MouseButton,int,int)> callback) {
  mouseButtonDownCallback = callback;
}

void SDLInput::setGUILock(bool gui) {
  this->gui = gui;
}

bool SDLInput::isKeyDown(string &key, bool guiLocked) {
  SDL_Scancode sc = SDL_GetScancodeFromName(key.c_str());
  return (gui == guiLocked) && focus && (sc != SDL_SCANCODE_UNKNOWN) && keys[sc];
}

bool SDLInput::isMouseDown(MouseButton button, bool guiLocked) {
  int mask = SDL_GetMouseState(NULL, NULL);
  return (gui == guiLocked) && focus && (mask&SDL_BUTTON(button));
}

glm::vec2 SDLInput::getMousePos(bool guiLocked) {
  if(focus && (gui == guiLocked)) {
    int x,y;
    SDL_GetMouseState(&x, &y);
    return glm::vec2(x,y);
  }

  return glm::vec2(-1,-1);
}

void SDLInput::setMousePos(glm::vec2 pos, bool guiLocked) {
  if(focus && (gui == guiLocked)) {
    int x = pos.x, y = pos.y;
    SDL_WarpMouseInWindow(NULL, x, y);
  }
}

string SDLInput::getInput(bool guiLocked) {
  if(focus && (gui == guiLocked)) {
    return input;
  }

  return "";
}

void SDLInput::clearInput(bool guiLocked) {
  if(focus && (gui == guiLocked)) {
    input = "";
  }
}

void setMouseRelative(bool relative) {
  SDL_SetRelativeMouseMode((SDL_bool)relative);
}

bool SDLInput::getQuit() {
  return quit;
}

void SDLInput::addInput(string newInput) {
  if(input.size() > MAX_INPUT_SIZE) {
    input = "";
  }

  for(auto it = newInput.begin(); it != newInput.end(); ++it) {
    if(*it != '\b') {
      input += *it;
    } else if(input.length() > 0) {
      input.erase(input.length() - 1);
    }
  }
}

