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
  std::vector<double> haloAngularMomentum;
  std::vector<double> bulgeAngularMomentum;
  std::vector<double> diskAngularMomentum;
  std::vector<double> diskInnerAngularMomentum;
  std::vector<double> diskAngularMomentumAxis;
  std::vector<std::vector<double> > diskMofI;

 public:
  // Constructor
  Galaxy(double x, double y, double z, double r, Snapshot* snap);
  // Destructor
  ~Galaxy();

  // Center on disk via shrinking sphere of centroids
  void CenterOnDiskCentroid();
  void CenterOnHaloCentroid();
  void AlignToMinorAxis();
  void ComputeAngularMomentum();
  void RemoveUnboundHaloParticles(double vxBar, double vyBar, double vzBar,double Rs);
  void PrintHaloIDs(char * file);
  double ComputeVirialRatio();
  std::vector<double> GetHaloXs();
  std::vector<double> GetHaloYs();
  std::vector<double> GetHaloZs();
  std::vector<double> GetDiskXs();
  std::vector<double> GetDiskYs();
  std::vector<double> GetDiskZs();
  std::vector<double> GetDiskVXs();
  std::vector<double> GetDiskVYs();
  std::vector<double> GetDiskVZs();
  std::vector<double> GetDiskVRs();
  std::vector<double> GetDiskVPhis();
  std::vector<double> GetDiskCylRs();
  std::vector<double> GetDiskCylPhis();
  std::vector<double> GetDiskEnergies();
  std::vector<double> GetDiskLzs();
  std::vector<double> GetAngularMomentumAxis(){return diskAngularMomentumAxis;}
  std::vector<std::vector<double> > GetDiskMofITensor();
};

#endif
