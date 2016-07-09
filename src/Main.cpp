#include "../include/Compile_Time_Options.h"
#include "../include/Context.h"
#include "../include/Snapshot.h"
#include "../include/Globals.h"
#include "../include/Galaxy.h"
#include "../include/Histogram.h"
#include <string>
#include <iostream>

/*
  
  This code is J. Bauer's unnamed disk analysis software
  intended to analyze very large N-body disk datasets for
  various useful quantities.  It is the author's hope that 
  this code may be built upon by others to expand research
  in the subject of galaxy dynamics.

  Copyright J. S. Bauer, 2016 (at Queen's University)

*/


int main(int argc, char ** argv){

  Global::ReadParameterFile(argv[1]);
  Global::PrintParameters();

  /*
    Loop over snapshots.
  */

  for (Global::snapNum = 0; Global::snapNum < Global::context.NumSnaps; Global::snapNum++){
    Global::snapName = "";

    // Gadget has a weird snapshot name format
#ifdef GADGET2
    Global::snapName = Global::GetGadgetSnapshotName(Global::snapNum);
#else
    Global::snapName = Global::context.SnapshotBase + std::to_string(Global::snapNum);
#endif // GADGET2

    // Create and load new snapshot data
    Global::newSnap = new Snapshot((char*)Global::snapName.c_str(), 1);

    // Create the galaxy

#ifdef USE_X_BAR
    Global::thisGalaxy = new Galaxy(Global::xBar, Global::yBar, Global::zBar, \
				    Global::context.GalaxyR0, Global::newSnap);
#else
    Global::thisGalaxy = new Galaxy(Global::context.GalaxyX0, Global::context.GalaxyY0,\
				    Global::context.GalaxyZ0, Global::context.GalaxyR0,\
				    Global::newSnap);
#endif // USE_X_BAR


#ifdef CENTER_ON_DISK_CENTROID
    Global::thisGalaxy->CenterOnDiskCentroid();
#endif // CENTER_ON_DISK_CENTROID

    // Make density histograms

    Global::xbins = Global::LinearSpacing(-25,25,Global::context.XGridRes);
    Global::ybins = Global::LinearSpacing(-25,25,Global::context.YGridRes);
    Global::zbins = Global::LinearSpacing(-25,25,Global::context.ZGridRes);


#ifdef DENSITY_HISTOGRAMS

    std::vector<double> xVals = Global::thisGalaxy->GetDiskXs();
    std::vector<double> yVals = Global::thisGalaxy->GetDiskYs();
    std::vector<double> zVals = Global::thisGalaxy->GetDiskZs();
    Global::diskXYHist = new Histogram2D<double>(xVals, yVals, Global::xbins,Global::ybins);
    Global::diskXZHist = new Histogram2D<double>(xVals, zVals, Global::xbins,Global::zbins);
    Global::diskYZHist = new Histogram2D<double>(yVals, zVals, Global::ybins,Global::zbins);    

    Global::diskXYHist->PrintASCII(Global::GetDiskXYDensityHistName(Global::snapNum));
    Global::diskXZHist->PrintASCII(Global::GetDiskXZDensityHistName(Global::snapNum));
    Global::diskYZHist->PrintASCII(Global::GetDiskYZDensityHistName(Global::snapNum));

    Global::GNUPLOT_DensityScripts(Global::snapNum);
#endif // DENSITY_HISTOGRAMS

#ifdef COMPUTE_VIRIAL_RATIO
    Global::virialRatio = Global::thisGalaxy->ComputeVirialRatio();
    std::cout << "Virial Ratio: " << Global::virialRatio << std::endl;
#endif

    // Free memory when we're done with snapshot

    delete Global::diskXYHist;
    delete Global::diskXZHist;
    delete Global::diskYZHist;
    delete Global::thisGalaxy;
    delete Global::newSnap;
  }

  return 0;  // Success
}
