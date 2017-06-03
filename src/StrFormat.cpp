#include "../include/Globals.h"
#include "../include/Compile_Time_Options.h"
#include <string>
#include <sstream>

/*
  Various functions for formatting strings
*/

std::string Global::GetGadgetSnapshotName(int i){
  std::string number, name;

  if (i < 10)
    number = "00" + std::to_string(i);
  else if (i >= 10 && i < 100)
    number = "0" + std::to_string(i);
  else
    number = std::to_string(i);

  name = Global::context.SnapshotBase + number;
  return name;
}


std::string Global::GetDiskXYDensityHistName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot + "/Disk_Density_XY.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);

}

std::string Global::GetDiskXZDensityHistName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot + "/Disk_Density_XZ.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);

}

std::string Global::GetDiskYZDensityHistName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_Density_YZ.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);

}


std::string Global::GetHaloXYDensityHistName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot + "/Halo_Density_XY.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);

}

std::string Global::GetHaloXZDensityHistName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot + "/Halo_Density_XZ.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);

}

std::string Global::GetHaloYZDensityHistName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Halo_Density_YZ.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);

}

std::string Global::GetDiskSurfaceDensityName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_Surface_Density.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}


std::string Global::GetDiskVerticalDensityName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_Vertical_Density.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}

std::string Global::GetDiskAbsVerticalDensityName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_Abs_Vertical_Density.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}


std::string Global::GetDiskAverageZName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_Average_Z.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}


std::string Global::GetDiskSigmaZ2VSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_Sigma_Z2VSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}


std::string Global::GetDiskSigmaR2VSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_Sigma_R2VSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}


std::string Global::GetDiskSigmaP2VSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_Sigma_P2VSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}


std::string Global::GetDiskSigmaRSigmaPVSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_Sigma_R_Sigma_PVSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}

std::string Global::GetDiskSigmaRSigmaZVSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_Sigma_R_Sigma_ZVSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}

std::string Global::GetDiskSigmaZSigmaPVSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_Sigma_Z_Sigma_PVSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}





std::string Global::GetDiskVPVSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_VPVSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}



std::string Global::GetDiskVRVSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_VRVSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}

std::string Global::GetDiskVZVSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_VZVSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}


std::string Global::GetDiskVP2VSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_VP2VSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}



std::string Global::GetDiskVR2VSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_VR2VSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}

std::string Global::GetDiskVZ2VSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_VZ2VSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}



std::string Global::GetDiskVRVPVSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_VRVPVSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}

std::string Global::GetDiskVRVZVSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_VRVZVSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}

std::string Global::GetDiskVZVPVSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_VZVPVSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}




std::string Global::GetDiskLZVSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_LZVSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}

std::string Global::GetDiskEnergyVSRName(int i){
  std::string base, number, tail;

  base = Global::context.PathToGnuplot +  "/Disk_EnergyVSR.";
  tail = ".ascii";
  number = std::to_string(i);

  return (base + number + tail);
}

