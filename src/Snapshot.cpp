#include "../include/Snapshot.h"
#include "../include/Compile_Time_Options.h"
#include "../include/Globals.h"
#include <vector>
#include <iostream>
#include <stdlib.h>

/*
  The constructor
*/
Snapshot::Snapshot(char * FILENAME, int n_snaps){

  #ifdef GADGET2
  LoadGadget2(FILENAME, n_snaps);
  #endif
  
}


/*
  Load a Gadget2 snapshot
*/

void Snapshot::LoadGadget2(char * FILENAME, int n_snaps){
  

  FILE * file;
  int gadgetFortranBuffer; // See Gadget user manual for what this is
  
  float empty3Array[3];
  float emptyFloat;
  int emptyInt;
  int needMassArr;
  int type;
  int cumSum;

  /*
    Try opening the file
  */

  try{
    if(!(file = fopen(FILENAME, "r"))){
      throw std::exception();
    }
  }
  catch(std::exception &e){
    std::cout << "Exception: Cannot open file " << FILENAME << " for reading." << std::endl;
    exit(1);
  }

  std::cout << "Opened file " << FILENAME << " for reading.\n" << std::endl;


  /*
    Header block
  */

  std::cout << "Reading block 1: Header" << std::endl;

  // Read Fortran block buffer

  fread(&gadgetFortranBuffer, 4, 1, file);

  // Read the header

  fread(&header, sizeof(header), 1, file);

  // Get total particle number

  nparts = 0;
  for (int i = 0; i < 6; i++){
    nparts += header.npart[i];
  }

  // Read Fortran block buffer
  
  fread(&gadgetFortranBuffer, 4, 1, file);

  /*
    Position block
  */

  std::cout << "Reading block 2: Positions" << std::endl;

  // Read Fortran block buffer

  fread(&gadgetFortranBuffer, 4, 1, file);

  // Read the positions

  for (int i = 0; i < nparts; i++){
    fread(empty3Array, sizeof(float), 3, file);
    std::vector<double> newPos(3,0);
    newPos[0] = (double) empty3Array[0];
    newPos[1] = (double) empty3Array[1];
    newPos[2] = (double) empty3Array[2];
    Positions.push_back(newPos);

    //    std::cout << "p = [" << newPos[0] << "," << newPos[1] << "," << newPos[2] << "]" << std::endl;
  }

  // Read Fortran block buffer

  fread(&gadgetFortranBuffer, 4, 1, file);


  /*
    Read velocities
  */

  std::cout << "Reading block 3: Velocities" << std::endl;


  // Read Fortran block buffer

  fread(&gadgetFortranBuffer, 4, 1, file);

  // Get velocities 

  for (int i = 0; i < nparts; i++){
    fread(empty3Array, sizeof(float), 3, file);
    std::vector<double> newVel(3,0);
    newVel[0] = (double) empty3Array[0];
    newVel[1] = (double) empty3Array[1];
    newVel[2] = (double) empty3Array[2];
    Velocities.push_back(newVel);
    //    std::cout << "v = [" << newVel[0] << "," << newVel[1] << "," << newVel[2] << "]" << std::endl; 
  }
  
  // Read Fortran block buffer

  fread(&gadgetFortranBuffer, 4, 1, file);

  /*
    Get IDs
  */

  std::cout << "Reading block 4: IDs" << std::endl;

  // Read Fortran block buffer

  fread(&gadgetFortranBuffer, 4, 1, file);

  // Get IDs

  for (int i = 0; i < nparts; i++){
    fread(&emptyInt, sizeof(int), 1, file);
    int newID = emptyInt;
    IDs.push_back(newID);
    //    std::cout << "ID: " << newID << std::endl;
  }

  // Read Fortran block buffer

  fread(&gadgetFortranBuffer, 4, 1, file);

  /*
    Masses if needed
  */

  needMassArr = 0;
  for (int i = 0; i < 6; i++){
    if (header.npart[i] != 0 && header.mpart_arr[i] == 0)
      needMassArr = 1;
  }

  if (needMassArr == 1){
    std::cout << "Reading block 5: Masses" << std::endl;

    // Read Fortran buffer

    fread(&gadgetFortranBuffer, 4, 1, file);

    // Get masses

    type = 0;
    for (int i = 0; i < nparts; i++){
      
      int j = 0;
      cumSum = 0;
      while (i >= cumSum){
	cumSum += header.npart[j];
	j++;
      }

      type = j - 1;
    
      

      fread(&emptyFloat, sizeof(float), 1, file);
      double newMass = (double)emptyFloat;
      Masses.push_back(newMass);
      Types.push_back(type);
      //      std::cout << "Mass, Type: " << newMass << "," << type << std::endl;   
    }
    

    // Read Fortran buffer

    fread(&gadgetFortranBuffer, 4, 1, file);


  }

  /*
    Print snapshot summary
  */

#ifdef GADGET2
  std::cout << "\n" << std::endl;
  PrintGadget2Header();
#endif
}

void Snapshot::PrintGadget2Header(){
  std::cout << "Type 0 (Gas): " << header.npart[0] << " (m=" << header.mpart_arr[0] << ")" << std::endl;
  std::cout << "Type 1 (Halo): " << header.npart[2] << " (m=" << header.mpart_arr[1] << ")" << std::endl;
  std::cout << "Type 2 (Disk): " << header.npart[2] << " (m=" << header.mpart_arr[2] << ")" << std::endl;
  std::cout << "Type 3 (Bulge): " << header.npart[3] << " (m=" << header.mpart_arr[3] << ")" << std::endl;
  std::cout << "Type 4 (Other): " << header.npart[4] << " (m=" << header.mpart_arr[4] << ")" << std::endl;
  std::cout << "Type 5 (Boundary): " << header.npart[5] << " (m=" << header.mpart_arr[5] << ")" << std::endl;
}
