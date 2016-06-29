#ifndef COMPONENTS_HEADER
#define COMPONENTS_HEADER

#include <string>
#include "Particle.h"

struct Disk{
  std::vector<Particle*> P;
};

struct Halo{
  std::vector<Particle*> P;
};

struct Bulge{
  std::vector<Particle*> P;
};

#endif
