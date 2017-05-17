#include "../include/Compile_Time_Options.h"
#include "../include/Context.h"
#include "../include/Snapshot.h"
#include "../include/Globals.h"
#include "../include/Galaxy.h"
#include "../include/Histogram.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

/*
  
  This code is J. Bauer's unnamed disk analysis software
  intended to analyze very large N-body disk datasets for
  various useful quantities.  It is the author's hope that 
  this code may be built upon by others to expand research
  in the subject of galaxy dynamics.

  Copyright J. S. Bauer, 2016 (at Queen's University)

*/


using namespace Global;

int main(int argc, char ** argv){

  ReadParameterFile(argv[1]);
  PrintParameters();

  xbins  = LinearSpacing(-25,25,context.XGridRes);
  ybins  = LinearSpacing(-25,25,context.YGridRes);
  zbins  = LinearSpacing(-25,25,context.ZGridRes);
  sbins  = LinearSpacing(0.,25.,context.RGridRes);
  
  vxbins = LinearSpacing(-200,200,context.XGridRes);
  vybins = LinearSpacing(-200,200,context.YGridRes);
  vzbins = LinearSpacing(-200,200,context.ZGridRes);

  vrbins = LinearSpacing(-200,200,2.*context.RGridRes);
  vpbins = LinearSpacing(-200,200,2.*context.RGridRes);


  /*
    Set the surface density weights
  */
  
  double ds, s;
  for (int k = 0; k < sbins.size() - 1; k++){
    ds = sbins[k + 1] - sbins[k];
    s  = sbins[k] + ds;
    surfaceDensityWeights.push_back(1./(2. * M_PI * s * ds));
  }

  /*
    Loop over snapshots.
  */

  for (snapNum = context.StartingSnap; snapNum < context.NumSnaps; \
       snapNum++){
    snapName = "";

    // Gadget has a weird snapshot name format
#ifdef GADGET2
    snapName = GetGadgetSnapshotName(snapNum);
#else
    snapName = context.SnapshotBase + std::to_string(snapNum);
#endif // GADGET2

    // Create and load new snapshot data
    newSnap = new Snapshot((char*)snapName.c_str(), 1);
    timeArr.push_back(newSnap->Time());
    // Create the galaxy

#ifdef USE_X_BAR
    thisGalaxy = new Galaxy(xBar, yBar, zBar, \
			    context.GalaxyR0, newSnap);
#else
    thisGalaxy = new Galaxy(context.GalaxyX0, context.GalaxyY0,\
			    context.GalaxyZ0, context.GalaxyR0,	\
			    newSnap);
#endif // USE_X_BAR


    

#ifdef CENTER_ON_DISK_CENTROID
    thisGalaxy->CenterOnDiskCentroid();
#endif // CENTER_ON_DISK_CENTROID


#ifdef COMPUTE_DISK_MOMENT_OF_INERTIA
    mofI = thisGalaxy->GetDiskMofITensor();
    std::cout << "MofI = " << std::endl;
    PrintMatrix(mofI);
#endif

#ifdef COMPUTE_ANGULAR_MOMENTUM
    thisGalaxy->ComputeAngularMomentum();
#endif

#if defined(COMPUTE_DISK_MOMENT_OF_INERTIA) && defined(ROTATE_SYSTEM) && \
    !defined(ORIENT_WITH_INNER_ANGULAR_MOMENTUM)
    Eigenspace(mofI, eigenSpace);

    std::cout << "Eigenspace = " << std::endl;
    PrintMatrix(eigenSpace);

    // Orient to minor axis                                                                                                                 
    EulerMatrix(eigenSpace[1], eulerMatrix);
    std::cout << "Euler matrix = " << std::endl;
    PrintMatrix(eulerMatrix);

    thisGalaxy->AlignToMinorAxis();
#endif


#if defined(COMPUTE_DISK_MOMENT_OF_INERTIA) && defined(ROTATE_SYSTEM) &&\
    defined(ORIENT_WITH_INNER_ANGULAR_MOMENTUM)

    std::vector<double> axis;
    axis = thisGalaxy->GetAngularMomentumAxis();
    EulerMatrix(axis, eulerMatrix);
    std::cout << "Euler matrix = " << std::endl;
    PrintMatrix(eulerMatrix);

    thisGalaxy->AlignToMinorAxis();
#endif
    
    // Make density histograms

#ifdef DENSITY_HISTOGRAMS

    std::cout << "Getting halo position lists..." << std::endl;
    std::vector<double> haloXVals, haloYVals, haloZVals, diskXVals,\
      diskYVals, diskZVals, diskAbsZVals, diskRVals, diskVZ2Vals, diskVR2Vals, diskVP2Vals, \
      diskVXVals, diskVYVals, diskVZVals, diskVRVals, diskVPVals, diskPhiVals;

    haloXVals = thisGalaxy->GetHaloXs();
    haloYVals = thisGalaxy->GetHaloYs();
    haloZVals = thisGalaxy->GetHaloZs();


    std::cout << "Done." << std::endl;
    haloXYHist = new Histogram2D<double>(haloXVals, haloYVals, xbins,ybins);
    haloXZHist = new Histogram2D<double>(haloXVals, haloZVals, xbins,zbins);
    haloYZHist = new Histogram2D<double>(haloYVals, haloZVals, ybins,zbins);

    std::cout << "Writing density histogram output..." << std::endl;
    haloXYHist->PrintASCII(GetHaloXYDensityHistName(snapNum));
    haloXZHist->PrintASCII(GetHaloXZDensityHistName(snapNum));
    haloYZHist->PrintASCII(GetHaloYZDensityHistName(snapNum));


    std::cout << "Getting disk position lists..." << std::endl;
    diskXVals  = thisGalaxy->GetDiskXs();
    diskYVals  = thisGalaxy->GetDiskYs();
    diskZVals  = thisGalaxy->GetDiskZs();
    diskVXVals = thisGalaxy->GetDiskVXs();
    diskVYVals = thisGalaxy->GetDiskVYs();
    diskVZVals = thisGalaxy->GetDiskVZs();
    diskVRVals = thisGalaxy->GetDiskVRs();
    diskVPVals = thisGalaxy->GetDiskVPhis();

    for (int i = 0; i < diskVZVals.size(); i++){
      diskAbsZVals.push_back(fabs(diskZVals[i]));
      diskVZ2Vals.push_back(diskVZVals[i]*diskVZVals[i]);
      diskVR2Vals.push_back(diskVRVals[i]*diskVRVals[i]);
      diskVP2Vals.push_back(diskVPVals[i]*diskVPVals[i]);
    }

    diskRVals   = thisGalaxy->GetDiskCylRs();
    diskPhiVals = thisGalaxy->GetDiskCylPhis();

    std::cout << "Done." << std::endl;
    diskXYHist = new Histogram2D<double>(diskXVals, diskYVals, xbins,ybins);
    diskXZHist = new Histogram2D<double>(diskXVals, diskZVals, xbins,zbins);
    diskYZHist = new Histogram2D<double>(diskYVals, diskZVals, ybins,zbins);    
    diskSurfaceDensity = new Histogram1D<double>(diskRVals,sbins\
						 ,1,surfaceDensityWeights);
    diskAbsVerticalDensity = new Histogram1D<double>(diskAbsZVals,sbins,1);
    diskVerticalDensity    = new Histogram1D<double>(diskZVals,zbins,1);
    diskAverageZ           = new Histogram1D<double>(diskRVals,sbins,0,diskAbsZVals);
    diskVXVSR              = new Histogram1D<double>(diskRVals,sbins,0,diskVXVals);
    diskVYVSR              = new Histogram1D<double>(diskRVals,sbins,0,diskVYVals);
    diskVZVSR              = new Histogram1D<double>(diskRVals,sbins,0,diskVZVals);
    diskVRVSR              = new Histogram1D<double>(diskRVals,sbins,0,diskVRVals);
    diskVPVSR              = new Histogram1D<double>(diskRVals,sbins,0,diskVPVals);

    diskVZ2VSR             = new Histogram1D<double>(diskRVals,sbins,0,diskVZ2Vals);
    diskVR2VSR             = new Histogram1D<double>(diskRVals,sbins,0,diskVR2Vals);
    diskVP2VSR             = new Histogram1D<double>(diskRVals,sbins,0,diskVP2Vals);

    diskSigmaZ2VSR         = *diskVZ2VSR - *((*diskVZVSR)*(*diskVZVSR));
    diskSigmaR2VSR         = *diskVR2VSR - *((*diskVRVSR)*(*diskVRVSR));
    diskSigmaP2VSR         = *diskVP2VSR - *((*diskVPVSR)*(*diskVPVSR));

    std::cout << "Writing density histogram output..." << std::endl;
    diskXYHist->PrintASCII(GetDiskXYDensityHistName(snapNum));
    diskXZHist->PrintASCII(GetDiskXZDensityHistName(snapNum));
    diskYZHist->PrintASCII(GetDiskYZDensityHistName(snapNum));
    diskSurfaceDensity->PrintASCII(GetDiskSurfaceDensityName(snapNum));
    diskVerticalDensity->PrintASCII(GetDiskVerticalDensityName(snapNum));
    diskAbsVerticalDensity->PrintASCII(GetDiskAbsVerticalDensityName(snapNum));
    diskAverageZ->PrintASCII(GetDiskAverageZName(snapNum));

    diskVPVSR->PrintASCII(GetDiskVPVSRName(snapNum));
    diskVRVSR->PrintASCII(GetDiskVRVSRName(snapNum));

    diskVP2VSR->PrintASCII(GetDiskVP2VSRName(snapNum));
    diskVR2VSR->PrintASCII(GetDiskVR2VSRName(snapNum));


    diskSigmaZ2VSR->PrintASCII(GetDiskSigmaZ2VSRName(snapNum));
    diskSigmaR2VSR->PrintASCII(GetDiskSigmaR2VSRName(snapNum));
    diskSigmaP2VSR->PrintASCII(GetDiskSigmaP2VSRName(snapNum));

    std::cout << "Done." << std::endl;
    GNUPLOT_DensityScripts(snapNum);

#ifdef RADIAL_MODES
    ComputeAndPrintRadialModes(diskPhiVals, diskRVals);
#endif // RADIAL_MODES
#endif // DENSITY_HISTOGRAMS

#ifdef IOM_ANALYSIS
    std::vector<double> diskEs, diskLzs;

    diskEs  = thisGalaxy->GetDiskEnergies();
    diskLzs = thisGalaxy->GetDiskLzs();

    diskEnergyVSR  = new Histogram1D<double>(diskRVals,sbins,0,diskEs);
    diskLZVSR      = new Histogram1D<double>(diskRVals,sbins,0,diskLzs);

    diskLZVSR->PrintASCII(GetDiskLZVSRName(snapNum));
    diskEnergyVSR->PrintASCII(GetDiskEnergyVSRName(snapNum));

#endif

#ifdef COMPUTE_VIRIAL_RATIO
    virialRatio = thisGalaxy->ComputeVirialRatio();
    std::cout << "Virial Ratio: " << virialRatio << std::endl;
#endif

    // Reset variables

    // Free memory when we're done with snapshot

#ifdef DENSITY_HISTOGRAMS
    diskXVals.clear();
    diskYVals.clear();
    diskZVals.clear();
#endif

#ifdef RADIAL_MODES
    radialAsVSR.clear();
    radialBsVSR.clear();
    radialCsVSR.clear();
#endif

    delete diskXYHist;
    delete diskXZHist;
    delete diskYZHist;
    delete diskVXVSR;
    delete diskVYVSR;
    delete diskVZVSR;
    delete diskVZ2VSR;
    delete diskSurfaceDensity;
    delete diskVerticalDensity;
    delete diskAbsVerticalDensity;
    delete diskAverageZ;
    delete thisGalaxy;
    delete newSnap;
  }

  WriteTimeSeriesOutput();
  return 0;  // Success
}


/*
  This function computes and prints the radial modes of the disk.
  Collapsed in this function to prevent clutter.
*/

#if defined(DENSITY_HISTOGRAMS) && defined(RADIAL_MODES)
inline void Global::ComputeAndPrintRadialModes(std::vector<double> &diskPhis, \
					       std::vector<double> &diskRVals){
  std::vector<double> aWeights;
  std::vector<double> bWeights;
  std::vector<double> cVals;
  std::string base, number, tail, fname;

  base   = Global::context.PathToGnuplot + "/Disk_Radial_Modes.";
  tail   = ".ascii";
  number = std::to_string(snapNum);
  fname  = base + number + tail;


  std::ofstream FILE(fname);

  Histogram1D<double> * thisAHist, * thisBHist;

  for (int m = 0; m <= RADIAL_MODES; m++){

    // Compute weights for mode m
    for (int i = 0; i < diskPhis.size(); i++){
      aWeights.push_back(cos(diskPhis[i] * m));
      bWeights.push_back(sin(diskPhis[i] * m));
      //std::cout << "a_m,m, phi = " << aWeights[i] << ", "<< m << ", " << diskPhis[i] << std::endl;
    }
    
    // Make histograms
    thisAHist = new Histogram1D<double>(diskRVals,sbins,0,aWeights);
    thisBHist = new Histogram1D<double>(diskRVals,sbins,0,bWeights);

    // Get c_m(r) vals
    for (int i = 0; i < thisAHist->counts.size(); i++){
      cVals.push_back(sqrt(thisAHist->counts[i] * thisAHist->counts[i] + \
			   thisBHist->counts[i] * thisBHist->counts[i]));
    }
    radialAsVSR.push_back(thisAHist->counts);
    radialBsVSR.push_back(thisBHist->counts);
    radialCsVSR.push_back(cVals);


    delete thisAHist;
    delete thisBHist;
    aWeights.clear();
    bWeights.clear();
    cVals.clear();
  }

  // Now print file

  for (int i = 0; i < sbins.size() - 1; i++){
    FILE << sbins[i] << " ";
    for (int m = 0; m <= RADIAL_MODES; m++){ 
      //FILE << radialAsVSR[m][i] << " ";
      //FILE << radialBsVSR[m][i] << " ";
      FILE << radialCsVSR[m ][i] << " ";
    }
    FILE << "\n";
  }
  
  FILE.close();
}

#endif
