#include "image.hpp"

Image::Image(int width, int height, uint8_t *data) :
  width(width), height(height), data(data)
{
}

int Image::getWidth() {
  return width;
}

int Image::getHeight() {
  return height;
}

uint8_t *Image::getData() {
  return data;
}
