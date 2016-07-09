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
  //outfileXY << "set border 3 back ls 11" << std::endl;
  outfileXY << "set view map" << std::endl;
  outfileXY << "set logscale cb" << std::endl;
  outfileXY << "splot \'" << Global::GetDiskXYDensityHistName(Global::snapNum) \
	    << "\' matrix with image " << std::endl;

  outfileXY.close();
  // XZ density

  std::ofstream outfileXZ(Global::context.PathToGnuplot + "/Disk_Density_XZ." + std::to_string(i) + ".gnuplot");

  outfileXZ << "set term post eps enhanced color font \'Helvetica,10\'" <<  std::endl;
  outfileXZ << "set output \'" << Global::context.PathToGnuplot + "/Disk_Density_XZ." + std::to_string(i)\
            << ".eps\'"<< std::endl;
  outfileXZ << "load \'" << Global::context.DensityPalette <<"\'" << std::endl;
  //  outfileXZ << "set border 3 back ls 11" << std::endl;
  outfileXZ << "set view map" << std::endl;
  outfileXZ << "set logscale cb" << std::endl;
  outfileXZ << "splot \'" << Global::GetDiskXZDensityHistName(Global::snapNum) 
	    << "\' matrix with image " << std::endl;

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
  outfileYZ << "splot \'" << Global::GetDiskYZDensityHistName(Global::snapNum) \
	    << "\' matrix with image " << std::endl;

  outfileYZ.close();
}
