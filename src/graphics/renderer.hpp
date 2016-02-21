#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "texture.hpp"
#include "image.hpp"

/* Abstract class to represent renderer object.
 * Handles the allocation of GPU-API specific
 * texture allocation. Also Handles rendering
 * 3D scenes of objects and lights.
 */
class Renderer {
  public:
    virtual ~Renderer() = 0;

    /* Allocate a texture object from image data
     */
    virtual Texture *createTexture(Image &image) = 0;
};

#endif