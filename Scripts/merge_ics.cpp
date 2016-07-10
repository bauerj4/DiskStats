#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <math.h>
#include <cstdlib>

/*
  Conversion script adapted from DiskStats code
  written by JSB
*/


#define GADGET2        // Currently no support for non-Gadget codes
#define USE_POTENTIALS // Comment this out if your snapshot
                       // doesn't have these


/*
  Class header for a single particle
*/

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

#ifdef USE_POTENTIALS
  std::vector<double> Potentials;
#endif

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
  double Pot(int idx){return Potentials[idx];}


  std::vector<double> Pos(int idx){return Positions[idx];}
  double PosX(int idx){return Positions[idx][0];}
  double PosY(int idx){return Positions[idx][1];}
  double PosZ(int idx){return Positions[idx][2];}

  std::vector<double> Vel(int idx){return Velocities[idx];}
  double VelX(int idx){return Velocities[idx][0];}
  double VelY(int idx){return Velocities[idx][1];}
  double VelZ(int idx){return Velocities[idx][2];}

  double Time(){return header.time;}
  double Redshift(){return header.redshift;}

  /*
    Load data
  */

  void LoadGadget2(char * FILE_PATH, int n_files); // Multiple snaps not supported
  void WriteGadget2(char * FILE_PATH);
  void AppendDiskParticles(std::vector<double> &disk_x, std::vector<double> &disk_y, \
			   std::vector<double> &disk_z, std::vector<double> &disk_vx, \
			   std::vector<double> &disk_vy, std::vector<double> &disk_vz,\
			   std::vector<double> &disk_m, std::vector<int> &disk_ids);


  /*
    Print header to terminal
  */

  void PrintGadget2Header();  

};

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

  fread(&header, sizeof(Gadget_Header), 1, file);

  // Get total particle number

  nparts = 0.;
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
      
      if (header.mpart_arr[type] == 0){
	fread(&emptyFloat, sizeof(float), 1, file);
	double newMass = (double)emptyFloat;
	Masses.push_back(newMass);
      }
      else{
	Masses.push_back(header.mpart_arr[type]);
      }
      Types.push_back(type);
    }
    
    // Read Fortran buffer

    fread(&gadgetFortranBuffer, 4, 1, file);

    

    /*
      Get internal energies (empty if no SPH)
    */
    std::cout << "Reading Block 6: Internal Energies" << std::endl;

    fread(&gadgetFortranBuffer, 4, 1, file);

    for (int i = 0; i < header.npart[0]; i++){
      fread(&emptyFloat, sizeof(float), 1, file);
      float newE = emptyFloat;
      //Potentials.push_back(newPot);   
      //std::cout << newE << std::endl;
    }

    fread(&gadgetFortranBuffer, 4, 1, file);


    /*
      Get density (empty if no SPH)
    */
    

    std::cout << "Reading Block 7: Densities" << std::endl;
    fread(&gadgetFortranBuffer, 4, 1, file);

    for (int i = 0; i < header.npart[0]; i++){
      fread(&emptyFloat, sizeof(float), 1, file);
      float newRho = emptyFloat;
      //Potentials.push_back(newPot);                                                                                                          
      //std::cout << newRho << std::endl;
    }
    

    fread(&gadgetFortranBuffer, 4, 1, file);

    /*
      Get smoothing (empty if no SPH)
    */

    std::cout << "Reading Block 8: Smoothing" << std::endl;
    fread(&gadgetFortranBuffer, 4, 1, file);

    for (int i = 0; i < header.npart[0]; i++){
      fread(&emptyFloat, sizeof(float), 1, file);
      float newSmooth = emptyFloat;
      //Potentials.push_back(newPot);                                                                                                          
      // std::cout << newSmooth << std::endl;
    }

    fread(&gadgetFortranBuffer, 4, 1, file);

    /*
      Get potentials
    */
    std::cout << "Reading Block 9: Potentials" << std::endl;

    fread(&gadgetFortranBuffer, 4, 1, file);

    for (int i = 0; i < nparts; i++){
      fread(&emptyFloat, sizeof(float), 1, file);
      float newPot = emptyFloat;
      //      std::cout << newPot << "," << Positions[i][0] << "," << Positions[i][1] \
      //		<< "," << Positions[i][2] << std::endl;
#ifdef USE_POTENTIALS
      if (newPot <= 0.){
	Potentials.push_back(newPot);
      }
      else{
	Potentials.push_back(-newPot);
      }
#endif
    }

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
  std::cout << "Type 1 (Halo): " << header.npart[1] << " (m=" << header.mpart_arr[1] << ")" << std::endl;
  std::cout << "Type 2 (Disk): " << header.npart[2] << " (m=" << header.mpart_arr[2] << ")" << std::endl;
  std::cout << "Type 3 (Bulge): " << header.npart[3] << " (m=" << header.mpart_arr[3] << ")" << std::endl;
  std::cout << "Type 4 (Other): " << header.npart[4] << " (m=" << header.mpart_arr[4] << ")" << std::endl;
  std::cout << "Type 5 (Boundary): " << header.npart[5] << " (m=" << header.mpart_arr[5] << ")" << std::endl;
}


/*
  Writethe snapshot
*/

void Snapshot::WriteGadget2(char * FILE_PATH){
  int gadgetFortranBuffer;
  std::string x,y,z,vx,vy,vz;
  // The out stream
  std::ofstream outFile (FILE_PATH, std::ios::out | std::ios::binary);

  /*
    Block 1 (header)
  */

  std::cout << "Writing block 1: Header" << std::endl;
  
  // Gadget expects the size of the block
  gadgetFortranBuffer = sizeof(header);
  
  outFile.write((char*)&gadgetFortranBuffer, sizeof(int));
  outFile.write((char*)&header, sizeof(header));
  outFile.write((char*)&gadgetFortranBuffer, sizeof(int));

  /*
    Block 2 (positions)
  */
  std::cout << "Writing block 2: Positions" << std::endl;

  gadgetFortranBuffer = (3 * sizeof(float) * Positions.size());  
  outFile.write((char*)&gadgetFortranBuffer, sizeof(int));
  for (int i = 0; i < Positions.size(); i++){
    x = Positions[i][0];
    y = Positions[i][1];
    z = Positions[i][2];
    outFile.write((char*)&x, sizeof(float));
    outFile.write((char*)&y, sizeof(float));
    outFile.write((char*)&z, sizeof(float));
  }
  outFile.write((char*)&gadgetFortranBuffer, sizeof(int));

  /*
    Block 3 (velocities)
  */

  std::cout << "Writing block 3: Velocities" << std::endl;

  gadgetFortranBuffer = (3 * sizeof(float) * Velocities.size());
  outFile.write((char*)&gadgetFortranBuffer, sizeof(int));
  for (int i = 0; i < Velocities.size(); i++){
    vx = Velocities[i][0];
    vy = Velocities[i][1];
    vz = Velocities[i][2];
    outFile.write((char*)&vx, sizeof(float));
    outFile.write((char*)&vy, sizeof(float));
    outFile.write((char*)&vz, sizeof(float));
  }
  outFile.write((char*)&gadgetFortranBuffer, sizeof(int));
  
  /*
    Block 4 (IDs)
  */

  std::cout << "Writing block 4: IDs" << std::endl;
  
  gadgetFortranBuffer = (sizeof(int) * IDs.size());
  outFile.write((char*)&gadgetFortranBuffer, sizeof(int));
  for (int i = 0; i < IDs.size(); i++){
    outFile.write((char*)&IDs[i], sizeof(int));
  }
  outFile.write((char*)&gadgetFortranBuffer, sizeof(int));

  /*
    Block 5 (masses)
  */

  std::cout << "Writing block 5: Masses" << std::endl;

  gadgetFortranBuffer =(sizeof(float) * IDs.size());
  outFile.write((char*)&gadgetFortranBuffer, sizeof(int));
  for (int i = 0; i < Masses.size(); i++){
    outFile.write((char*)&Masses[i], sizeof(float));
  }
  outFile.write((char*)&gadgetFortranBuffer, sizeof(int));

  outFile.close();
}


/*
  Add disk particles
*/

void Snapshot::AppendDiskParticles(std::vector<double> &disk_x, std::vector<double> &disk_y, \
				   std::vector<double> &disk_z, std::vector<double> &disk_vx, \
				   std::vector<double> &disk_vy, std::vector<double> &disk_vz,\
				   std::vector<double> &disk_m, std::vector<int> &disk_ids){
  std::vector<std::vector<double> > newPos;
  std::vector<std::vector<double> > newVel;
  std::vector<double> newM;
  std::vector<int> newIDs;
  std::vector<double>::iterator it;
  std::vector<std::vector<double> >::iterator it2;
  std::vector<int>::iterator it3;
  int npartsBeforeDisk;
  int npartsAfterDisk;

  npartsBeforeDisk = header.npart[0] + header.npart[1];

  for (int i = 0; i < disk_x.size(); i++){
    std::vector<double> newVector(3,0);
    newVector[0] = disk_x[i];
    newVector[1] = disk_y[i];
    newVector[2] = disk_z[i];
    newPos.push_back(newVector);
  }

  std::cout << "Size of new positions is " << newPos.size() << std::endl;

  for (int i = 0; i < disk_vx.size(); i++){
    std::vector<double> newVector(3,0);
    newVector[0] = disk_vx[i];
    newVector[1] = disk_vy[i];
    newVector[2] = disk_vz[i];
    newVel.push_back(newVector);
  }

  std::cout << "Size of new velocities is " << newVel.size() << std::endl;

  for (int i = 0; i < nparts; i++){
    newIDs.push_back(i + 1);
  }

  for (int i = 0; i < disk_m.size(); i++){
    newM.push_back(disk_m[i]);
  }

  header.npart[2] += disk_x.size();
  nparts += disk_x.size();
  header.mpart_arr[0] = header.mpart_arr[1] = header.mpart_arr[2] \
    = header.mpart_arr[3] = header.mpart_arr[4] = header.mpart_arr[5] = 0.;

  std::cout << "Arrays constructed. Merging IDs..." << std::endl;

  IDs = newIDs;

  std::cout << "Merging masses... " << std::endl;
  it = Masses.begin();
  Masses.insert(it+npartsBeforeDisk, newM.begin(), newM.end());
  std::cout << "New mass vector of length " << Masses.size() << std::endl;


  std::cout << "Merging positions... " << std::endl;
  it2 = Positions.begin();
  Positions.insert(it2+npartsBeforeDisk, newPos.begin(), newPos.end());
  std::cout << "New position vector of length " << Positions.size() << std::endl;
  
  std::cout << "Merging velocities... " << std::endl;
  it2 = Velocities.begin();
  Velocities.insert(it2+npartsBeforeDisk, newVel.begin(), newVel.end());
  std::cout << "New velocity vector of length " << Velocities.size() << std::endl;


  std::cout << "Arrays reconstructed." << std::endl;
}

/*
  Main loop
*/
int main(int argc, char ** argv){
  double timeFromRedshift, massUnitConversion;
  std::string x,y,z,vx,vy,vz,m;
  char * haloFile;
  char * diskFile;
  char * outFile;
  int nHaloParts, nDiskParts;
  std::string line;
  std::vector<double> disk_x,disk_y,disk_z;
  std::vector<double> disk_vx,disk_vy,disk_vz;
  std::vector<double> disk_m;
  std::vector<int> disk_ids;
  Snapshot * snap;
  std::fstream inDisk;


  haloFile = argv[1];
  diskFile = argv[2];
  outFile = argv[3];


  try{
    inDisk.open(diskFile);
    if (!inDisk.is_open())
      throw std::exception();
  }
  catch (std::exception &e){
    std::cout << "Could not open disk file.  Check if it exists. " << std::endl;
    exit(1);
  }



  massUnitConversion = 1./4.301;

  /*
    The halo file is a Gadget snapshot
  */
  
  snap = new Snapshot(haloFile,1);
  timeFromRedshift = 1./(1. + snap->Redshift());
  
  /*
    The disk file will be in GalactICS ASCII
  */

  getline(inDisk,line);
  std::stringstream iss0(line);
  iss0 >> nDiskParts;
  
  //  while (iss0.peek() == ' ')
    //    iss0 >> nDiskParts;
  std::cout << "\nReading " << nDiskParts << " disk particles from " << diskFile << std::endl;
  int id = 1;
  while (id <= nDiskParts){
    getline(inDisk,line);
    std::stringstream iss(line);
    //std::cout << "reading... " << std::endl;
    //    std::cout << line;
    
    //iss >> m >> x >> y >> z >> vx >> vy >> vz;
    iss >> m;
    //    while (iss.peek() == ' ')
    //      iss >> m;

    iss >> x;
    //    while (iss.peek() == ' ')
    //      iss >> x;

    iss >> y;
    //    while (iss.peek() == ' ')
    //    iss >> y;

    iss >> z;
    //  while (iss.peek() == ' ')
    //    iss >> z;

    iss >> vx;
    //  while (iss.peek() == ' ')
    //    iss >> vx;

    iss >> vy;
    //  while (iss.peek() == ' ')
    //      iss >> vy;

    iss >> vz;
    //    while (iss.peek() == ' ')
    //      iss >> vz;


    disk_m.push_back(atof(m.c_str()) * massUnitConversion);
    disk_x.push_back(atof(x.c_str()) / timeFromRedshift);
    disk_y.push_back(atof(y.c_str()) / timeFromRedshift);
    disk_z.push_back(atof(z.c_str()) / timeFromRedshift);
    disk_vx.push_back(atof(vx.c_str()) * 100. / pow(timeFromRedshift,0.5));
    disk_vy.push_back(atof(vy.c_str()) * 100. / pow(timeFromRedshift,0.5));
    disk_vz.push_back(atof(vz.c_str()) * 100. / pow(timeFromRedshift,0.5));
    disk_ids.push_back(id);
    std::cout << disk_x[id-1] << " " << disk_y[id - 1] << " " << " " << disk_z[id - 1] \
	      <<  " " << disk_vx[id-1] <<  " "  << disk_vy[id-1] << " " <<  disk_vz[id-1] \
	      << " " << disk_m[id-1] << std::endl;
    id++;
  }
  std::cout << "Disk file read." << std::endl;

  std::cout << "Appending disk particles..." << std::endl;

  snap->AppendDiskParticles(disk_x, disk_y, disk_z, disk_vx,\
			    disk_vy, disk_vz, disk_m, disk_ids);

  snap->WriteGadget2(outFile);

  delete snap;

  // Test reopen

  std::cout << "Testing by reopening..." << std::endl;
  snap = new Snapshot(outFile, 1);

  delete snap;

  return 0; //success
}
