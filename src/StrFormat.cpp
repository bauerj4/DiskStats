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
