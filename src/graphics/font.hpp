#ifndef FONT_HPP
#define FONT_HPP
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "texture.hpp"

class Font {
  public:
    Font(int width, int height, std::shared_ptr<Texture> texture, float spread, int padding);

    int getWidth();
    int getHeight();
    std::shared_ptr<Texture> getTexture();
    float getSpread();
    int getPadding();
  private:
    int width;
    int height;
    std::shared_ptr<Texture> texture;
    float spread;
    int padding;
};

struct Text {
  std::string text;
  glm::vec4 colour;
  float size;
};

#endif
