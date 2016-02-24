#ifndef GLRENDERER_HPP
#define GLRENDERER_HPP
#include "../renderer.hpp"
#include "glshader.hpp"

struct GuiUniforms {
  GLint pos,scale,tex,colour;
};

struct SdfUniforms {
  GLint pos,scale,sdfTexture,colour,spread,character,glyphScale;
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
    void drawString(Font &font, Text &text, glm::vec2 pos);
    void setRenderDimensions(int width, int height);
    void setDepthTest(bool doDepthTest);
    void clearColour();
    void clearDepth();
  private:
    void convertScreenDim(glm::vec2 &pos, glm::vec2 &scale);
    std::unique_ptr<ShaderProgram> guiShader;
    std::unique_ptr<ShaderProgram> sdfShader;
    glm::vec2 dimensions;
    GLuint currentProgram;
    GLuint quadVBO;
    GLuint quadVAO;
    GuiUniforms guiUniforms;
    SdfUniforms sdfUniforms;
};

#endif
