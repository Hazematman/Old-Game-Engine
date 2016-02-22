#include "gl_3_3.hpp"
#include "glrenderer.hpp"
#include "gltexture.hpp"
#include "../../error.hpp"

#define GL_ERROR_STRING "GL ERROR"

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
  gl::Enable(gl::CULL_FACE);
  gl::CullFace(gl::BACK);
  gl::ClearColor(0.0, 0.0, 0.0, 1.0);

  //Create shader objects
  guiShader.reset(new ShaderProgram("./data/shaders/guivertex.glsl", "./data/shaders/guifragment.glsl"));
}

void GLRenderer::setRenderDimensions(int width, int height) {
  gl::Viewport(0, 0, width, height);
}

void GLRenderer::drawBox(Box &box) {
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

