#ifndef LOADER_HPP
#define LOADER_HPP
#include <memory>
#include <unordered_map>
#include "../graphics/texture.hpp"
#include "../graphics/renderer.hpp"

class Loader {
  public:
    Loader(std::shared_ptr<Renderer> renderer);
    std::shared_ptr<Texture> loadTexture(const std::string &name);
    std::shared_ptr<Font> loadFont(const std::string &name);
  private:
    std::shared_ptr<Renderer> renderer;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    std::unordered_map<std::string, std::shared_ptr<Font>> fonts;
};

#endif
