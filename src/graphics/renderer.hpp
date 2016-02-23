#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <glm/glm.hpp>
#include <memory>
#include "texture.hpp"
#include "image.hpp"
#include "box.hpp"
#include "font.hpp"

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
    virtual void drawBox(Box &box) = 0;
    virtual void drawString(Font &font, const std::string &text) = 0;
    virtual void setRenderDimensions(int width, int height) = 0;
    virtual void clearColour() = 0;
    virtual void clearDepth() = 0;
    std::shared_ptr<Texture> getBlankTexture();
  protected:
    std::shared_ptr<Texture> blankTexture;
};

#endif
