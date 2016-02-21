#include "gltexture.hpp"

GLTexture::GLTexture(GLuint id) : id(id) {}

GLuint GLTexture::getId() {
  return id;
}
