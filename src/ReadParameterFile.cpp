#include "../include/Compile_Time_Options.h"
#include "../include/Context.h"
#include "../include/Globals.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

void Global::ReadParameterFile(char * FILENAME){
  std::ifstream file;
  std::string option;
  std::string value;
  std::string line;
  
  try{
    file.open(FILENAME);
    if (!file.is_open())
      throw std::exception();
  }
  catch (std::exception &e){
    std::cout << "Could not open parameter file.  Check if it exists. " << std::endl;
    exit(1);
  }
  
  while (!file.eof()){
    getline(file,line);
    std::stringstream ss(line);
    ss >> option;
    if (option == "\%")
      continue;
    ss >> value;

    /*
      Now create the context.
    */
  
    if (option == "Omega0")
      Global::context.Omega0 = stod(value);
    else if (option == "OmegaB")
      Global::context.OmegaB = stod(value);
    else if (option == "OmegaLambda")
      Global::context.OmegaLambda = stod(value);
    else if (option == "HubbleParam")
      Global::context.HubbleParam = stod(value);

    else if (option == "DiskShrinkingSphereRad")
      Global::context.DiskShrinkingSphereRad = stod(value);
    else if (option == "DiskShrinkingSphereCut")
      Global::context.DiskShrinkingSphereCut = stod(value);
    else if (option == "DiskShrinkingSphereRed")
      Global::context.DiskShrinkingSphereRed = stod(value);
    else if (option == "GalaxyX0")
      Global::context.GalaxyX0 = stod(value);
    else if (option == "GalaxyY0")
      Global::context.GalaxyY0 = stod(value);
    else if (option == "GalaxyZ0")
      Global::context.GalaxyZ0 = stod(value);
    else if (option == "GalaxyR0")
      Global::context.GalaxyR0 = stod(value);


    else if (option == "ComovingIntegration")
      Global::context.ComovingIntegration = stoi(value);
    else if (option == "PeriodicBoundaries")
      Global::context.PeriodicBoundaries = stoi(value);
    else if (option == "NumSnaps")
      Global::context.NumSnaps = stoi(value);

    else if (option == "HaloParticleType")
      Global::context.HaloParticleType = stoi(value);
    else if (option == "DiskParticleType")
      Global::context.DiskParticleType = stoi(value);
    else if (option == "BulgeParticleType")
      Global::context.BulgeParticleType = stoi(value);
    else if (option == "MultipleGalaxies")
      Global::context.MultipleGalaxies = stoi(value);

    else if (option == "SnapshotBase")
      Global::context.SnapshotBase = value;    
    else if (option == "GalaxyList")
      Global::context.GalaxyList = value;

  }
}


void Global::PrintParameters(){
  std::cout << "Analysis initiated with the following parameters: \n" << std::endl;
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "[Cosmology]\n" << std::endl;
  std::cout << "Omega0 = " << Global::context.Omega0 << std::endl;
  std::cout << "OmegaB = " << Global::context.OmegaB << std::endl;
  std::cout << "OmegaLambda = " << Global::context.OmegaLambda << std::endl;
  std::cout << "HubbleParam = " << Global::context.HubbleParam << std::endl;
  std::cout << std::endl;
  std::cout << "[File I/O]\n" << std::endl;
  std::cout << "SnapshotBase = " << Global::context.SnapshotBase << std::endl;
  std::cout << "NumSnaps = " << Global::context.NumSnaps << std::endl;
  std::cout << "-------------------------------------\n" << std::endl;
}
