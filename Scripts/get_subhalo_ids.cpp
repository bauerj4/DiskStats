/*
  This script takes a subhalo position and returns 
  a list of bound particle IDs.

  Usage:
  
  get_subhalo_ids snapshot_file M X Y Z VX VY VZ R id_list 

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

int main(int argc, char ** argv){
  double X,Y,Z,VX,VY,VZ,M,RV,RS;
  std::vector<int> boundIDs; // The IDs of bound particles.
  std::vector<double> sortedRadii; // The sorted radii to determine potential
  std::vector<double> sphPotential; // The spherically averaged potential

  M  = atof(argv[2]);
  X  = atof(argv[3]);
  Y  = atof(argv[4]);
  Z  = atof(argv[5]);
  VX = atof(argv[6]);
  VY = atof(argv[7]);
  VZ = atof(argv[8]);
  RV  = atof(argv[9]); 
  RS  = atof(argv[10]);

  Global::context.HaloParticleType = 1; // Halo is type 1

  Snapshot * snap = new Snapshot(argv[1], 0);
  Galaxy * subhalo = new Galaxy(X,Y,Z,RV,snap);
  subhalo->RemoveUnboundHaloParticles(VX,VY,VZ,RS);
  subhalo->PrintHaloIDs(argv[11]);

  return 0; // Success
}
