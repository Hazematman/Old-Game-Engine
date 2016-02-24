#ifndef TEXTURE_HPP
#define TEXTURE_HPP

/* Abstract class to represent texture object.
 * Textures are a GPU-API specific handle for
 * image data. The renderer should implement
 * it's own texture object representation
 */
class Texture {
  public:
    Texture(int width, int height);
    virtual ~Texture() = 0;

    int getWidth();
    int getHeight();
  protected:
    int width,height;
};

#endif
