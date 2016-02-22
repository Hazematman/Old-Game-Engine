#include "renderer.hpp"
using namespace std;

// Required for pure virtual deconstructor
Renderer::~Renderer() {}

shared_ptr<Texture> Renderer::getBlankTexture() {
  return blankTexture;
}
