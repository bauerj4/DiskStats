#include "../include/Compile_Time_Options.h"
#include "../include/Context.h"
#include "../include/Snapshot.h"
#include "../include/Galaxy.h"
#include "../include/Histogram.h"
#include <vector>

namespace Global{

  /*
    Global variables
  */

  int snapNum;
  int posCounter;

  double xBar;
  double yBar;
  double zBar;

  Context context;

  Snapshot * newSnap;

  Galaxy * thisGalaxy;

  std::string snapName;

  std::vector<double> timeArr;
  std::vector<double> sbins;
  std::vector<double> zbins;
  std::vector<double> ybins;
  std::vector<double> xbins;
  std::vector<double> vzbins;
  std::vector<double> vybins;
  std::vector<double> vxbins;
  std::vector<double> vpbins;
  std::vector<double> vrbins;

  std::vector<double> surfaceDensityWeights;

#ifdef DENSITY_HISTOGRAMS
  Histogram2D<double> * diskXYHist;
  Histogram2D<double> * diskXZHist;
  Histogram2D<double> * diskYZHist;

  Histogram2D<double> * haloXYHist;
  Histogram2D<double> * haloXZHist;
  Histogram2D<double> * haloYZHist;

  Histogram1D<double> * diskSurfaceDensity;
  Histogram1D<double> * diskVerticalDensity;
  Histogram1D<double> * diskAbsVerticalDensity;
  Histogram1D<double> * diskAverageZ;
  Histogram1D<double> * diskAverageZDispersion;
  Histogram1D<double> * diskVXVSR;
  Histogram1D<double> * diskVYVSR;
  Histogram1D<double> * diskVZVSR;
  Histogram1D<double> * diskVRVSR;
  Histogram1D<double> * diskVPVSR;

  Histogram1D<double> * diskVR2VSR;
  Histogram1D<double> * diskVP2VSR;
  Histogram1D<double> * diskVZ2VSR;
  Histogram1D<double> * diskVRVPVSR;
  Histogram1D<double> * diskVRVZVSR;
  Histogram1D<double> * diskVZVPVSR;


  Histogram1D<double> * diskSigmaZ2VSR;
  Histogram1D<double> * diskSigmaR2VSR;
  Histogram1D<double> * diskSigmaP2VSR;
  Histogram1D<double> * diskSigmaRSigmaPVSR;
  Histogram1D<double> * diskSigmaZSigmaPVSR;
  Histogram1D<double> * diskSigmaRSigmaZVSR;

#endif

#ifdef IOM_ANALYSIS
  Histogram1D<double> * diskEnergyVSR;
  Histogram1D<double> * diskLZVSR;
#endif

#ifdef RADIAL_MODES
  std::vector<std::vector<double> > radialAsVSR; // cos component
  std::vector<std::vector<double> > radialBsVSR; // sin component
  std::vector<std::vector<double> > radialCsVSR; // magnitude
#endif



#ifdef COMPUTE_VIRIAL_RATIO
  double virialRatio;
  double verticalHeating0;
  std::vector<double> kineticEnergies;
  std::vector<double> potentialEnergies;
  std::vector<double> virialRatios;
  std::vector<double> verticalHeating;

  std::vector<double> viralRatios;
#endif

#ifdef COMPUTE_DISK_MOMENT_OF_INERTIA
  std::vector<std::vector<double> > mofI(3, std::vector<double>(3,0));
#endif

#ifdef ROTATE_SYSTEM
  std::vector<std::vector<double> > eulerMatrix(3, std::vector<double>(3,0));
  std::vector<std::vector<double> > eulerMatrixT(3, std::vector<double>(3,0));
  std::vector<std::vector<double> > eigenSpace(4, std::vector<double>(3,0));
  std::vector<double> eigenvalues(3,0);
  std::vector<double> evA(3,0);
  std::vector<double> evB(3,0);
  std::vector<double> evC(3,0);
#endif

#ifdef COMPUTE_ANGULAR_MOMENTUM
  std::vector<std::vector<double> > lHalo;
  std::vector<std::vector<double> > lInnerHalo;
  std::vector<std::vector<double> > lDisk;
  std::vector<std::vector<double> > lBulge;
#endif


}
