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

  std::vector<double> sbins;
  std::vector<double> zbins;
  std::vector<double> ybins;
  std::vector<double> xbins;

#ifdef DENSITY_HISTOGRAMS
  Histogram2D<double> * diskXYHist;
  Histogram2D<double> * diskXZHist;
  Histogram2D<double> * diskYZHist;
#endif

#ifdef COMPUTE_VIRIAL_RATIO
  double virialRatio;
  std::vector<double> viralRatios;
#endif


}
