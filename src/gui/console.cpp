#include <iostream>
#include <sstream>
#include <algorithm>
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
  bgBox(renderer->getBlankTexture(), glm::vec2(0, (LINES)*(HEIGHT)), glm::vec2(WIDTH*GET_WIDTH(), HEIGHT), bgColour),
  cursor(renderer->getBlankTexture(), glm::vec2(0,0), glm::vec2(GET_WIDTH(), HEIGHT), textColour)
{
  inputPosition = 0;
  input->setKeyDownCallback(bind(&Console::handleKeyDown, this, placeholders::_1));
  input->setKeyInputCallback(bind(&Console::handleKeyInput, this, placeholders::_1));

  lines.push_back("");
}

void Console::handleKeyInput(const string &input) {
  textInputLeft += input;
  inputPosition += input.length();
}

void Console::handleKeyDown(const string &input) {
  if(input == "Left") {
    if(inputPosition > 0) {
      inputPosition -= 1; 
      textInputRight.insert(textInputRight.begin(), textInputLeft.back());
      textInputLeft.pop_back();
    }
  } else if(input == "Right") {
    if(textInputRight.length() > 0) {
      inputPosition += 1;
      textInputLeft.push_back(textInputRight.at(0));
      textInputRight.erase(0, 1);
    }
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
  for(size_t i=0; i < input.length(); ++i) {
    if(lines.back().length() > (WIDTH-1)) {
      lines.push_back("");
    }

    lines.back().push_back(input.at(i));
  }
  while(lines.size() > LINES) {
    lines.pop_front();
  }
}

void Console::draw() {
  Text text;
  text.colour = textColour;
  text.size = HEIGHT;

  cursor.setPosition(glm::vec2(min<int>(inputPosition, WIDTH-1)*GET_WIDTH(), LINES*HEIGHT));

  renderer->drawBox(fgBox);
  renderer->drawBox(bgBox);

  int i = 0;
  for(auto it=lines.begin(); it != lines.end(); ++it) {
    text.text = *it; 
    renderer->drawString(*font, text, glm::vec2(0,i*HEIGHT));
    ++i;
  }

  string combine = textInputLeft + textInputRight;
  if(combine.length() < WIDTH){
    text.text = combine;
  } else if(inputPosition == (int)combine.length()) {
    text.text = combine;
    text.text.erase(0, text.text.length() - (WIDTH-1));
  } else if(inputPosition < WIDTH) {
    text.text = textInputLeft;
    if(text.text.length() > (WIDTH-1)) {
      text.text.erase(0, text.text.length() - (WIDTH-1));
    } else {
      text.text = text.text.substr(text.text.length() - inputPosition, inputPosition) + textInputRight.substr(0, (WIDTH-1)-inputPosition);
    }
  }else {
    text.text = textInputLeft;
    if(text.text.length() > (WIDTH-1)) {
      text.text.erase(0, text.text.length() - (WIDTH-1));
    }
  }

  renderer->drawString(*font, text, glm::vec2(0, LINES*HEIGHT));
  renderer->drawBox(cursor);
}
