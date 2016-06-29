#include "../include/Globals.h"
#include "../include/Compile_Time_Options.h"
#include "../include/Particle.h"
#include <vector>

/*
  Implementation for the particle class
*/

Particle::Particle(std::vector<double> &p, std::vector<double> &v, double m, double rho, double pot, int id, int t){
  Pos[0] = p[0];
  Pos[1] = p[1];
  Pos[2] = p[2];
  Vel[0] = v[0];
  Vel[1] = v[1];
  Vel[2] = v[2];
  Rho = rho;
  Pot = pot;
  Mass = m;
  ID = id;
  Type=t;
}


void Particle::AddPos(double x, double y, double z){
  Pos[0] += x;
  Pos[1] += y;
  Pos[2] += z;
}

void Particle::AddVel(double vx, double vy, double vz){
  Vel[0] += vx;
  Vel[1] += vy;
  Vel[2] += vz;
}

void Particle::Rotate3(std::vector<std::vector<double> > &Euler){
}

