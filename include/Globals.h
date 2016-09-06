#include "Compile_Time_Options.h"
#include "Context.h"
#include "Snapshot.h"
#include "Galaxy.h"
#include "Histogram.h"
#include <string>
#include <vector>

namespace Global{

  /*
    Global variables
  */

  extern int snapNum;
  extern int posCounter;

  extern double xBar;
  extern double yBar;
  extern double zBar;

  extern Snapshot * newSnap;

  extern Galaxy * thisGalaxy;

  extern Context context;

  extern std::string snapName;

  extern std::vector<double> sbins;
  extern std::vector<double> zbins;
  extern std::vector<double> ybins;
  extern std::vector<double> xbins;

#ifdef DENSITY_HISTOGRAMS
  extern Histogram2D<double> * diskXYHist;
  extern Histogram2D<double> * diskXZHist;
  extern Histogram2D<double> * diskYZHist;
#endif

#ifdef COMPUTE_VIRIAL_RATIO
  extern double virialRatio;
  extern std::vector<double> viralRatios;
#endif
  /*
    Prototypes for global functions
  */

  void RotateToBoxFrame(std::vector<double> &x, std::vector<std::vector<double> > &A);
  void RotateToDiskFrame(std::vector<double> &x, std::vector<std::vector<double> > &A);
  std::vector<std::vector<double> > EulerMatrix(std::vector<double> &a);
  std::vector<std::vector<double> > Eigenspace(std::vector<std::vector<double> > &A);
  double Dot(std::vector<double> &a, std::vector<double> &b);
  double Outer(std::vector<double> &a, std::vector<double> &b);
  std::vector<double> LinearSpacing(double a, double b, int n);

  void ReadParameterFile(char * FILENAME);
  void PrintParameters();

  std::string GetGadgetSnapshotName(int i);
  std::string GetDiskXYDensityHistName(int i);
  std::string GetDiskXZDensityHistName(int i);
  std::string GetDiskYZDensityHistName(int i);

  void GNUPLOT_DensityScripts(int i);


  
}
