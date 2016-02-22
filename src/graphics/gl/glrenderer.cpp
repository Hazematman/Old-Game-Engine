#include <iostream>
#include "gl_3_3.hpp"
#include "glrenderer.hpp"
#include "gltexture.hpp"
#include "../../error.hpp"
#include "../../helperfuncs.hpp"
using namespace std;

#define GL_ERROR_STRING "GL ERROR"

static uint8_t black[] = {0,0,0,255};
static Image blankImage(1, 1, &black[0]);

static const float quadVerts[] = {
  -1, -1,
   1, -1,
  -1,  1,
  -1,  1,
   1, -1,
   1,  1,
};

GLRenderer::GLRenderer() {
  if(!gl::sys::LoadFunctions()) {
    throw Error(GL_ERROR_STRING, "Could not load OpenGL functions");
  }

  if(gl::sys::IsVersionGEQ(3,3) == false) {
    throw Error(GL_ERROR_STRING, "OpenGL 3.3 not supported");
  }

  // Set inital GL settings
  gl::Enable(gl::BLEND);
  gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);
  gl::Enable(gl::DEPTH_TEST);
  gl::DepthFunc(gl::LESS);
  //gl::Enable(gl::CULL_FACE);
  //gl::CullFace(gl::BACK);
  gl::ClearColor(0.0, 0.0, 0.0, 1.0);

  // Create the blank texture
  blankTexture.reset(createTexture(blankImage));

  // Create shader objects
  guiShader.reset(new ShaderProgram("./data/shaders/guivertex.glsl", "./data/shaders/guifragment.glsl"));

  // Create quadVBO and VAO
  gl::GenVertexArrays(1, &quadVAO);
  gl::BindVertexArray(quadVAO);
  gl::GenBuffers(1, &quadVBO);
  gl::BindBuffer(gl::ARRAY_BUFFER, quadVBO);
  gl::BufferData(gl::ARRAY_BUFFER, sizeof(quadVerts), quadVerts, gl::STATIC_DRAW);
  gl::EnableVertexAttribArray(0);
  gl::VertexAttribPointer(
      0,
      2,
      gl::FLOAT,
      false,
      0,
      nullptr);

  guiUniforms.pos = guiShader->getUniform("pos");
  guiUniforms.scale = guiShader->getUniform("scale");
  guiUniforms.tex = guiShader->getUniform("tex");
  guiUniforms.colour = guiShader->getUniform("colour");

}

void GLRenderer::setRenderDimensions(int width, int height) {
  gl::Viewport(0, 0, width, height);
  dimensions.x = width;
  dimensions.y = height;
}

void GLRenderer::clearColour() {
  gl::Clear(gl::COLOR_BUFFER_BIT);
}

void GLRenderer::clearDepth() {
  gl::Clear(gl::DEPTH_BUFFER_BIT);
}

void GLRenderer::drawBox(Box &box) {
  if(currentProgram != guiShader->getID()) {
    gl::UseProgram(guiShader->getID());
    currentProgram = guiShader->getID();
  }
  glm::vec2 pos = box.getPosition();
  glm::vec2 size = box.getSize();
  glm::vec2 tPos = pos; // Translated position to OpenGL screen space
  glm::vec2 tScale;
  if(pos.x < 0) {
    tPos.x = dimensions.x + pos.x;
  }
  if(pos.y < 0) {
    tPos.y = dimensions.y + pos.y;
  }

  tScale.x = size.x/dimensions.x;
  tScale.y = size.y/dimensions.y;

  tPos.x = tPos.x/dimensions.x;
  tPos.y = tPos.y/dimensions.y;

  tPos.x = remap(tPos.x, 0, 1, -1, 1);
  tPos.y = -remap(tPos.y, 0, 1, -1, 1);

  gl::BindVertexArray(quadVAO);
  GLTexture *texture = static_cast<GLTexture*>(box.getTexture());
  gl::BindTexture(gl::TEXTURE_2D, texture->getId());
  gl::Uniform1i(guiUniforms.tex, 0);
  gl::Uniform2fv(guiUniforms.pos, 1, &tPos[0]);
  gl::Uniform2fv(guiUniforms.scale, 1, &tScale[0]);
  gl::Uniform4fv(guiUniforms.colour, 1, &box.getColour()[0]);

  gl::DrawArrays(gl::TRIANGLES, 0, 6);
}

Texture *GLRenderer::createTexture(Image &image) {
  GLuint textureId;
  gl::GenTextures(1, &textureId);
  gl::BindTexture(gl::TEXTURE_2D, textureId);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
  gl::TexImage2D(gl::TEXTURE_2D,
      0,
      gl::RGBA,
      image.getWidth(),
      image.getHeight(),
      0,
      gl::RGBA,
      gl::UNSIGNED_BYTE,
      image.getData());

  return new GLTexture(textureId);
}

