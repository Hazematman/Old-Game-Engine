#ifndef BOX_HPP
#define BOX_HPP
#include <glm/glm.hpp>
#include <memory>
#include "texture.hpp"

class Box {
  public:
    Box(std::shared_ptr<Texture> texture, glm::vec2 position, glm::vec2 size, glm::vec4 colour);

    void setPosition(glm::vec2 position);
    void setSize(glm::vec2 size);
    void setTexture(std::shared_ptr<Texture> texture);
    void setColour(glm::vec4 colour);

    Texture *getTexture();
    glm::vec2 getPosition();
    glm::vec2 getSize();
    glm::vec4 getColour();
  private:
    std::shared_ptr<Texture> texture;
    glm::vec2 position;
    glm::vec2 size;
    glm::vec4 colour;
};

#endif
