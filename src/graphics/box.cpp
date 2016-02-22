#include "box.hpp"
using namespace std;

Box::Box(shared_ptr<Texture> texture, glm::vec2 position, glm::vec2 size, glm::vec4 colour) :
  texture(texture), position(position), size(size), colour(colour)
{
}

void Box::setPosition(glm::vec2 position) {
  this->position = position;
}

void Box::setSize(glm::vec2 size){
  this->size = size;
}

void Box::setTexture(shared_ptr<Texture> texture) {
  this->texture = texture;
}

void Box::setColour(glm::vec4 colour) {
  this->colour = colour;
}

Texture *Box::getTexture() {
  return texture.get();
}

glm::vec2 Box::getPosition() {
  return position;
}

glm::vec2 Box::getSize() {
  return size;
}

glm::vec4 Box::getColour() {
  return colour;
}
