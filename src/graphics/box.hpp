#ifndef BOX_HPP
#define BOX_HPP
#include <glm/glm.hpp>
#include <memory>
#include "texture.hpp"

class Box {
  public:
    Box(std::shared_ptr<Texture> texture, glm::vec2 position, glm::vec2 size);

    void setPosition(glm::vec2 position);
    void setSize(glm::vec2 size);
    void setTexture(std::shared_ptr<Texture> texture);
  private:
    std::shared_ptr<Texture> texture;
    glm::vec2 position;
    glm::vec2 size;
};

#endif
