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
  /*
    Prototypes for global functions
  */

  std::vector<double> LinearSpacing(double a, double b, int n);

  void ReadParameterFile(char * FILENAME);
  void PrintParameters();

  std::string GetGadgetSnapshotName(int i);

  
}
