#ifndef GALAXY_HEADER
#define GALAXY_HEADER

#include "Compile_Time_Options.h"
#include "Components.h"
#include "Snapshot.h"
#include <vector>

class Galaxy{
 private:
  Disk myDisk;
  Bulge myBulge;
  Halo myHalo;
  int num;
  std::vector<std::vector<double> > diskMofI;

 public:
  // Constructor
  Galaxy(double x, double y, double z, double r, Snapshot* snap);
  // Destructor
  ~Galaxy();

  // Center on disk via shrinking sphere of centroids
  void CenterOnDiskCentroid();
  void AlignToMinorAxis();
  double ComputeVirialRatio();
  std::vector<double> GetDiskXs();
  std::vector<double> GetDiskYs();
  std::vector<double> GetDiskZs();
};

#endif
