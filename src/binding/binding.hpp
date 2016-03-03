#ifndef BINDING_HPP
#define BINDING_HPP
#include <functional>
#include <string>
#include "../input/sdlinput.hpp"
extern "C" {
#include "wren.h"
}

void bindingInit();
void bindingFree();
char *loadModule(WrenVM *vm, const char *name);
void logText(WrenVM *vm, const char *text);
WrenForeignMethodFn foreignMethod(WrenVM *vm, const char *module, const char *className, bool isStatic, const char *signature);
WrenForeignClassMethods foreignClass(WrenVM *vm, const char *module, const char *className);

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
