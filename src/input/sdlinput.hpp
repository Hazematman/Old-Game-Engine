#ifndef SDLINPUT_HPP
#define SDLINPUT_HPP
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <stdint.h>
#include <string>
#include <functional>

enum MouseButton {
  MOUSE_LEFT = 1,
  MOUSE_MIDDLE,
  MOUSE_RIGHT,
};

/* Class to handle input from an SDL2
 * window. Process keyboard and mouse input
 * including text input.
 */
class SDLInput {
  public:
    SDLInput();

    void processInput();
    void setGUILock(bool gui);
    bool getQuit();

    /* Set callback functions for when mouse
     * and keyboard events are generated.
     */
    void setKeyDownCallback(std::function<void(std::string&)> callback);
    void setKeyUpCallback(std::function<void(std::string&)> callback);
    void setMouseMovedCallback(std::function<void(int,int,int,int)> callback);
    void setMouseButtonUpCallback(std::function<void(MouseButton,int,int)> callback);
    void setMouseButtonDownCallback(std::function<void(MouseButton,int,int)> callback);
    
    /* Functions for checking input.
     * Can use guilock state for input locking when
     * gui lock is enabled.
     */
    bool isKeyDown(std::string &key, bool guiLocked);
    bool isMouseDown(MouseButton button, bool guiLocked);
    glm::vec2 getMousePos(bool guiLocked);
    void setMousePos(glm::vec2 pos, bool guiLocked);
    void setMouseRelative(bool relative);

    std::string getInput(bool guiLocked);
    void clearInput(bool guiLocked);
  private:
    uint8_t *keys;
    bool quit;
    bool focus;
    bool gui;
    std::string input;
    std::function<void(std::string&)> keyUpCallback;
    std::function<void(std::string&)> keyDownCallback;
    std::function<void(int,int,int,int)> mouseMovedCallback;
    std::function<void(MouseButton,int,int)> mouseButtonUpCallback;
    std::function<void(MouseButton,int,int)> mouseButtonDownCallback;

    void addInput(std::string newInput);
};

#endif
