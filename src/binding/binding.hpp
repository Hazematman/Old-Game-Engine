#ifndef BINDING_HPP
#define BINDING_HPP
#include <functional>
#include <string>
#include "../input/sdlinput.hpp"
#include "lua.hpp"

void bindingInit();
void bindingFree();
void logText(const char *text);

void callInit();
void callRun(float dt);

void setLogFunction(std::function<void(std::string)> callback);
void runString(const std::string &code);

void keyUpCallback(const std::string &key);
void keyDownCallback(const std::string &key);
void mouseUpCallback(MouseButton button, int x, int y);
void mouseDownCallback(MouseButton button, int x, int y);
void mouseMovedCallback(int x, int y, int dx, int dy);

#endif
