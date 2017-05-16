#include "../include/Globals.h"
#include "../include/Compile_Time_Options.h"
#include "../include/Particle.h"
#include <vector>
#include <cmath>

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


std::vector<double> Particle::R(){
  std::vector<double> p(3,0);

  p[0] = Pos[0];
  p[1] = Pos[1];
  p[2] = Pos[2];
  
  return p;
}

std::vector<double> Particle::V(){
  std::vector<double> v(3,0);
  v[0] = Vel[0];
  v[1] = Vel[1];
  v[2] = Vel[2];
  
  return v;
}

void Particle::SetPos(std::vector<double> &p){
  Pos[0] = p[0];
  Pos[1] = p[1];
  Pos[2] = p[2];
}

void Particle::SetVel(std::vector<double> &v){
  Vel[0] = v[0];
  Vel[1] = v[1];
  Vel[2] = v[2];
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

void Particle::Rotate3(){
  std::vector<double> temp(3,0);
  temp[0] = Pos[0];
  temp[1] = Pos[1];
  temp[2] = Pos[2];
  
  Global::RotateToDiskFrame(temp);

  Pos[0] = temp[0];
  Pos[1] = temp[1];
  Pos[2] = temp[2];

  temp[0] = Vel[0];
  temp[1] = Vel[1];
  temp[2] = Vel[2];

  Global::RotateToDiskFrame(temp);

  Vel[0] = temp[0];
  Vel[1] = temp[1];
  Vel[2] = temp[2];

}


/*
  Calculate VR magnitude for a particle
*/

double Particle::VelR(){
  double r,vr;
  
  r  = sqrt(Pos[0]*Pos[0] + Pos[1] * Pos[1]);
  vr = (Pos[0] * Vel[0] + Pos[1] * Vel[1])/r;

  return vr;
}

/*
  Calculate VPhi magnitude for a particle
*/

double Particle::VelPhi(){
  double r;

  r       = sqrt(Pos[0]*Pos[0] + Pos[1] * Pos[1]);
  return (-Vel[0] * Pos[1]  + Vel[1] * Pos[0])/r;
}
