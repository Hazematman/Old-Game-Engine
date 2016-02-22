#include "helperfuncs.hpp"

float remap(float value, float oldmin, float oldmax, float newmin, float newmax) {
  float oldRange = oldmax - oldmin;
  float newRange = newmax - newmin;
  return (((value-oldmin)*newRange)/oldRange)+newmin;
}
