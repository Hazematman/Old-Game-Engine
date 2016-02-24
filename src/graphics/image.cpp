#include "image.hpp"

Image::Image(int width, int height, uint8_t *data, Format format) :
  width(width), height(height), data(data), format(format)
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

Image::Format Image::getFormat() {
  return format;
}
