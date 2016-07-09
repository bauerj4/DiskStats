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
