#include "../include/Compile_Time_Options.h"
#include "../include/Context.h"
#include "../include/Snapshot.h"
#include "../include/Globals.h"
#include "../include/Galaxy.h"
#include <string>
#include <iostream>

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

  /*
    Loop over snapshots.
  */

  for (Global::snapNum = 0; Global::snapNum < Global::context.NumSnaps; Global::snapNum++){
    Global::snapName = "";

    // Gadget has a weird snapshot name format
#ifdef GADGET2
    Global::snapName = Global::GetGadgetSnapshotName(Global::snapNum);
#else
    Global::snapName = Global::context.SnapshotBase + std::to_string(Global::snapNum);
#endif

    // Create and load new snapshot data
    Global::newSnap = new Snapshot((char*)Global::snapName.c_str(), 1);

    // Create the galaxy

    Global::thisGalaxy = new Galaxy(Global::context.GalaxyX0, Global::context.GalaxyY0,\
				    Global::context.GalaxyZ0, Global::context.GalaxyR0,\
				    Global::newSnap);
#ifdef CENTER_ON_DISK_CENTROID
    Global::thisGalaxy->CenterOnDiskCentroid();
#endif

    // Make density histograms

    // Free memory when we're done with snapshot
    delete Global::thisGalaxy;
    delete Global::newSnap;
  }

  return 0;  // Success
}
