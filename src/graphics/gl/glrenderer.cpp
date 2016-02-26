#include <iostream>
#include "gl_3_3.hpp"
#include "glrenderer.hpp"
#include "gltexture.hpp"
#include "../../error.hpp"
#include "../../helperfuncs.hpp"
using namespace std;

#define GL_ERROR_STRING "GL ERROR"

static uint8_t black[] = {0,0,0};
static Image blankImage(1, 1, &black[0], Image::Format::RGB);
static GLenum formats[] = {
  gl::RGBA,
  gl::RGB,
  gl::RED,
};

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
  gl::DepthFunc(gl::LESS);
  //gl::Enable(gl::CULL_FACE);
  //gl::CullFace(gl::BACK);
  gl::ClearColor(0.0, 0.0, 0.0, 1.0);

  // Create the blank texture
  blankTexture.reset(createTexture(blankImage));

  // Create shader objects
  guiShader.reset(new ShaderProgram("./data/shaders/guivertex.glsl", "./data/shaders/guifragment.glsl"));
  sdfShader.reset(new ShaderProgram("./data/shaders/sdftextvertex.glsl", "./data/shaders/sdftextfragment.glsl"));

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

  gl::UseProgram(sdfShader->getID());

  sdfUniforms.pos = sdfShader->getUniform("pos");
  sdfUniforms.scale = sdfShader->getUniform("scale");
  sdfUniforms.colour = sdfShader->getUniform("colour");
  sdfUniforms.spread = sdfShader->getUniform("spread");
  sdfUniforms.sdfTexture = sdfShader->getUniform("sdfTexture");
  sdfUniforms.character = sdfShader->getUniform("character");
  sdfUniforms.glyphScale = sdfShader->getUniform("glyphScale");
  sdfUniforms.scaleFactor = sdfShader->getUniform("scaleFactor");

}

void GLRenderer::setRenderDimensions(int width, int height) {
  gl::Viewport(0, 0, width, height);
  dimensions.x = width;
  dimensions.y = height;
}

void GLRenderer::setDepthTest(bool doDepthTest) {
  if(doDepthTest) {
    gl::Enable(gl::DEPTH_TEST);
  } else {
    gl::Disable(gl::DEPTH_TEST);
  }
}

void GLRenderer::clearColour() {
  gl::Clear(gl::COLOR_BUFFER_BIT);
}

void GLRenderer::clearDepth() {
  gl::Clear(gl::DEPTH_BUFFER_BIT);
}


void GLRenderer::convertScreenDim(glm::vec2 &pos, glm::vec2 &scale) {
  /*if(pos.x < 0) {
    pos.x = dimensions.x + pos.x;
  }
  if(pos.y < 0) {
    pos.y = dimensions.y + pos.y;
  }*/

  scale.x = scale.x/dimensions.x;
  scale.y = scale.y/dimensions.y;

  pos.x = pos.x/dimensions.x;
  pos.y = pos.y/dimensions.y;

  pos.x = remap(pos.x, 0, 1, -1, 1) + scale.x;
  pos.y = -(remap(pos.y, 0, 1, -1, 1) + scale.y);
}

void GLRenderer::drawBox(Box &box) {
  if(currentProgram != guiShader->getID()) {
    gl::UseProgram(guiShader->getID());
    currentProgram = guiShader->getID();
  }
  glm::vec2 pos = box.getPosition();
  glm::vec2 size = box.getSize();
  glm::vec2 tPos = pos; // Translated position to OpenGL screen space
  glm::vec2 tScale = size;

  convertScreenDim(tPos, tScale);
  
  gl::BindVertexArray(quadVAO);
  GLTexture *texture = static_cast<GLTexture*>(box.getTexture());
  gl::BindTexture(gl::TEXTURE_2D, texture->getId());
  gl::Uniform1i(guiUniforms.tex, 0);
  gl::Uniform2fv(guiUniforms.pos, 1, &tPos[0]);
  gl::Uniform2fv(guiUniforms.scale, 1, &tScale[0]);
  gl::Uniform4fv(guiUniforms.colour, 1, &box.getColour()[0]);

  gl::DrawArrays(gl::TRIANGLES, 0, 6);
}

void GLRenderer::drawString(Font &font, Text &text, glm::vec2 pos) {
  if(currentProgram != sdfShader->getID()) {
    gl::UseProgram(sdfShader->getID());
    currentProgram = sdfShader->getID();
  }
  gl::BindVertexArray(quadVAO);

  int glyphWidth = font.getWidth();
  int glyphHeight = font.getHeight();
  int padding = font.getPadding();

  int charWidth = glyphWidth - font.getPadding()*2;
  int charHeight = glyphHeight - font.getPadding()*2;
  float scaleFactor = (float) text.size / charHeight;

  GLTexture *texture = static_cast<GLTexture*>(font.getTexture().get());
  glm::vec2 charScale((float)glyphWidth/texture->getWidth(), (float)glyphHeight/texture->getHeight());

  gl::BindTexture(gl::TEXTURE_2D, texture->getId());
  gl::Uniform1i(sdfUniforms.sdfTexture, 0);
  gl::Uniform1f(sdfUniforms.spread, font.getSpread());
  gl::Uniform1f(sdfUniforms.scaleFactor, scaleFactor);
  gl::Uniform2fv(sdfUniforms.glyphScale, 1, &charScale[0]);
  gl::Uniform4fv(sdfUniforms.colour, 1, &text.colour[0]);

  for(size_t i=0; i < text.text.size(); ++i) {
    char c = text.text.at(i) - '!';
    if(c < 0) {
      continue;
    }
    glm::vec2 tPos = pos - glm::vec2(padding*scaleFactor)+ glm::vec2(charWidth*i*scaleFactor,0);
    glm::vec2 tScale(glyphWidth*scaleFactor, glyphHeight*scaleFactor);

    convertScreenDim(tPos, tScale);

    gl::Uniform1f(sdfUniforms.character, c);
    gl::Uniform2fv(sdfUniforms.pos, 1, &tPos[0]);
    gl::Uniform2fv(sdfUniforms.scale, 1, &tScale[0]);

    gl::DrawArrays(gl::TRIANGLES, 0, 6);
  }
}

Texture *GLRenderer::createTexture(Image &image) {
  GLuint textureId;
  gl::GenTextures(1, &textureId);
  gl::BindTexture(gl::TEXTURE_2D, textureId);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
  gl::TexImage2D(gl::TEXTURE_2D,
      0,
      formats[image.getFormat()],
      image.getWidth(),
      image.getHeight(),
      0,
      formats[image.getFormat()],
      gl::UNSIGNED_BYTE,
      image.getData());

  return new GLTexture(textureId, image.getWidth(), image.getHeight());
}

