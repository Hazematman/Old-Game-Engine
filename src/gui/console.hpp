#ifndef CONSOLE_HPP
#define CONSOLE_HPP
#include <memory>
#include <glm/glm.hpp>
#include "../graphics/renderer.hpp"

class Console {
  public:
    Console(std::shared_ptr<Renderer> renderer, std::shared_ptr<Font> font, 
        const glm::vec4 &bgColour, const glm::vec4 &fgColour, 
        const glm::vec4 &textColour);

    void draw();
  private:
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Font> font;
    glm::vec4 bgColour;
    glm::vec4 fgColour;
    glm::vec4 textColour;
    std::string text;
    Box fgBox;
    Box bgBox;
};

#endif
