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
          case SDL_WINDOWEVENT_RESIZED:
            for(auto it=resizeCallback.begin(); it != resizeCallback.end(); ++it) {
              (*it)(e.window.data1, e.window.data2);
            }
            break;
        }
        break;
      case SDL_TEXTINPUT:
        for(auto it=keyInputCallback.begin(); it != keyInputCallback.end(); ++it) {
          (*it)(string(e.text.text));
        }
        addInput(string(e.text.text));
        break;
      case SDL_KEYDOWN:
        for(auto it=keyDownCallback.begin(); it != keyDownCallback.end(); ++it) {
          string key = SDL_GetKeyName(e.key.keysym.sym);
          (*it)(key);
        }
        break;
      case SDL_KEYUP:
        for(auto it=keyUpCallback.begin(); it != keyUpCallback.end(); ++it) {
          string key = SDL_GetKeyName(e.key.keysym.sym);
          (*it)(key);
        }
        break;
      case SDL_MOUSEMOTION:
        for(auto it=mouseMovedCallback.begin(); it != mouseMovedCallback.end(); ++it) {
          (*it)(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
        }
        break;
      case SDL_MOUSEBUTTONUP:
        for(auto it=mouseButtonUpCallback.begin(); it != mouseButtonUpCallback.end(); ++it) {
          (*it)((MouseButton)e.button.button, 
              e.button.x, e.button.y);
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        for(auto it=mouseButtonDownCallback.end(); it != mouseButtonDownCallback.end(); ++it) {
          (*it)((MouseButton)e.button.button,
              e.button.x, e.button.y);
        }
        break;
    }
  }
}

void SDLInput::setKeyDownCallback(function<void(const string&)> callback) {
  keyDownCallback.push_back(callback);
}

void SDLInput::setKeyUpCallback(function<void(const string&)> callback) {
  keyUpCallback.push_back(callback);
}

void SDLInput::setKeyInputCallback(function<void(const string&)> callback) {
  keyInputCallback.push_back(callback);
}

void SDLInput::setMouseMovedCallback(function<void(int,int,int,int)> callback) {
  mouseMovedCallback.push_back(callback);
}

void SDLInput::setMouseButtonUpCallback(function<void(MouseButton,int,int)> callback) {
  mouseButtonUpCallback.push_back(callback);
}

void SDLInput::setMouseButtonDownCallback(function<void(MouseButton,int,int)> callback) {
  mouseButtonDownCallback.push_back(callback);
}

void SDLInput::setResizeCallback(function<void(int,int)> callback) {
  resizeCallback.push_back(callback);
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

void SDLInput::addInput(const string &newInput) {
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

