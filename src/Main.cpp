#include "../include/Compile_Time_Options.h"
#include "../include/Context.h"
#include "../include/Snapshot.h"
#include "../include/Globals.h"
#include "../include/Galaxy.h"
#include "../include/Histogram.h"
#include <string>
#include <iostream>
#include <vector>

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

  Global::xbins = Global::LinearSpacing(-25,25,Global::context.XGridRes);
  Global::ybins = Global::LinearSpacing(-25,25,Global::context.YGridRes);
  Global::zbins = Global::LinearSpacing(-25,25,Global::context.ZGridRes);


  /*
    Loop over snapshots.
  */

  for (Global::snapNum = Global::context.StartingSnap; Global::snapNum < Global::context.NumSnaps; \
       Global::snapNum++){
    Global::snapName = "";

    // Gadget has a weird snapshot name format
#ifdef GADGET2
    Global::snapName = Global::GetGadgetSnapshotName(Global::snapNum);
#else
    Global::snapName = Global::context.SnapshotBase + std::to_string(Global::snapNum);
#endif // GADGET2

    // Create and load new snapshot data
    Global::newSnap = new Snapshot((char*)Global::snapName.c_str(), 1);
    Global::timeArr.push_back(Global::newSnap->Time());
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


#ifdef COMPUTE_DISK_MOMENT_OF_INERTIA
    Global::mofI = Global::thisGalaxy->GetDiskMofITensor();
    std::cout << "MofI = " << std::endl;
    Global::PrintMatrix(Global::mofI);
#endif

#ifdef COMPUTE_ANGULAR_MOMENTUM
    Global::thisGalaxy->ComputeAngularMomentum();
#endif

#if defined(COMPUTE_DISK_MOMENT_OF_INERTIA) && defined(ROTATE_SYSTEM) && \
    !defined(ORIENT_WITH_INNER_ANGULAR_MOMENTUM)
    Global::Eigenspace(Global::mofI, Global::eigenSpace);
    //Global::SortEigenspace();

    std::cout << "Eigenspace = " << std::endl;
    Global::PrintMatrix(Global::eigenSpace);

    // Orient to minor axis                                                                                                                 
    Global::EulerMatrix(Global::eigenSpace[1], Global::eulerMatrix);
    std::cout << "Euler matrix = " << std::endl;
    Global::PrintMatrix(Global::eulerMatrix);

    Global::thisGalaxy->AlignToMinorAxis();
#endif


#if defined(COMPUTE_DISK_MOMENT_OF_INERTIA) && defined(ROTATE_SYSTEM) &&\
    defined(ORIENT_WITH_INNER_ANGULAR_MOMENTUM)

    std::vector<double> axis;
    axis = Global::thisGalaxy->GetAngularMomentumAxis();
    Global::EulerMatrix(axis, Global::eulerMatrix);
    std::cout << "Euler matrix = " << std::endl;
    Global::PrintMatrix(Global::eulerMatrix);

    Global::thisGalaxy->AlignToMinorAxis();
#endif
    
    /*
    delete Global::thisGalaxy;
    delete Global::newSnap;
    continue;
    */

    // Make density histograms

    /*
    std::cout << "Making spatial bins..." << std::endl;

    Global::xbins = Global::LinearSpacing(-25,25,Global::context.XGridRes);
    Global::ybins = Global::LinearSpacing(-25,25,Global::context.YGridRes);
    Global::zbins = Global::LinearSpacing(-25,25,Global::context.ZGridRes);

    std::cout << "Done." << std::endl;
    
    */
#ifdef DENSITY_HISTOGRAMS

    std::cout << "Getting halo position lists..." << std::endl;
    std::vector<double> haloXVals;
    haloXVals = Global::thisGalaxy->GetHaloXs();
    std::vector<double> haloYVals;
    haloYVals = Global::thisGalaxy->GetHaloYs();
    std::vector<double> haloZVals;
    haloZVals = Global::thisGalaxy->GetHaloZs();
    std::cout << "Done." << std::endl;
    Global::haloXYHist = new Histogram2D<double>(haloXVals, haloYVals, Global::xbins,Global::ybins);
    Global::haloXZHist = new Histogram2D<double>(haloXVals, haloZVals, Global::xbins,Global::zbins);
    Global::haloYZHist = new Histogram2D<double>(haloYVals, haloZVals, Global::ybins,Global::zbins);

    std::cout << "Writing density histogram output..." << std::endl;
    Global::haloXYHist->PrintASCII(Global::GetHaloXYDensityHistName(Global::snapNum));
    Global::haloXZHist->PrintASCII(Global::GetHaloXZDensityHistName(Global::snapNum));
    Global::haloYZHist->PrintASCII(Global::GetHaloYZDensityHistName(Global::snapNum));


    std::cout << "Getting disk position lists..." << std::endl;
    std::vector<double> diskXVals; 
    diskXVals = Global::thisGalaxy->GetDiskXs();
    std::vector<double> diskYVals;
    diskYVals = Global::thisGalaxy->GetDiskYs();
    std::vector<double> diskZVals;
    diskZVals = Global::thisGalaxy->GetDiskZs();
    std::cout << "Done." << std::endl;
    Global::diskXYHist = new Histogram2D<double>(diskXVals, diskYVals, Global::xbins,Global::ybins);
    Global::diskXZHist = new Histogram2D<double>(diskXVals, diskZVals, Global::xbins,Global::zbins);
    Global::diskYZHist = new Histogram2D<double>(diskYVals, diskZVals, Global::ybins,Global::zbins);    

    std::cout << "Writing density histogram output..." << std::endl;
    Global::diskXYHist->PrintASCII(Global::GetDiskXYDensityHistName(Global::snapNum));
    Global::diskXZHist->PrintASCII(Global::GetDiskXZDensityHistName(Global::snapNum));
    Global::diskYZHist->PrintASCII(Global::GetDiskYZDensityHistName(Global::snapNum));

    std::cout << "Done." << std::endl;
    Global::GNUPLOT_DensityScripts(Global::snapNum);
#endif // DENSITY_HISTOGRAMS

#ifdef COMPUTE_VIRIAL_RATIO
    Global::virialRatio = Global::thisGalaxy->ComputeVirialRatio();
    std::cout << "Virial Ratio: " << Global::virialRatio << std::endl;
#endif

    // Reset variables

    // Free memory when we're done with snapshot

#ifdef DENSITY_HISTOGRAMS
    diskXVals.clear();
    diskYVals.clear();
    diskZVals.clear();
#endif
    //xVals.swap(std::vector<double>(xVals));
    //yVals.swap(std::vector<double>(yVals));
    //zVals.swap(std::vector<double>(zVals));

    delete Global::diskXYHist;
    delete Global::diskXZHist;
    delete Global::diskYZHist;
    delete Global::thisGalaxy;
    delete Global::newSnap;
  }

  Global::WriteTimeSeriesOutput();
  return 0;  // Success
}
