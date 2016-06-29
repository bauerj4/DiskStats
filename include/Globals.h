#include "Compile_Time_Options.h"
#include "Context.h"
#include "Snapshot.h"
#include "Galaxy.h"
#include <string>

namespace Global{

  /*
    Global variables
  */

  extern int snapNum;
  
  extern Snapshot * newSnap;

  extern Galaxy * thisGalaxy;

  extern Context context;

  extern std::string snapName;

  /*
    Prototypes for global functions
  */

  void ReadParameterFile(char * FILENAME);
  void PrintParameters();

  std::string GetGadgetSnapshotName(int i);

  
}
