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
  extern double verticalHeating0;

  extern Snapshot * newSnap;

  extern Galaxy * thisGalaxy;

  extern Context context;

  extern std::string snapName;

  extern std::vector<double> sbins;
  extern std::vector<double> zbins;
  extern std::vector<double> ybins;
  extern std::vector<double> xbins;
  extern std::vector<double> timeArr;
  extern std::vector<double> kineticEnergies;
  extern std::vector<double> potentialEnergies;
  extern std::vector<double> virialRatios;
  extern std::vector<double> verticalHeating;

  extern std::vector<std::vector<double> > lHalo;
  extern std::vector<std::vector<double> > lInnerHalo;
  extern std::vector<std::vector<double> > lDisk;
  extern std::vector<std::vector<double> > lBulge;

#ifdef DENSITY_HISTOGRAMS
  extern Histogram2D<double> * diskXYHist;
  extern Histogram2D<double> * diskXZHist;
  extern Histogram2D<double> * diskYZHist;

  extern Histogram2D<double> * haloXYHist;
  extern Histogram2D<double> * haloXZHist;
  extern Histogram2D<double> * haloYZHist;

#endif

#ifdef COMPUTE_VIRIAL_RATIO
  extern double virialRatio;
  extern std::vector<double> viralRatios;
#endif

#ifdef COMPUTE_DISK_MOMENT_OF_INERTIA
  extern std::vector<std::vector<double> > mofI;
#endif

#ifdef ROTATE_SYSTEM
  extern std::vector<std::vector<double> > eulerMatrix;
  extern std::vector<std::vector<double> > eulerMatrixT;
  extern std::vector<std::vector<double> > eigenSpace;
  extern std::vector<double> eigenvalues;
  extern std::vector<double> evA;
  extern std::vector<double> evB;
  extern std::vector<double> evC;
#endif

  /*
    Prototypes for global functions
  */

  double Magnitude(std::vector<double> &a);
  void Normalize(std::vector<double> &a);
  void SortEigenspace();
  void RotateToBoxFrame(std::vector<double> &x);
  void RotateToDiskFrame(std::vector<double> &x);
  void EulerMatrix(std::vector<double> &a, std::vector<std::vector<double> > &E);
  void Eigenspace(std::vector<std::vector<double> > &A, std::vector<std::vector<double> > &E);
  void CheckConvergence(  std::vector<std::vector<double > > &A, int &converged);
  void ForceZeros(std::vector<std::vector<double> > &A);
  void PrintMatrix(std::vector<std::vector<double> > &A);
  void HouseholderQR(std::vector<std::vector<double> > &A,\
		     std::vector<std::vector<double> > &QT,\
		     std::vector<std::vector<double> > &R);
  double Dot(std::vector<double> &a, std::vector<double> &b);
  std::vector<std::vector<double> > Outer(std::vector<double> &a, std::vector<double> &b);
  void SqrMatrixVectorMultiply(std::vector<std::vector<double> > &A,    \
			       std::vector<double> &b,			\
			       std::vector<double> &c);
  void SqrMatrixScalarMultiply(std::vector<std::vector<double> > &A, double b,\
			       std::vector<std::vector<double> > &C);
  void SqrMatrixMultiply(std::vector<std::vector<double> > &A,\
			 std::vector<std::vector<double> > &B,	\
			 std::vector<std::vector<double> > &C);
  void SqrMatrixAdd(std::vector<std::vector<double> > &A,\
		    std::vector<std::vector<double> > &B, \
		    std::vector<std::vector<double> > &C);
  void SqrMatrixSubtract(std::vector<std::vector<double> > &A,\
			 std::vector<std::vector<double> > &B, \
			 std::vector<std::vector<double> > &C);
  void SqrTransposeArg1MatrixMultiply(std::vector<std::vector<double> > &A,\
				      std::vector<std::vector<double> > &B, \
				      std::vector<std::vector<double> > &C);
  void SqrTransposeArg2MatrixMultiply(std::vector<std::vector<double> > &A,\
				      std::vector<std::vector<double> > &B, \
				      std::vector<std::vector<double> > &C);
  std::vector<double> LinearSpacing(double a, double b, int n);

  void ReadParameterFile(char * FILENAME);
  void PrintParameters();

  std::string GetGadgetSnapshotName(int i);
  std::string GetDiskXYDensityHistName(int i);
  std::string GetDiskXZDensityHistName(int i);
  std::string GetDiskYZDensityHistName(int i);
  std::string GetHaloXYDensityHistName(int i);
  std::string GetHaloXZDensityHistName(int i);
  std::string GetHaloYZDensityHistName(int i);

  void GNUPLOT_DensityScripts(int i);
  void WriteTimeSeriesOutput();
  
}
