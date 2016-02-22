#ifndef BINDING_HPP
#define BINDING_HPP
#include <functional>
#include <string>
#include "wren.h"

char *loadModule(WrenVM *vm, const char *name);
void logText(WrenVM *vm, const char *text);

void setLogFunction(std::function<void(std::string)> callback);

#endif
