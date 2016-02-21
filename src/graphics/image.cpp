#include "image.hpp"

int Image::getWidth() {
  return width;
}

int Image::getHeight() {
  return height;
}

uint8_t *Image::getData() {
  return data;
}
