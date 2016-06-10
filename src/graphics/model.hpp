#ifndef MODEL_HPP
#define MODEL_HPP
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "modeldata.hpp"

class Model {
  public:
    void setModelData(ModelData *modelData);
    void setPosition(glm::vec3 position);
    void setScale(glm::vec3 scale);
    void setRotation(glm::quat rotation);

    glm::vec3 getPosition();
    glm::vec3 getScale();
    glm::quat getRotation();
  private:
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
    
    ModelData *modelData;
};

#endif
