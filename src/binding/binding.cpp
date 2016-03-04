#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "binding.hpp"
#include "../error.hpp"
using namespace std;

#define LUA_ERROR "Lua Error"

static lua_State *lua;
static function<void(string)> logFunction;

static int binding_print(lua_State *l) {
  int n = lua_gettop(l);
  if(n < 1) {
    lua_pushliteral(l, "expected one argument");
    lua_error(l);
  }

  if(!lua_isstring(l, 1)) {
    lua_pushliteral(l, "expected string");
    lua_error(l);
  }
  logText(lua_tostring(l, 1));
  return 0;
}

const struct luaL_Reg core_functions[] = {
  {"print", binding_print},
  {NULL, NULL},
};

void bindingInit() {
  lua = luaL_newstate();

  lua_pushglobaltable(lua);
  luaL_setfuncs(lua, core_functions, 0);
  lua_pop(lua, 1);

  if(luaL_dofile(lua, "./data/scripts/main.lua")) {
    throw Error(LUA_ERROR, lua_tostring(lua, -1)); 
  }
}

void bindingFree() {
  lua_close(lua);
}

void logText(const char *text) {
  if(logFunction) {
    logFunction(text);
  }
}

void callInit() {
  lua_getglobal(lua, "init");
  if(lua_pcall(lua, 0, 0, 0)) {
    logText(lua_tostring(lua, -1));
    lua_pop(lua ,-1);
  }
}

void callRun(float dt) {
}

void setLogFunction(function<void(string)> callback) {
  logFunction = callback;
}

void runString(const string &code) {
  if(luaL_dostring(lua, code.c_str())) {
    logText(lua_tostring(lua, -1));
    lua_pop(lua, -1);
  }
}

void keyUpCallback(const string &key) {
}

void keyDownCallback(const string &key) {
}

void mouseUpCallback(MouseButton button, int x, int y) {
}

void mouseDownCallback(MouseButton button, int x, int y) {
}

void mouseMovedCallback(int x, int y, int dx, int dy) {
}

