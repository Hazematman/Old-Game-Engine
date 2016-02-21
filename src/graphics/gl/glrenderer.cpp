#include "gl_3_3.hpp"
#include "glrenderer.hpp"
#include "gltexture.hpp"
#include "../../error.hpp"

#define GL_ERROR_STRING "GL ERROR"

GLRenderer::GLRenderer() {
  if(gl::sys::LoadFunctions()) {
    throw Error(GL_ERROR_STRING, "Could not load OpenGL functions");
  }

  if(gl::sys::IsVersionGEQ(3,3) == false) {
    throw Error(GL_ERROR_STRING, "OpenGL 3.3 not supported");
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
      gl::RGBA,
      image.getWidth(),
      image.getHeight(),
      0,
      gl::RGBA,
      gl::UNSIGNED_BYTE,
      image.getData());

  return new GLTexture(textureId);
}

