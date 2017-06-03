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
  extern std::vector<double> vzbins;
  extern std::vector<double> vybins;
  extern std::vector<double> vxbins;
  extern std::vector<double> vrbins;
  extern std::vector<double> vpbins;
  extern std::vector<double> timeArr;
  extern std::vector<double> kineticEnergies;
  extern std::vector<double> potentialEnergies;
  extern std::vector<double> virialRatios;
  extern std::vector<double> verticalHeating;
  extern std::vector<double> surfaceDensityWeights;

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

  extern Histogram1D<double> * diskSurfaceDensity;
  extern Histogram1D<double> * diskVerticalDensity;
  extern Histogram1D<double> * diskAbsVerticalDensity;
  extern Histogram1D<double> * diskAverageZ;
  extern Histogram1D<double> * diskAverageZDispersion;
  extern Histogram1D<double> * diskVXVSR;
  extern Histogram1D<double> * diskVYVSR;
  extern Histogram1D<double> * diskVZVSR;
  extern Histogram1D<double> * diskVRVSR;
  extern Histogram1D<double> * diskVPVSR;

  extern Histogram1D<double> * diskVR2VSR;
  extern Histogram1D<double> * diskVP2VSR;
  extern Histogram1D<double> * diskVZ2VSR;
  extern Histogram1D<double> * diskVRVPVSR;
  extern Histogram1D<double> * diskVRVZVSR;
  extern Histogram1D<double> * diskVZVPVSR;

  extern Histogram1D<double> * diskSigmaZ2VSR;
  extern Histogram1D<double> * diskSigmaR2VSR;
  extern Histogram1D<double> * diskSigmaP2VSR;
  extern Histogram1D<double> * diskSigmaRSigmaPVSR;
  extern Histogram1D<double> * diskSigmaZSigmaPVSR;
  extern Histogram1D<double> * diskSigmaRSigmaZVSR;

#endif

#ifdef IOM_ANALYSIS
  extern Histogram1D<double> * diskEnergyVSR;
  extern Histogram1D<double> * diskLZVSR;
#endif

#ifdef RADIAL_MODES
  extern std::vector<std::vector<double> > radialAsVSR;
  extern std::vector<std::vector<double> > radialBsVSR;
  extern std::vector<std::vector<double> > radialCsVSR;
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
  std::string GetDiskSurfaceDensityName(int i);
  std::string GetDiskVerticalDensityName(int i);
  std::string GetDiskAbsVerticalDensityName(int i);
  std::string GetDiskAverageZName(int i);
  std::string GetDiskSigmaZ2VSRName(int i);
  std::string GetDiskSigmaR2VSRName(int i);
  std::string GetDiskSigmaP2VSRName(int i);

  std::string GetDiskSigmaRSigmaPVSRName(int i);
  std::string GetDiskSigmaRSigmaZVSRName(int i);
  std::string GetDiskSigmaZSigmaPVSRName(int i);

  std::string GetDiskVPVSRName(int i);
  std::string GetDiskVRVSRName(int i);
  std::string GetDiskVZVSRName(int i);

  std::string GetDiskVP2VSRName(int i);
  std::string GetDiskVR2VSRName(int i);
  std::string GetDiskVZ2VSRName(int i);


  std::string GetDiskVRVPVSRName(int i);
  std::string GetDiskVRVZVSRName(int i);
  std::string GetDiskVZVPVSRName(int i);
  std::string GetDiskLZVSRName(int i);
  std::string GetDiskEnergyVSRName(int i);

  void GNUPLOT_DensityScripts(int i);
  void WriteTimeSeriesOutput();


  inline void ComputeAndPrintRadialModes(std::vector<double> &diskPhis,\
					 std::vector<double> &diskRVals);  
}
