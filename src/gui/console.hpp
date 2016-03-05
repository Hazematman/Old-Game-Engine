#ifndef CONSOLE_HPP
#define CONSOLE_HPP
#include <memory>
#include <list>
#include <functional>
#include <glm/glm.hpp>
#include "../graphics/renderer.hpp"
#include "../input/sdlinput.hpp"

class Console {
  public:
    Console(std::shared_ptr<Renderer> renderer, std::shared_ptr<SDLInput> input, std::shared_ptr<Font> font, 
        const glm::vec4 &bgColour, const glm::vec4 &fgColour, 
        const glm::vec4 &textColour);

    void draw();
    void handleKeyInput(const std::string &input);
    void handleKeyDown(const std::string &input);
    void print(const std::string &input);
    void setRunCallback(std::function<void(const std::string&)> callback);
  private:
    void insertLine(const std::string &input);
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Font> font;
    glm::vec4 bgColour;
    glm::vec4 fgColour;
    glm::vec4 textColour;
    std::string textInputLeft;
    std::string textInputRight;
    int inputPosition;
    std::list<std::string> lines;
    Box fgBox;
    Box bgBox;
    Box cursor;

    std::function<void(const std::string&)> runCallback;
};

#endif
