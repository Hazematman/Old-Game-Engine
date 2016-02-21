#ifndef GLTEXTURE_HPP
#define GLTEXTURE_HPP
#include "gl_3_3.hpp"
#include "../texture.hpp"

/* OpenGL 3.3 specific texture
 */
class GLTexture : public Texture {
  public:
    GLTexture(GLuint id);

    GLuint getId();
  private:
    GLuint id;
};

#endif
