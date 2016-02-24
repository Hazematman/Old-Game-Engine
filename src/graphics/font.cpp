#include "font.hpp"
using namespace std;

Font::Font(int width, int height, shared_ptr<Texture> texture, float spread, int padding) :
  width(width), height(height), texture(texture), spread(spread), padding(padding)
{
}

int Font::getWidth() {
  return width;
}

int Font::getHeight() {
  return height;
}

shared_ptr<Texture> Font::getTexture() {
  return texture;
}

float Font::getSpread() {
  return spread;
}

int Font::getPadding() {
  return padding;
}
