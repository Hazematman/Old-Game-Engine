#ifndef IMAGE_HPP
#define IMAGE_HPP
#include <stdint.h>

/* An image represents image data stored
 * in CPU memory before it has been passed
 * to the GPU. Images are stored as RGBA8
 * textures in memory
 */
class Image {
  public:
    enum Format {
      RGBA,
      RGB,
      MONO,
    };
    Image(int width, int height, uint8_t *data, Format format);
    int getWidth();
    int getHeight();
    uint8_t *getData();
    Format getFormat();
  private:
    int width;
    int height;
    uint8_t *data;
    Format format;
};

#endif
