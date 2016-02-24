#include "texture.hpp"

Texture::Texture(int width, int height) :
  width(width), height(height)
{
}

// Required for pure virtual deconstructor
Texture::~Texture() {}

int Texture::getWidth() {
  return width;
}

int Texture::getHeight() {
  return height;
}
