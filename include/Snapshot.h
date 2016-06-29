#include "Compile_Time_Options.h"
#include <vector>

/*
  Class header for a single particle
*/

#ifndef SNAPSHOT_HEADER
#define SNAPSHOT_HEADER

struct Gadget_Header{
  int npart[6];
  double mpart_arr[6];
  double time;
  double redshift;
  int flag_sfr;
  int flag_feedback;
  int npart_cum[6];
  int num_snap_files;
  double boxSize;
  double omega0;
  double omegaLambda;
  double hubbleParam;
  char fill[96];  // The total header size is always 256 bytes (see user guide)
};//header;


class Snapshot{
 private:
  Gadget_Header header;
  int nparts;

  std::vector<int> IDs;
  std::vector<int> Types;
  std::vector<double> Masses;
  std::vector<std::vector<double> > Positions;
  std::vector<std::vector<double> > Velocities;
  std::vector<double> Densities;

 public:
  Snapshot(char * FILE_PATH, int n_files);

  /*
    Get private data
  */

  int NParts(){return nparts;}

  int ID(int idx){return IDs[idx];}
  int Type(int idx){return Types[idx];}
  double Mass(int idx){return Masses[idx];}
  double Rho(int idx){return Densities[idx];}


  std::vector<double> Pos(int idx){return Positions[idx];}
  double PosX(int idx){return Positions[idx][0];}
  double PosY(int idx){return Positions[idx][1];}
  double PosZ(int idx){return Positions[idx][2];}

  std::vector<double> Vel(int idx){return Velocities[idx];}
  double VelX(int idx){return Velocities[idx][0];}
  double VelY(int idx){return Velocities[idx][1];}
  double VelZ(int idx){return Velocities[idx][2];}


  /*
    Load data
  */

  void LoadGadget2(char * FILE_PATH, int n_files); // Multiple snaps not supported

  /*
    Print header to terminal
  */

  void PrintGadget2Header();  

};
#endif
