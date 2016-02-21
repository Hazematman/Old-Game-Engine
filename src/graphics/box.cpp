#include "box.hpp"
using namespace std;

Box::Box(shared_ptr<Texture> texture, glm::vec2 position, glm::vec2 size) :
  texture(texture), position(position), size(size)
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
