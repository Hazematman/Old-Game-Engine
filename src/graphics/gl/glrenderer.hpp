#ifndef GLRENDERER_HPP
#define GLRENDERER_HPP
#include "../renderer.hpp"
#include "glshader.hpp"

struct GuiUniforms {
  GLint pos,scale,tex,colour;
};

class GLRenderer : public Renderer {
  public:
    GLRenderer();

    /* Convert an RGBA image into an
     * OpenGL 3.3 texture with linear
     * filtering.
     */
    Texture *createTexture(Image &image);
    void drawBox(Box &box);
    void drawString(Font &font, const std::string &text);
    void setRenderDimensions(int width, int height);
    void clearColour();
    void clearDepth();
  private:
    std::unique_ptr<ShaderProgram> guiShader;
    glm::vec2 dimensions;
    GLuint currentProgram;
    GLuint quadVBO;
    GLuint quadVAO;
    GuiUniforms guiUniforms;
};

#endif
