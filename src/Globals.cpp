#include "../include/Compile_Time_Options.h"
#include "../include/Context.h"
#include "../include/Snapshot.h"
#include "../include/Galaxy.h"

namespace Global{

  /*
    Global variables
  */

  int snapNum;

  Context context;

  Snapshot * newSnap;

  Galaxy * thisGalaxy;

  std::string snapName;
  
}
