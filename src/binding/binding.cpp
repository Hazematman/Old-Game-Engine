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

  /*
  game.init = wrenMakeCallHandle(vm, "init()");
  game.run = wrenMakeCallHandle(vm, "run(_)");
  game.onKeyUp = wrenMakeCallHandle(vm, "onKeyDown(_)");
  game.onKeyDown = wrenMakeCallHandle(vm, "onKeyDown(_)");
  game.onMouseButtonUp = wrenMakeCallHandle(vm, "onMouseButtonUp(_)");
  game.onMouseButtonDown = wrenMakeCallHandle(vm, "onMouseButtonDown(_)");
  game.onMouseMoved = wrenMakeCallHandle(vm, "onMouseMoved(_,_,_,_)");
  */
}

void bindingFree() {
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
  game.game = getValue(vm, "game", "Game");
  game.init = wrenMakeCallHandle(vm, "init()");
  wrenEnsureSlots(vm, 1);
  wrenSetSlotValue(vm, 0, game.game);
  wrenCall(vm, game.init);
  wrenReleaseValue(vm, game.game);
  wrenReleaseValue(vm, game.init);
}

void callRun(float dt) {
  game.game = getValue(vm, "game", "Game");
  game.run = wrenMakeCallHandle(vm, "run(_)");
  wrenEnsureSlots(vm, 2);
  wrenSetSlotValue(vm, 0, game.game);
  wrenSetSlotDouble(vm, 1, dt);
  wrenCall(vm, game.run);
  wrenReleaseValue(vm, game.game);
  wrenReleaseValue(vm, game.run);
}

void setLogFunction(function<void(string)> callback) {
  logFunction = callback;
}

void runString(const string &code) {
  string newCode = "import \"game\" for Game\n" + code;
  wrenInterpret(vm, newCode.c_str());
}

void keyUpCallback(const string &key) {
  game.game = getValue(vm, "game", "Game");
  game.onKeyUp = wrenMakeCallHandle(vm, "onKeyUp(_)");
  wrenEnsureSlots(vm, 2);
  wrenSetSlotValue(vm, 0, game.game);
  wrenSetSlotString(vm, 1, key.c_str());
  wrenCall(vm, game.onKeyUp);
  wrenReleaseValue(vm, game.game);
  wrenReleaseValue(vm, game.onKeyUp);
}

void keyDownCallback(const string &key) {
  if(key == "Return") {
    cout << key << endl;
  }
  game.game = getValue(vm, "game", "Game");
  game.onKeyDown = wrenMakeCallHandle(vm, "onKeyDown(_)");
  wrenEnsureSlots(vm, 2);
  wrenSetSlotValue(vm, 0, game.game);
  wrenSetSlotString(vm, 1, key.c_str());
  wrenCall(vm, game.onKeyDown);
  wrenReleaseValue(vm, game.game);
  wrenReleaseValue(vm, game.onKeyDown);
}

void mouseUpCallback(MouseButton button, int x, int y) {
  game.game = getValue(vm, "game", "Game");
  game.onMouseButtonUp = wrenMakeCallHandle(vm, "onMouseButtonUp(_,_,_)");
  wrenEnsureSlots(vm, 4);
  wrenSetSlotValue(vm, 0, game.game);
  wrenSetSlotDouble(vm, 1, button);
  wrenSetSlotDouble(vm, 2, x);
  wrenSetSlotDouble(vm, 3, y);
  wrenCall(vm, game.onMouseButtonUp);
  wrenReleaseValue(vm, game.game);
  wrenReleaseValue(vm, game.onMouseButtonUp);
}

void mouseDownCallback(MouseButton button, int x, int y) {
  game.game = getValue(vm, "game", "Game");
  game.onMouseButtonDown = wrenMakeCallHandle(vm, "onMouseButtonDown(_,_,_)");
  wrenEnsureSlots(vm, 4);
  wrenSetSlotValue(vm, 0, game.game);
  wrenSetSlotDouble(vm, 1, button);
  wrenSetSlotDouble(vm, 2, x);
  wrenSetSlotDouble(vm, 3, y);
  wrenCall(vm, game.onMouseButtonDown);
  wrenReleaseValue(vm, game.game);
  wrenReleaseValue(vm, game.onMouseButtonDown);
}

void mouseMovedCallback(int x, int y, int dx, int dy) {
  game.game = getValue(vm, "game", "Game");
  game.onMouseMoved = wrenMakeCallHandle(vm, "onMouseMoved(_,_,_,_)");
  wrenEnsureSlots(vm, 5);
  wrenSetSlotValue(vm, 0, game.game);
  wrenSetSlotDouble(vm, 1, x);
  wrenSetSlotDouble(vm, 2, y);
  wrenSetSlotDouble(vm, 3, dx);
  wrenSetSlotDouble(vm, 4, dy);
  wrenCall(vm, game.onMouseMoved);
  wrenReleaseValue(vm, game.game);
  wrenReleaseValue(vm, game.onMouseMoved);
}

