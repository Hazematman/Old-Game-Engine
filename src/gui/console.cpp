#include <iostream>
#include <sstream>
#include "console.hpp"
using namespace std;

#define HEIGHT 12
#define LINES 24
#define WIDTH 80
#define GET_WIDTH() (font->getWidth()-font->getPadding()*2)*((float)HEIGHT/(font->getHeight()-font->getPadding()*2)) 

Console::Console(shared_ptr<Renderer> renderer, shared_ptr<SDLInput> input,shared_ptr<Font> font, 
    const glm::vec4 &bgColour, const glm::vec4 &fgColour, 
    const glm::vec4 &textColour) :
  renderer(renderer), font(font), bgColour(bgColour), fgColour(fgColour), textColour(textColour),
  fgBox(renderer->getBlankTexture(), glm::vec2(0,0), glm::vec2(WIDTH*GET_WIDTH(), LINES*(HEIGHT)), fgColour),
  bgBox(renderer->getBlankTexture(), glm::vec2(0, (LINES)*(HEIGHT)), glm::vec2(WIDTH*GET_WIDTH(), HEIGHT), bgColour)
{
  inputPosition = 0;
  input->setKeyDownCallback(bind(&Console::handleKeyDown, this, placeholders::_1));
  input->setKeyInputCallback(bind(&Console::handleKeyInput, this, placeholders::_1));
}

void Console::handleKeyInput(const string &input) {
  textInputLeft += input;
}

void Console::handleKeyDown(const string &input) {
  if(input == "Left") {
    inputPosition = inputPosition == 0 ? 0 : inputPosition-1; 
  } else if(input == "Right") {
    inputPosition += 1;
  } else if(input == "Backspace") {
    if(textInputLeft.length() > 0) {
      textInputLeft.erase(textInputLeft.length() - 1);
      inputPosition -= 1;
    }
  } else if(input == "Return") {
    string command = textInputLeft + textInputRight;
    inputPosition = 0;
    textInputLeft = "";
    textInputRight = "";
    if(runCallback) {
      runCallback(command);
    }
  }
}

void Console::setRunCallback(function<void(const string&)> callback) {
  runCallback = callback;
}

void Console::print(const string &input) {
  stringstream streamInput(input);
  string line;
  while(getline(streamInput, line)) {
    insertLine(line);
  }
}

void Console::insertLine(const string &input) {
  if(lines.size() < LINES) {
    lines.push_back(input);
  } else {
    lines.push_back(input);
    lines.pop_front();
  }
}

void Console::draw() {
  Text text;
  text.colour = textColour;
  text.size = HEIGHT;
  renderer->drawBox(fgBox);
  renderer->drawBox(bgBox);

  int i = 0;
  for(auto it=lines.begin(); it != lines.end(); ++it) {
    text.text = *it; 
    renderer->drawString(*font, text, glm::vec2(0,i*HEIGHT));
    ++i;
  }

  text.text = textInputLeft + textInputRight;
  if(text.text.length() > WIDTH) {
    text.text.erase(0, text.text.length() - WIDTH);
  }
  renderer->drawString(*font, text, glm::vec2(0, LINES*HEIGHT));
}
