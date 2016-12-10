#include "../include/Compile_Time_Options.h"
#include "../include/Galaxy.h"
#include "../include/Components.h"
#include "../include/Snapshot.h"
#include "../include/Particle.h"
#include "../include/Globals.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <omp.h>
#include <math.h>
#include <algorithm>

bool operator< (Particle &a,Particle &b);

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

  /*
    Initialize empty MofI
  */
  for (int axis = 0; axis < 3; axis++){
    std::vector<double> newVector(3,0);
    diskMofI.push_back(newVector);
  }

  std::cout << "\n" << std::endl;
  std::cout << "Constructing galaxy with approximate position r = [" << x <<","<< y << ","<< z <<"]" << std::endl;
  for (int i = 0; i < np; i++){
    my_x = snap->PosX(i) - x;
    my_y = snap->PosY(i) - y;
    my_z = snap->PosZ(i) - z;
    
    my_vx = snap->VelX(i);
    my_vy = snap->VelY(i);
    my_vz = snap->VelZ(i);

    if (Global::context.ComovingIntegration == 1){
      my_x *= snap->Time();
      my_y *= snap->Time();
      my_z *= snap->Time();
      
      my_vx *= pow(snap->Time(), 1.5) / Global::context.HubbleParam;
      my_vy *= pow(snap->Time(), 1.5) / Global::context.HubbleParam;
      my_vz *= pow(snap->Time(), 1.5) / Global::context.HubbleParam;
    }

    my_m  = snap->Mass(i);
    my_id = snap->ID(i);
    my_t  = snap->Type(i);
    my_pot = snap->Pot(i);

    my_p[0] = my_x;
    my_p[1] = my_y;
    my_p[2] = my_z;
    
    my_v[0] = my_vx;
    my_v[1] = my_vy;
    my_v[2] = my_vz;

    if ((my_x)*(my_x) + (my_y)*(my_y) + (my_z)*(my_z) < r*r){
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

#ifdef OPENMP
#pragma omp parallel for private(x,y,z) reduction(+:x_bar,y_bar,z_bar,count)
#endif
  for (int i = 0; i < myDisk.P.size(); i++){
    x = myDisk.P[i]->PosX();
    y = myDisk.P[i]->PosY();
    z = myDisk.P[i]->PosZ();

    x_bar += x;
    y_bar += y;
    z_bar += z;
    count++;
  }

  std::cout << "x_bar, y_bar, z_bar, counts " << x_bar << ", " << y_bar << ", " \
	    << z_bar << ", " << count << std::endl;

  x_bar /= count;
  y_bar /= count;
  z_bar /= count;
  //#else
  //  x_bar = Global::xBar;
  //  y_bar = Global::yBar;
  //  z_bar = Global::zBar;
  //#endif

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

#ifdef OPENMP
#pragma omp parallel for private(x,y,z) reduction(+:x_sum, y_sum, z_sum, count)
#endif
    for (int i = 0; i < myDisk.P.size(); i++){
      x = myDisk.P[i]->PosX() - x_bar;
      y = myDisk.P[i]->PosY() - y_bar;
      z = myDisk.P[i]->PosZ() - z_bar;

      // Check if particle is in sphere

      if ((x-x_bar)*(x-x_bar) + (y-y_bar)*(y-y_bar) \
	  + (z-z_bar)*(z-z_bar) < r_cut*r_cut){
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
  Compute the disk moment of inertia;
  Align the system to the disk minor axis.
*/

#ifdef ROTATE_SYSTEM
void Galaxy::AlignToMinorAxis(){
  //std::vector<double> thisPos;
  //std::vector<double> thisVel;

  std::cout << "Realigning halo...\n";
  for (int i = 0; i < myHalo.P.size(); i++){
    myHalo.P[i]->Rotate3();
    //std::vector<double> thisPos;
    //std::vector<double> thisVel;

    //thisPos = myHalo.P[i]->R();
    //thisVel = myHalo.P[i]->V();

    //std::cout << "Pos before: " << std::endl;
    //std::cout << "[" << thisPos[0] << ", " << thisPos[1] << ", " << thisPos[2] << "\n";
    //Global::RotateToDiskFrame(thisPos);
    //Global::RotateToDiskFrame(thisVel);
    
    //std::cout << "Pos after: " << std::endl;
    //std::cout << "[" << thisPos[0] << ", " << thisPos[1] << ", " << thisPos[2] << "\n";

    //myHalo.P[i]->SetPos(thisPos);
    //myHalo.P[i]->SetVel(thisVel);
  }

  std::cout << "Realigning disk...\n";
  for (int i = 0; i < myDisk.P.size(); i++){
    myDisk.P[i]->Rotate3();
    
    //std::vector<double> thisPos;
    //std::vector<double> thisVel;

    //thisPos = myDisk.P[i]->R();
    //thisVel = myDisk.P[i]->V();

    //Global::RotateToDiskFrame(thisPos);
    //Global::RotateToDiskFrame(thisVel);

    //myDisk.P[i]->SetPos(thisPos);
    //myDisk.P[i]->SetVel(thisVel);
  }

  std::cout << "Realigning bulge...\n";
  for (int i = 0; i < myBulge.P.size(); i++){
    myBulge.P[i]->Rotate3();
    //std::vector<double> thisPos;
    //std::vector<double> thisVel;

    //thisPos = myBulge.P[i]->R();
    //thisVel = myBulge.P[i]->V();

    //Global::RotateToDiskFrame(thisPos);
    //Global::RotateToDiskFrame(thisVel);

    //myBulge.P[i]->SetPos(thisPos);
    //myBulge.P[i]->SetVel(thisVel);
  }
  std::cout << "Done with system realignment." << std::endl;
}
#endif

/*
  Compute the virial ratio
*/

double Galaxy::ComputeVirialRatio(){
  double potential, kinetic, verticalKineticEnergy, ratio, total_energy;

  potential = kinetic = verticalKineticEnergy = 0.;

  if (Global::context.StartingSnap == Global::snapNum){
    Global::verticalHeating0 = 0;
  }


  for (int i = 0; i < myHalo.P.size(); i++){
    if (Global::context.ComovingIntegration == 1){
      potential += 0.5 * myHalo.P[i]->M() * myHalo.P[i]->Potential() \
	* pow(Global::newSnap->Time(), -2.);
      kinetic += 0.5 * myHalo.P[i]->M() * myHalo.P[i]->VelX() \
	* myHalo.P[i]->VelX() * pow(Global::newSnap->Time(),0.0);
      kinetic += 0.5 * myHalo.P[i]->M() * myHalo.P[i]->VelY() \
	* myHalo.P[i]->VelY() * pow(Global::newSnap->Time(),0.0);
      kinetic += 0.5 * myHalo.P[i]->M() * myHalo.P[i]->VelZ() \
	* myHalo.P[i]->VelZ() * pow(Global::newSnap->Time(),0.0);	

    }
    else{
      potential += 0.5 * myHalo.P[i]->M() * myHalo.P[i]->Potential();
      kinetic += 0.5 * myHalo.P[i]->M() * myHalo.P[i]->VelX() * myHalo.P[i]->VelX();
      kinetic += 0.5 * myHalo.P[i]->M() * myHalo.P[i]->VelY() * myHalo.P[i]->VelY();
      kinetic += 0.5 * myHalo.P[i]->M() * myHalo.P[i]->VelZ() * myHalo.P[i]->VelZ();

    }
  }

  for (int i = 0; i < myDisk.P.size(); i++){
    if (Global::context.ComovingIntegration == 1){
      potential += 0.5 * myDisk.P[i]->M() * myDisk.P[i]->Potential() \
	* pow(Global::newSnap->Time(), -2.);
      kinetic += 0.5 * myDisk.P[i]->M() * myDisk.P[i]->VelX() \
        * myDisk.P[i]->VelX() * pow(Global::newSnap->Time(),0.0);
      kinetic += 0.5 * myDisk.P[i]->M() * myDisk.P[i]->VelY() \
        * myDisk.P[i]->VelY() * pow(Global::newSnap->Time(),0.0);
      kinetic += 0.5 * myDisk.P[i]->M() * myDisk.P[i]->VelZ() \
        * myDisk.P[i]->VelZ() * pow(Global::newSnap->Time(),0.0);

      verticalKineticEnergy += 0.5 * myDisk.P[i]->M() * myDisk.P[i]->VelZ() \
        * myDisk.P[i]->VelZ() * pow(Global::newSnap->Time(),0.0);
      if (Global::context.StartingSnap == Global::snapNum){
	Global::verticalHeating0 += 0.5 * myDisk.P[i]->M() * myDisk.P[i]->VelZ() \
          * myDisk.P[i]->VelZ() * pow(Global::newSnap->Time(),0.0);
      }


    }
    else{
      potential += 0.5 * myDisk.P[i]->M() * myDisk.P[i]->Potential();
      kinetic += 0.5 * myDisk.P[i]->M() * myDisk.P[i]->VelX() * myDisk.P[i]->VelX();
      kinetic += 0.5 * myDisk.P[i]->M() * myDisk.P[i]->VelY() * myDisk.P[i]->VelY();
      kinetic += 0.5 * myDisk.P[i]->M() * myDisk.P[i]->VelZ() * myDisk.P[i]->VelZ();
      verticalKineticEnergy += 0.5 * myDisk.P[i]->M() * myDisk.P[i]->VelZ() \
        * myDisk.P[i]->VelZ();
      if (Global::context.StartingSnap == Global::snapNum){
	Global::verticalHeating0 += 0.5 * myDisk.P[i]->M() * myDisk.P[i]->VelZ() \
          * myDisk.P[i]->VelZ();
      }
    }
  }
  
  for (int i = 0; i < myBulge.P.size(); i++){
    if (Global::context.ComovingIntegration == 1){
      potential += 0.5 * myBulge.P[i]->M() * myBulge.P[i]->Potential() *\
	pow(Global::newSnap->Time(), -2.);
      kinetic += 0.5 * myBulge.P[i]->M() * myBulge.P[i]->VelX() \
        * myBulge.P[i]->VelX() * pow(Global::newSnap->Time(),0.0);
      kinetic += 0.5 * myBulge.P[i]->M() * myBulge.P[i]->VelY() \
        * myBulge.P[i]->VelY() * pow(Global::newSnap->Time(),0.0);
      kinetic += 0.5 * myBulge.P[i]->M() * myBulge.P[i]->VelZ() \
        * myBulge.P[i]->VelZ() * pow(Global::newSnap->Time(),0.0);
    }
    else{
      potential += 0.5 * myBulge.P[i]->M() * myBulge.P[i]->Potential();
      kinetic += 0.5 * myBulge.P[i]->M() * myBulge.P[i]->VelX() * myBulge.P[i]->VelX();
      kinetic += 0.5 * myBulge.P[i]->M() * myBulge.P[i]->VelY() * myBulge.P[i]->VelY();
      kinetic += 0.5 * myBulge.P[i]->M() * myBulge.P[i]->VelZ() * myBulge.P[i]->VelZ();
    }
  }

  std::cout << "Time: " << Global::newSnap->Time() << std::endl;
  std::cout << "Total Energy: " << potential + kinetic << std::endl;
  std::cout << "Potential Energy: " << potential << std::endl;
  std::cout << "Kinetic Energy: " << kinetic << std::endl;
  ratio = potential / (2. * kinetic);
  
  Global::kineticEnergies.push_back(kinetic);
  Global::verticalHeating.push_back(verticalKineticEnergy / Global::verticalHeating0);
  Global::potentialEnergies.push_back(potential);
  Global::virialRatios.push_back(ratio);
  return ratio;
}


/*
  Get a list of the particle x positions
*/

std::vector<double> Galaxy::GetHaloXs(){
  std::vector<double> xVals;
  for (int i = 0; i < myHalo.P.size(); i++){
    xVals.push_back(myHalo.P[i]->PosX());
  }
  return xVals;
}


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


std::vector<double>  Galaxy::GetHaloYs(){
  std::vector<double> yVals;
  for (int i = 0; i < myHalo.P.size(); i++){
    yVals.push_back(myHalo.P[i]->PosY());
  }
  return yVals;
}

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

std::vector<double>  Galaxy::GetHaloZs(){
  std::vector<double> zVals;
  for (int i = 0; i < myHalo.P.size(); i++){
    zVals.push_back(myHalo.P[i]->PosZ());
  }
  return zVals;
}

std::vector<double>  Galaxy::GetDiskZs(){
  std::vector<double> zVals;
  for (int i = 0; i < myDisk.P.size(); i++){
    zVals.push_back(myDisk.P[i]->PosZ());
  }
  return zVals;
}



/*
  Compute the moment of inertia tensor
  for disk particles
*/

std::vector<std::vector<double> > Galaxy::GetDiskMofITensor(){
  std::vector<std::vector<double> > mofI(3,std::vector<double>(3,0.));
  std::vector<std::vector<double> > identity(3,std::vector<double>(3,0.));
  std::vector<std::vector<double> > inner(3,std::vector<double>(3,0.));
  std::vector<std::vector<double> > outer;
  std::vector<double> p;

  identity[0][0] = identity[1][1] = identity[2][2] = 1;
  
  std::cout << "Computing disk MofI... " << std::endl;

  for (unsigned int i = 0; i < myDisk.P.size(); i++){
    p = myDisk.P[i]->R();
    Global::SqrMatrixScalarMultiply(identity, Global::Dot(p,p),inner);
    outer = Global::Outer(p,p);
    Global::SqrMatrixScalarMultiply(inner, myDisk.P[i]->M(), inner);
    Global::SqrMatrixScalarMultiply(outer, myDisk.P[i]->M(), outer);

    Global::SqrMatrixAdd(mofI, inner, mofI);
    Global::SqrMatrixSubtract(mofI, outer, mofI);

    Global::SqrMatrixSubtract(inner,inner,inner);
  }
  std::cout << "Done." << std::endl;

  return mofI;
}


/*
  Compute angular momentum components
*/

void Galaxy::ComputeAngularMomentum(){
  std::vector<double> lHalo(3,0);
  std::vector<double> lDisk(3,0);
  std::vector<double> lBulge(3,0);
  std::vector<double> lInnerHalo(3,0);

  std::vector<double> diskInnerL(3,0);
  double r;
  std::vector<double> pTemp;


  std::cout << "Computing halo angular momentum..." << std::endl;
  for (unsigned int i = 0; i < myHalo.P.size(); i++){
    lHalo[0] += myHalo.P[i]->M() * (myHalo.P[i]->PosY() * myHalo.P[i]->VelZ() - \
				    myHalo.P[i]->PosZ() * myHalo.P[i]->VelY());
    lHalo[1] += myHalo.P[i]->M() * (myHalo.P[i]->PosZ() * myHalo.P[i]->VelX() - \
				    myHalo.P[i]->PosX() * myHalo.P[i]->VelZ());
    lHalo[2] += myHalo.P[i]->M() * (myHalo.P[i]->PosX() * myHalo.P[i]->VelY() - \
				    myHalo.P[i]->PosY() * myHalo.P[i]->VelX());

    pTemp = myHalo.P[i]->R();
    r = Global::Magnitude(pTemp);
    if (r < Global::context.HaloInnerRad){
      lInnerHalo[0] += myHalo.P[i]->M() * (myHalo.P[i]->PosY() * myHalo.P[i]->VelZ() - \
				      myHalo.P[i]->PosZ() * myHalo.P[i]->VelY());
      lInnerHalo[1] += myHalo.P[i]->M() * (myHalo.P[i]->PosZ() * myHalo.P[i]->VelX() - \
				      myHalo.P[i]->PosX() * myHalo.P[i]->VelZ());
      lInnerHalo[2] += myHalo.P[i]->M() * (myHalo.P[i]->PosX() * myHalo.P[i]->VelY() - \
				      myHalo.P[i]->PosY() * myHalo.P[i]->VelX());

    }
  }

  std::cout << "Computing disk angular momentum..." << std::endl;

  for (unsigned int i = 0; i < myDisk.P.size(); i++){
    lDisk[0] += myDisk.P[i]->M() * (myDisk.P[i]->PosY() * myDisk.P[i]->VelZ() - \
                                    myDisk.P[i]->PosZ() * myDisk.P[i]->VelY());
    lDisk[1] += myDisk.P[i]->M() * (myDisk.P[i]->PosZ() * myDisk.P[i]->VelX() - \
                                    myDisk.P[i]->PosX() * myDisk.P[i]->VelZ());
    lDisk[2] += myDisk.P[i]->M() * (myDisk.P[i]->PosX() * myDisk.P[i]->VelY() - \
                                    myDisk.P[i]->PosY() * myDisk.P[i]->VelX());

    std::vector<double> pTemp;
    pTemp = myDisk.P[i]->R();
    if (Global::Magnitude(pTemp) < INNER_RADIUS){
      diskInnerL[0] += myDisk.P[i]->M() * (myDisk.P[i]->PosY() * myDisk.P[i]->VelZ() - \
					   myDisk.P[i]->PosZ() * myDisk.P[i]->VelY());
      diskInnerL[1] += myDisk.P[i]->M() * (myDisk.P[i]->PosZ() * myDisk.P[i]->VelX() - \
					   myDisk.P[i]->PosX() * myDisk.P[i]->VelZ());
      diskInnerL[2] += myDisk.P[i]->M() * (myDisk.P[i]->PosX() * myDisk.P[i]->VelY() - \
					   myDisk.P[i]->PosY() * myDisk.P[i]->VelX());
    }
  }

  std::cout << "Computing bulge angular momentum..." << std::endl;

  for (unsigned int i = 0; i < myBulge.P.size(); i++){
    lBulge[0] += myBulge.P[i]->M() * (myBulge.P[i]->PosY() * myBulge.P[i]->VelZ() - \
				      myBulge.P[i]->PosZ() * myBulge.P[i]->VelY());
    lBulge[1] += myBulge.P[i]->M() * (myBulge.P[i]->PosZ() * myBulge.P[i]->VelX() - \
				      myBulge.P[i]->PosX() * myBulge.P[i]->VelZ());
    lBulge[2] += myBulge.P[i]->M() * (myBulge.P[i]->PosX() * myBulge.P[i]->VelY() - \
				      myBulge.P[i]->PosY() * myBulge.P[i]->VelX());
  }


  Global::lHalo.push_back(lHalo);
  Global::lDisk.push_back(lDisk);
  Global::lBulge.push_back(lBulge);
  Global::lInnerHalo.push_back(lInnerHalo);

  haloAngularMomentum  = lHalo;
  diskAngularMomentum  = lDisk;
  bulgeAngularMomentum = lBulge;

  diskInnerAngularMomentum = diskInnerL;
  diskAngularMomentumAxis  = diskInnerL;

  Global::Normalize(diskAngularMomentumAxis);
}


/*
  Removes unbound particles by assuming halo
  is dominant source of mass and constructing
  and approximation to local escape velocity
*/


bool operator< (Particle &a,Particle &b){
  std::vector<double> ra;
  std::vector<double> rb;

  ra = a.R();
  rb = b.R();

  return (ra[0]*ra[0] + ra[1] * ra[1] + ra[2] * ra[2] <
	  rb[0]*rb[0] + rb[1] * rb[1] + rb[2] * rb[2]);

}


void Galaxy::RemoveUnboundHaloParticles(double vxBar, double vyBar, double vzBar, double Rs){

  std::vector<double> mEnclosed; // Mass enclosed
  double m,vx,vy,vz,v2,x,y,z,r;
  
  /*
    Sort halo particles by radius
  */

  std::cout << "Sorting particles by radius..." << std::endl;
  std::sort(myHalo.P.begin(), myHalo.P.end());
  std::cout << "Sorted." << std::endl;

  /*
    If particle velocity^2 exceeds 2 G M / r,
    pop particle from vector.
  */

  for (int i = 0; i < myHalo.P.size(); i++){
    x  = myHalo.P[i]->PosX();
    y  = myHalo.P[i]->PosY();
    z  = myHalo.P[i]->PosZ();
    r  = sqrt(x*x + y*y + z*z);
    vx = myHalo.P[i]->VelX() - vxBar;
    vy = myHalo.P[i]->VelY() - vyBar;
    vz = myHalo.P[i]->VelZ() - vzBar;
    v2 = vx*vx + vy*vy + vz*vz;
    m  = myHalo.P[i]->M();

    if (v2 > 2. * 1.e10 * G * m * myHalo.P.size() / (0.5*Rs)){
      std::cout << "Removed particle at r = " << r << " with v = " << sqrt(v2) << " > " \
		<< sqrt(2. * 1.e10 * G * m * myHalo.P.size() / (0.5*Rs)) << std::endl;
      myHalo.P.erase(myHalo.P.begin() + i);
      i--;
    }
  }
}

/*
  Prints IDs of particles in the halo
*/

void Galaxy::PrintHaloIDs(char * file){
  std::ofstream f(file, std::ofstream::out);
  for (unsigned int i = 0; i < myHalo.P.size(); i++){
    f << myHalo.P[i]->MyID() << std::endl;
  }
}
