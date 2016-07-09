#include "../include/Compile_Time_Options.h"
#include "../include/Galaxy.h"
#include "../include/Components.h"
#include "../include/Snapshot.h"
#include "../include/Particle.h"
#include "../include/Globals.h"

#include <vector>
#include <iostream>

/*
  Class constructor
*/

Galaxy::Galaxy(double x, double y, double z, double r, Snapshot * snap){
  int np, my_t, my_id, n_galaxy, n_disk, n_halo, n_bulge;
  double my_x,my_y,my_z,my_vx,my_vy,my_vz,my_m,my_rho,my_pot;
  std::vector<double> my_p(3,0);
  std::vector<double> my_v(3,0);
  Particle * newParticle;
  /*
    Go through snap and add particles to galaxy that are
    within a radius r of position (x,y,z).
  */

  np = snap->NParts();
  n_disk = n_bulge = n_halo = n_galaxy = 0;

  std::cout << "\n" << std::endl;
  std::cout << "Constructing galaxy with approximate position r = [" << x <<","<< y << ","<< z <<"]" << std::endl;
  for (int i = 0; i < np; i++){
    my_x = snap->PosX(i);
    my_y = snap->PosY(i);
    my_z = snap->PosZ(i);
    
    my_vx = snap->VelX(i);
    my_vy = snap->VelY(i);
    my_vz = snap->VelZ(i);

    my_m  = snap->Mass(i);
    my_id = snap->ID(i);
    my_t  = snap->Type(i);

    my_p[0] = my_x;
    my_p[1] = my_y;
    my_p[2] = my_z;
    
    my_v[0] = my_vx;
    my_v[1] = my_vy;
    my_v[2] = my_vz;

    if (my_x*my_x + my_y*my_y + my_z*my_z < r*r){
      newParticle = new Particle(my_p, my_v, my_m, my_rho, my_pot, my_id, my_t);
      
      if (my_t == Global::context.HaloParticleType){
	myHalo.P.push_back(newParticle);
	n_halo++;
      }
      else if (my_t == Global::context.DiskParticleType){
	myDisk.P.push_back(newParticle);
	n_disk++;
      }
      else if (my_t == Global::context.BulgeParticleType){
	myBulge.P.push_back(newParticle);
	n_bulge++;
      }
      n_galaxy++;
    }
  }
  std::cout << "Galaxy constructed with " << n_galaxy << " particles" << std::endl;
  std::cout << "Halo:  " << n_halo << std::endl;
  std::cout << "Disk:  " << n_disk << std::endl;
  std::cout << "Bulge: " << n_bulge << std::endl; 
  std::cout << "\n";
}


/*
  Class destructor; must delete all particle objects
*/

Galaxy::~Galaxy(){
  for (int i = 0; i < myHalo.P.size(); i++){
    delete myHalo.P[i];
  }

  for (int i = 0; i < myDisk.P.size(); i++){
    delete myDisk.P[i];
  }

  for (int i = 0; i < myBulge.P.size(); i++){
    delete myHalo.P[i];
  }
}


/*
  Recenter the galaxy on the center of the disk. Do 
  this by performing a series of "shrinking spheres"
  and calculate the centroid of each until the 
  radius is below the cutoff specified in the 
  parameter file.
*/


void Galaxy::CenterOnDiskCentroid(){
  double x_bar, y_bar, z_bar, x, y, z, x_sum, y_sum, z_sum;
  int count;

  std::cout << "Centering on disk..." << std::endl;
  x_bar = y_bar = z_bar = 0.;
  count = 0;

  /*
    Our first guess will be the arithmetic mean.  So we 
    compute it here first.
  */

  for (int i = 0; i < myDisk.P.size(); i++){
    x = myDisk.P[i]->PosX();
    y = myDisk.P[i]->PosY();
    z = myDisk.P[i]->PosZ();

    x_bar += x;
    y_bar += y;
    z_bar += z;
    count++;
  }

  x_bar /= count;
  y_bar /= count;
  z_bar /= count;


  /*
    Now that the mean is computed, use it as a guess.
  */

  for (double r_cut = Global::context.DiskShrinkingSphereRad; \
       r_cut > Global::context.DiskShrinkingSphereCut;\
       r_cut /= Global::context.DiskShrinkingSphereRed){

    std::cout << "Shrinking sphere: " << r_cut << " ,[" << x_bar << "," << \
      y_bar << "," << z_bar << "]" <<std::endl;



    // Reset variables

    count = 0;
    x_sum = y_sum = z_sum = 0.;

    for (int i = 0; i < myDisk.P.size(); i++){
      x = myDisk.P[i]->PosX() - x_bar;
      y = myDisk.P[i]->PosY() - y_bar;
      z = myDisk.P[i]->PosZ() - z_bar;

      // Check if particle is in sphere

      if (x*x + y*y + z*z < r_cut){
	x_sum += x;
	y_sum += y;
	z_sum += z;
	count++;
      }
    }
    x_bar = x_sum/count;
    y_bar = y_sum/count;
    z_bar = z_sum/count;
  }

  std::cout << "Shifting halo positions by shrinking sphere coordinates..." << std::endl;

  for (int i = 0; i < myHalo.P.size(); i++){
    myHalo.P[i]->AddPos(-x_bar, -y_bar, -z_bar);
  }

  std::cout << "Shifting disk positions by shrinking sphere coordinates..." << std::endl;

  for (int i = 0; i < myDisk.P.size(); i++){
    myDisk.P[i]->AddPos(-x_bar, -y_bar, -z_bar);
  }

  std::cout << "Shifting bulge positions by shrinking sphere coordinates..." << std::endl;

  for (int i = 0; i < myBulge.P.size(); i++){
    myBulge.P[i]->AddPos(-x_bar, -y_bar, -z_bar);
  }

  std::cout << "Positions shifted." << std::endl;
  
}



/*
  Get a list of the particle x positions
*/

std::vector<double> Galaxy::GetDiskXs(){
  std::vector<double> xVals;
  for (int i = 0; i < myDisk.P.size(); i++){
    xVals.push_back(myDisk.P[i]->PosX());
  }
  return xVals;
}

/*
  Get a list of the particle y positions
*/

std::vector<double>  Galaxy::GetDiskYs(){
  std::vector<double> yVals;
  for (int i = 0; i < myDisk.P.size(); i++){
    yVals.push_back(myDisk.P[i]->PosY());
  }
  return yVals;
}

/*
  Get a list of the particle z positions
*/

std::vector<double>  Galaxy::GetDiskZs(){
  std::vector<double> zVals;
  for (int i = 0; i < myDisk.P.size(); i++){
    zVals.push_back(myDisk.P[i]->PosZ());
  }
  return zVals;
}


