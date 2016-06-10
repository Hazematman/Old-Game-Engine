#include <fstream>
#include <SDL2/SDL_image.h>
#include "loader.hpp"
#include "../graphics/image.hpp"
#include "../error.hpp"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

#define LOADER_ERROR "Loader Error"

#define BASE_DIR "./data/"
#define TEXTURE_DIR "textures/"
#define FONT_DIR "fonts/"
#define MODEL_DIR "models/"

Loader::Loader(shared_ptr<Renderer> renderer) : renderer(renderer) {}

shared_ptr<Texture> Loader::loadTexture(const string &name) {
  string file = BASE_DIR TEXTURE_DIR + name;
  auto it = textures.find(file);
  if(it != textures.end()) {
    return it->second;
  }

  SDL_Surface *sdlimage = IMG_Load(file.c_str());
  if(!sdlimage) {
    throw Error(LOADER_ERROR, "Could not load image: "+name);
  }

  // Get texture format
  // TODO support different endianness
  Image::Format tformat = Image::Format::RGBA;
  switch(sdlimage->format->BytesPerPixel){
    // If 4 colours or RGBA most likely
    case 4:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        tformat = Image::Format::RGBA;
      else
        tformat = Image::Format::RGBA;
      break;
    // If 3 colours of RGB most likely
    case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        tformat = Image::Format::RGB;
      else
        tformat = Image::Format::RGB;
      break;
  }

  Image image(sdlimage->w, sdlimage->h, (uint8_t*)sdlimage->pixels, tformat);
  shared_ptr<Texture> texture(renderer->createTexture(image)); 

  textures[name] = texture;
  SDL_FreeSurface(sdlimage);
  return texture;
}

shared_ptr<Font> Loader::loadFont(const string &name) {
  string file = BASE_DIR FONT_DIR + name;
  auto it = fonts.find(file);
  if(it != fonts.end()) {
    return it->second;
  }

  json fontData; 
  ifstream fontFile(file);
  if(fontFile.is_open() == false) {
    throw Error(LOADER_ERROR, "Could not open file "+name);
  }

  int width = 0;
  int height = 0;
  shared_ptr<Texture> texture;
  float spread;
  int padding;
  try {
    fontFile >> fontData;
    width = fontData["charWidth"];
    height = fontData["charHeight"];
    texture = loadTexture(fontData["texture"]);
    spread = fontData["spread"];
    padding = fontData["padding"];
  } catch(domain_error error) {
    throw Error(LOADER_ERROR, name+" is a bad font");
  }
  shared_ptr<Font> font(new Font(width, height, texture, spread, padding));
  
  fonts[name] = font;
  return font;
}

shared_ptr<ModelData> Loader::loadModelData(const std::string &name) {
  string file = BASE_DIR MODEL_DIR + name;
  auto it = models.find(file);
  if(it != models.end()) {
    return it->second;
  }
}
