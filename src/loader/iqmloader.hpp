#ifndef IQMLOADER_HPP
#define IQMLOADER_HPP
#include <string>
#include <vector>
#include <stdint.h>
#include "iqm.hpp"

struct Extents {
  float minx;
  float maxx;
  float miny;
  float maxy;
  float minz;
  float maxz;
};

struct Vertex {
  float position[3];
  float normal[3];
  float tangent[4];
  float texcoord[2];
  uint8_t blendindex[4];
  uint8_t blendweight[4];
};

struct IQMModelData {
  std::vector<Vertex> verticies;
  std::vector<iqmtriangle> triangles;
  std::vector<iqmmesh> meshes;
  std::vector<iqmjoint> joints;
  std::vector<std::string> textures;
  std::vector<glm::mat4> baseFrame;
  std::vector<glm::mat4> inverseBaseFrame;
  std::vector<iqmanim> animations;
  std::vector<std::string> animationNames;
  std::vector<iqmpose> poses;
  std::vector<glm::mat4> frames;
  int numFrames;
};

bool loadIQM(const std::string &filename, IQMModelData &data);

#endif
