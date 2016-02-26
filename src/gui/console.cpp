#include <iostream>
#include "console.hpp"
using namespace std;

#define HEIGHT 12

Console::Console(shared_ptr<Renderer> renderer, shared_ptr<Font> font, 
    const glm::vec4 &bgColour, const glm::vec4 &fgColour, 
    const glm::vec4 &textColour) :
  renderer(renderer), font(font), bgColour(bgColour), fgColour(fgColour), textColour(textColour),
  fgBox(renderer->getBlankTexture(), glm::vec2(0,0), glm::vec2(60*10, 24*(HEIGHT)), fgColour),
  bgBox(renderer->getBlankTexture(), glm::vec2(0, 24*(HEIGHT)), glm::vec2(60*10, HEIGHT), bgColour)
{
  text = "";
  for(char i = '!'; i < 'z'; ++i) {
    text+=i;
  }
}

void Console::draw() {
  Text text;
  text.colour = textColour;
  text.size = HEIGHT;
  text.text = this->text;
  renderer->drawBox(fgBox);
  renderer->drawBox(bgBox);
  for(int i=0; i < 25; ++i)
    renderer->drawString(*font, text, glm::vec2(0, i*(HEIGHT)));
}

