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
  private:
    int width;
    int height;
    uint8_t *data;
};

#endif
