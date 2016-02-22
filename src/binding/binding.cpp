#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "binding.hpp"
using namespace std;

static function<void(string)> logFunction;

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

char *loadModule(WrenVM *vm, const char *name) {
  return loadFile(name);
}

void logText(WrenVM *vm, const char *text) {
  cout << text;
  if(logFunction) {
    logFunction(text);
  }
}

void setLogFunction(std::function<void(std::string)> callback) {
  logFunction = callback;
}

