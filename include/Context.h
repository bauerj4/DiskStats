#ifndef CONTEXT_HEADER
#define CONTEXT_HEADER

#include <string>

struct Context{
  double Omega0;
  double OmegaB;
  double OmegaLambda;
  double HubbleParam;
  double DiskShrinkingSphereRad;
  double DiskShrinkingSphereCut;
  double DiskShrinkingSphereRed;
  double GalaxyX0;
  double GalaxyY0;
  double GalaxyZ0;
  double GalaxyR0;

  int ComovingIntegration;
  int PeriodicBoundaries;
  int NumSnaps;

  int HaloParticleType;
  int DiskParticleType;
  int BulgeParticleType;
  int MultipleGalaxies;

  std::string SnapshotBase;
  std::string GalaxyList;
};

#endif
