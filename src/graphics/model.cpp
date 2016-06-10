#include "model.hpp"

void Model::setModelData(ModelData *modelData) {
  this->modelData = modelData;
}

void Model::setPosition(glm::vec3 position) {
  this->position = position;
}

void Model::setScale(glm::vec3 scale) {
  this->scale = scale;
}

void Model::setRotation(glm::quat rotation) {
  this->rotation = rotation;
}

glm::vec3 Model::getPosition() {
  return position;
}

glm::vec3 Model::getScale() {
  return scale;
}

glm::quat Model::getRotation() {
  return rotation;
}
