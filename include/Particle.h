#ifndef PARTICLE_HEADER
#define PARTICLE_HEADER

#include <vector>

class Particle{
 private:
  double Pos[3];
  double Vel[3];
  double Rho;
  double Pot;
  double Mass;
  int ID;
  int Type;
  
 public:

  /*
    Constructor
  */

  Particle(std::vector<double> &p, std::vector<double> &v, double m, double rho, double pot, int id, int t);


  double PosX(){return Pos[0];}
  double PosY(){return Pos[1];}
  double PosZ(){return Pos[2];}

  double VelX(){return Vel[0];}
  double VelY(){return Vel[1];}
  double VelZ(){return Vel[2];}

  double Potential(){return Pot;}
  double M(){return Mass;}

  /*
    Add to position or velocity vectors
  */
  void AddPos(double x, double y, double z);
  void AddVel(double vx, double vy, double vz);

  /*
    Rotation operations
  */
  
  void Rotate3(std::vector<std::vector<double> > &Euler);
  
};

#endif
