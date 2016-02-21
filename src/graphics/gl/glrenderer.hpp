#ifndef GLRENDERER_HPP
#define GLRENDERER_HPP
#include "../renderer.hpp"

class GLRenderer : public Renderer {
  public:
    GLRenderer();

    /* Convert an RGBA image into an
     * OpenGL 3.3 texture with linear
     * filtering.
     */
    Texture *createTexture(Image &image);
    void drawBox(Box &box);
  private:
};

#endif
