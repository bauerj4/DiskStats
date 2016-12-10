#include "../include/Compile_Time_Options.h"
#include "../include/Globals.h"
#include <fstream>
#include <iostream>

/*
  These functions will write Gnuplot scripts and
  make system calls to generate plots.
*/

void Global::GNUPLOT_DensityScripts(int i){

  // XY density

  std::ofstream outfileXY(Global::context.PathToGnuplot + "/Disk_Density_XY." + std::to_string(i) + ".gnuplot");
  
  outfileXY << "set term post eps enhanced color font \'Helvetica,10\'" <<  std::endl;
  outfileXY << "set output \'" << Global::context.PathToGnuplot + "/Disk_Density_XY." + std::to_string(i)\
	    << ".eps\'"<< std::endl;
  outfileXY << "load \'" << Global::context.DensityPalette << "\'" << std::endl;
  outfileXY << "set view map" << std::endl;
  outfileXY << "set logscale cb" << std::endl;
  outfileXY << "plot \'" << Global::GetDiskXYDensityHistName(Global::snapNum) \
	    << "\' matrix nonuniform  with image" << std::endl;

  outfileXY.close();

  outfileXY.open(Global::context.PathToGnuplot + "/Halo_Density_XY." + std::to_string(i) + ".gnuplot");

  outfileXY << "set term post eps enhanced color font \'Helvetica,10\'" <<  std::endl;
  outfileXY << "set output \'" << Global::context.PathToGnuplot + "/Halo_Density_XY." + std::to_string(i)\
            << ".eps\'"<< std::endl;
  outfileXY << "load \'" << Global::context.DensityPalette << "\'" << std::endl;
  outfileXY << "set view map" << std::endl;
  outfileXY << "set logscale cb" << std::endl;
  outfileXY << "plot \'" << Global::GetHaloXYDensityHistName(Global::snapNum) \
            << "\' matrix nonuniform  with image" << std::endl;

  outfileXY.close();



  // XZ density

  std::ofstream outfileXZ(Global::context.PathToGnuplot + "/Disk_Density_XZ." + std::to_string(i) + ".gnuplot");

  outfileXZ << "set term post eps enhanced color font \'Helvetica,10\'" <<  std::endl;
  outfileXZ << "set output \'" << Global::context.PathToGnuplot + "/Disk_Density_XZ." + std::to_string(i)\
            << ".eps\'"<< std::endl;
  outfileXZ << "load \'" << Global::context.DensityPalette <<"\'" << std::endl;
  outfileXZ << "set view map" << std::endl;
  outfileXZ << "set logscale cb" << std::endl;
  outfileXZ << "plot \'" << Global::GetDiskXZDensityHistName(Global::snapNum) 
	    << "\' matrix nonuniform with image" << std::endl;

  outfileXZ.close();

  outfileXZ.open(Global::context.PathToGnuplot + "/Halo_Density_XZ." + std::to_string(i) + ".gnuplot");

  outfileXZ << "set term post eps enhanced color font \'Helvetica,10\'" <<  std::endl;
  outfileXZ << "set output \'" << Global::context.PathToGnuplot + "/Halo_Density_XZ." + std::to_string(i)\
            << ".eps\'"<< std::endl;
  outfileXZ << "load \'" << Global::context.DensityPalette <<"\'" << std::endl;
  outfileXZ << "set view map" << std::endl;
  outfileXZ << "set logscale cb" << std::endl;
  outfileXZ << "plot \'" << Global::GetHaloXZDensityHistName(Global::snapNum)
            << "\' matrix nonuniform with image" << std::endl;

  outfileXZ.close();

  
  // YZ density

  std::ofstream outfileYZ(Global::context.PathToGnuplot + "/Disk_Density_YZ." + std::to_string(i) + ".gnuplot");

  outfileYZ << "set term post eps enhanced color font \'Helvetica,10\'" <<  std::endl;
  outfileYZ << "set output \'" << Global::context.PathToGnuplot + "/Disk_Density_YZ." + std::to_string(i)\
            << ".eps\'"<< std::endl;
  outfileYZ << "load \'" << Global::context.DensityPalette << "\'" << std::endl;
  //  outfileYZ << "set border 3 back ls 11" << std::endl;
  outfileYZ << "set view map" << std::endl;
  outfileYZ << "set logscale cb" << std::endl;
  outfileYZ << "plot \'" << Global::GetDiskYZDensityHistName(Global::snapNum) \
	    << "\' matrix nonuniform  with image" << std::endl;

  outfileYZ.close();

  outfileYZ.open(Global::context.PathToGnuplot + "/Halo_Density_YZ." + std::to_string(i) + ".gnuplot");

  outfileYZ << "set term post eps enhanced color font \'Helvetica,10\'" <<  std::endl;
  outfileYZ << "set output \'" << Global::context.PathToGnuplot + "/Halo_Density_YZ." + std::to_string(i)\
            << ".eps\'"<< std::endl;
  outfileYZ << "load \'" << Global::context.DensityPalette << "\'" << std::endl;
  //  outfileYZ << "set border 3 back ls 11" << std::endl;                                                                                                              
  outfileYZ << "set view map" << std::endl;
  outfileYZ << "set logscale cb" << std::endl;
  outfileYZ << "plot \'" << Global::GetHaloYZDensityHistName(Global::snapNum) \
            << "\' matrix nonuniform  with image" << std::endl;


  // Panels

  std::ofstream outfilePanels(Global::context.PathToGnuplot + "/Disk_Density_Panels." + std::to_string(i) \
			      + ".gnuplot");

  outfilePanels << "set term post eps enhanced color font \'Helvetica,10\'" <<  std::endl;
  outfilePanels << "set output \'" << Global::context.PathToGnuplot + "/Disk_Density_Panels." + std::to_string(i) \
		<< ".eps\'"<< std::endl;
  outfilePanels << "set tmargin 1" << std::endl;
  outfilePanels << "set bmargin 1" << std::endl;
  outfilePanels << "set lmargin 0" << std::endl;
  outfilePanels << "set rmargin 0" << std::endl;
  outfilePanels << "load \'" << Global::context.DensityPalette << "\'" << std::endl;
  outfilePanels << "set view map" << std::endl;
  outfilePanels << "set logscale cb" << std::endl;
  outfilePanels << "unset xtics" << std::endl;
  outfilePanels << "unset ytics" << std::endl;
  outfilePanels << "unset colorbox" << std::endl;

  outfilePanels << "set multiplot layout 2,3 scale 1.0, 0.9" << std::endl;

  outfilePanels << "set xlabel \"XY\"" << std::endl;
  outfilePanels << "plot \'" << Global::GetDiskXYDensityHistName(Global::snapNum) \
                << "\' matrix nonuniform  with image t \'\'" << std::endl;
  //outfilePanels << "set contour" << std::endl; 

  outfilePanels << "set xlabel \"XZ\"" << std::endl;
  outfilePanels << "plot \'" << Global::GetDiskXZDensityHistName(Global::snapNum) \
                << "\' matrix nonuniform  with image t \'\'" << std::endl;

  outfilePanels << "set xlabel \"YZ\"" << std::endl;
  outfilePanels << "plot \'" << Global::GetDiskYZDensityHistName(Global::snapNum) \
		<< "\' matrix nonuniform  with image t \'\'" << std::endl;

  outfilePanels << "load \'" << Global::context.DensityPalette2 <<"\'" << std::endl;

  outfilePanels << "set xlabel \"XY\"" << std::endl;
  outfilePanels << "plot \'" << Global::GetHaloXYDensityHistName(Global::snapNum) \
                << "\' matrix nonuniform  with image t \'\'" << std::endl;

  outfilePanels << "set xlabel \"XZ\"" << std::endl;
  outfilePanels << "plot \'" << Global::GetHaloXZDensityHistName(Global::snapNum) \
                << "\' matrix nonuniform  with image t \'\'" << std::endl;

  outfilePanels << "set xlabel \"YZ\"" << std::endl;
  outfilePanels << "plot \'" << Global::GetHaloYZDensityHistName(Global::snapNum) \
                << "\' matrix nonuniform  with image t \'\'" << std::endl;

  outfilePanels << "unset multiplot" << std::endl;
  outfilePanels.close();
}


void Global::WriteTimeSeriesOutput(){
  
  /*
    Energies file
  */

#ifdef COMPUTE_VIRIAL_RATIO
  std::ofstream energies(Global::context.PathToGnuplot + "/Energies.dat");
  for (unsigned int i = 0; i < timeArr.size(); i++){
    energies << Global::timeArr[i] << "\t" << Global::kineticEnergies[i] << "\t" \
	     << Global::potentialEnergies[i] << "\t" << Global::virialRatios[i] \ 
	     << "\t" << Global::verticalHeating[i] << std::endl;
  }
#endif


  /*
    Component angular momenta file
  */

#ifdef COMPUTE_ANGULAR_MOMENTUM
  std::ofstream am(Global::context.PathToGnuplot + "/HDB_Angular_Momenta.dat");
  for (unsigned int i = 0; i < timeArr.size(); i++){
    am << Global::timeArr[i] << "\t" << Global::lHalo[i][0] << "\t" \
       << Global::lHalo[i][1] << "\t" << Global::lHalo[i][2] << "\t"	\
       << Global::lDisk[i][0] << "\t" << Global::lDisk[i][1] << "\t"	\
       << Global::lDisk[i][2] << "\t" << Global::lBulge[i][0] << "\t"	\
       << Global::lBulge[i][1] << "\t" << Global::lBulge[i][2] << "\t" \
       << Global::lInnerHalo[i][0] << "\t" << Global::lInnerHalo[i][1] << "\t" \
       << Global::lInnerHalo[i][2] << "\t" << std::endl;
  }
#endif
}
