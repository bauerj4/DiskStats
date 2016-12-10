/*
  This script takes a list of ids and finds
  the phase space info of those particles

  Usage:
  
  get_subhalo_ids snapshot_file id_list out_list 

  for a subhalo at (X,Y,Z).  The R parameter determines
  a truncation radius for the boundedness search.
*/

#include "../include/Snapshot.h"
#include "../include/Galaxy.h"
#include "../include/Particle.h"
#include "../include/Globals.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void WritePhaseInfo(char * out_file, std::vector<std::vector<double> > &info, double time);
void ReadIDFile(char * filepath, std::vector<int> &ids);
int isInIDList(int i, std::vector<int> &ids);


int main(int argc, char ** argv){
  double X,Y,Z,VX,VY,VZ,M,t;
  int N, snapID, idIndex;
  std::vector<int> boundIDs; // The IDs of bound particles.
  std::vector<double> subX;
  std::vector<double> subY;
  std::vector<double> subZ;
  std::vector<double> subVX;
  std::vector<double> subVY;
  std::vector<double> subVZ;
  std::vector<double> subM;
  std::vector<std::vector<double> > phaseInfo;
  ReadIDFile(argv[2], boundIDs);
  Snapshot * snap = new Snapshot(argv[1], 0);

  
  t = snap->Time();
  N = snap->NParts();
  
  /*
    We now loop over ID list to get indices of
    the specified particles.
  */

  for (int i = 0; i < N; i ++){
    snapID  = snap->ID(i);
    idIndex = isInIDList(snapID, boundIDs);

    if (idIndex != -1){
      std::vector<double> thisPhase(7,0);
      X  = snap->PosX(i);
      Y  = snap->PosY(i);
      Z  = snap->PosZ(i);
      VX = snap->VelX(i);
      VY = snap->VelY(i);
      VZ = snap->VelZ(i);
      M  = snap->Mass(i);
      
      thisPhase[0] = M;
      thisPhase[1] = X;
      thisPhase[2] = Y;
      thisPhase[3] = Z;
      thisPhase[4] = VX;
      thisPhase[5] = VY;
      thisPhase[6] = VZ;
      phaseInfo.push_back(thisPhase);
    }
  }

  WritePhaseInfo(argv[3], phaseInfo,t);
  return 0; // Success
}

void WritePhaseInfo(char * out_file, std::vector<std::vector<double> > &info, double time){
  std::ofstream f(out_file, std::ofstream::out);
  f << time << std::endl;
  for (int i = 0; i < info.size(); i++){
    f << info[i][0] << " " << info[i][1] << " " << info[i][2] << " " << info[i][3] \
      << " " << info[i][4] << " " << info[i][5] << " " << info[i][6] << std::endl;
  }
  f.close();
}

int isInIDList(int i, std::vector<int> &ids){
  int isIn = -1;

  for (int j = 0; j < ids.size(); j++){
    if (i == ids[j]){
      isIn = j;
    }
  }

  return isIn;
}

void ReadIDFile(char * filepath, std::vector<int> &ids){
  std::ifstream file;
  std::string line;
  int value;
  try{
    file.open(filepath);
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
    ss >> value;
    std::cout << value << std::endl;
    ids.push_back(value);
  }
}
