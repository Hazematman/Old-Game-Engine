#include "gltexture.hpp"

GLTexture::GLTexture(GLuint id, int width, int height) : 
  Texture(width, height), id(id) 
{
}

GLuint GLTexture::getId() {
  return id;
}
