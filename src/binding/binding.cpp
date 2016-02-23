#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "binding.hpp"
#include "../error.hpp"
using namespace std;

#define WREN_ERROR "Wren Error"

struct Game {
  WrenValue *game;
  WrenValue *init;
  WrenValue *run;
  WrenValue *onKeyDown;
  WrenValue *onKeyUp;
  WrenValue *onMouseButtonDown;
  WrenValue *onMouseButtonUp;
  WrenValue *onMouseMoved;
};

static WrenVM *vm;
static function<void(string)> logFunction;
static Game game = {0};

static WrenValue *getValue(WrenVM *vm, const char *module, const char *name) {
  wrenEnsureSlots(vm, 1);
  wrenGetVariable(vm, module, name, 0);
  if(wrenGetSlotType(vm,0) != WREN_TYPE_UNKNOWN) {
    return nullptr;
  } else {
    return wrenGetSlotValue(vm, 0);
  }
}

static char *loadFile(const char *name) {
  string path = "./data/scripts/" + string(name) + ".wren";
  FILE *f = fopen(path.c_str(), "r");
  if(f == nullptr) {
    return nullptr;
  }
  fseek(f,0,SEEK_END);
  long fileSize = ftell(f);
  fseek(f,0,SEEK_SET);

  char *data = (char*)malloc(fileSize+1);
  fread(data,fileSize,1,f);
  fclose(f);

  data[fileSize] = 0;

  return data;
}

void bindingInit() {
  WrenConfiguration config;
  wrenInitConfiguration(&config);

  config.loadModuleFn = loadModule;
  config.writeFn = logText;
  config.bindForeignMethodFn = foreignMethod;
  config.bindForeignClassFn = foreignClass;
  vm = wrenNewVM(&config);

  wrenInterpret(vm, "import \"core\"");

  game.game = getValue(vm, "game", "Game");

  game.init = wrenMakeCallHandle(vm, "init()");
  game.run = wrenMakeCallHandle(vm, "run(_)");
  game.onKeyUp = wrenMakeCallHandle(vm, "onKeyDown(_)");
  game.onKeyDown = wrenMakeCallHandle(vm, "onKeyDown(_)");
  game.onMouseButtonUp = wrenMakeCallHandle(vm, "onMouseButtonUp(_)");
  game.onMouseButtonDown = wrenMakeCallHandle(vm, "onMouseButtonDown(_)");
  game.onMouseMoved = wrenMakeCallHandle(vm, "onMouseMoved(_,_,_,_)");
}

void bindingFree() {
  if(game.run != nullptr) {
    wrenReleaseValue(vm, game.run);
  }
  if(game.init != nullptr) {
    wrenReleaseValue(vm, game.init);
  }

  wrenFreeVM(vm);
}

char *loadModule(WrenVM *vm, const char *name) {
  return loadFile(name);
}

WrenForeignMethodFn foreignMethod(WrenVM *vm, const char *module, const char *className, bool isStatic, const char *signature) {
  cout << "className" << endl;
  return nullptr; 
}

WrenForeignClassMethods foreignClass(WrenVM *vm, const char *module, const char *className) {
  WrenForeignClassMethods methods = {NULL, NULL};
  cout << module << " " << className << endl;
  return methods;
}

void logText(WrenVM *vm, const char *text) {
  cout << text;
  if(logFunction) {
    logFunction(text);
  }
}

void callInit() {
  wrenEnsureSlots(vm, 1);
  wrenSetSlotValue(vm, 0, game.game);
  wrenCall(vm, game.init);
}

void callRun(float dt) {
  wrenEnsureSlots(vm, 2);
  wrenSetSlotValue(vm, 0, game.game);
  wrenSetSlotDouble(vm, 1, dt);
  wrenCall(vm, game.run);
}

void setLogFunction(std::function<void(std::string)> callback) {
  logFunction = callback;
}

void keyUpCallback(std::string &key) {
  wrenEnsureSlots(vm, 2);
  wrenSetSlotValue(vm, 0, game.game);
  wrenSetSlotString(vm, 1, key.c_str());
  wrenCall(vm, game.onKeyUp);
}

void keyDownCallback(std::string &key) {
  wrenEnsureSlots(vm, 2);
  wrenSetSlotValue(vm, 0, game.game);
  wrenSetSlotString(vm, 1, key.c_str());
  wrenCall(vm, game.onKeyDown);
}

void mouseUpCallback(MouseButton button, int x, int y) {
  wrenEnsureSlots(vm, 4);
  wrenSetSlotValue(vm, 0, game.game);
  wrenSetSlotDouble(vm, 1, button);
  wrenSetSlotDouble(vm, 2, x);
  wrenSetSlotDouble(vm, 3, y);
  wrenCall(vm, game.onMouseButtonUp);
}

void mouseDownCallback(MouseButton button, int x, int y) {
  wrenEnsureSlots(vm, 4);
  wrenSetSlotValue(vm, 0, game.game);
  wrenSetSlotDouble(vm, 1, button);
  wrenSetSlotDouble(vm, 2, x);
  wrenSetSlotDouble(vm, 3, y);
  wrenCall(vm, game.onMouseButtonDown);
}

void mouseMovedCallback(int x, int y, int dx, int dy) {
  wrenEnsureSlots(vm, 5);
  wrenSetSlotValue(vm, 0, game.game);
  wrenSetSlotDouble(vm, 1, x);
  wrenSetSlotDouble(vm, 2, y);
  wrenSetSlotDouble(vm, 3, dx);
  wrenSetSlotDouble(vm, 4, dy);
  wrenCall(vm, game.onMouseMoved);
}

