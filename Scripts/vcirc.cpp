#include "../include/Snapshot.h"
#include "../include/Galaxy.h"
#include <stdio.h>
#include <cmath.h>
#include <iostream>
#include <unordered_map>


/*
  Take a Gadget snapshot and approximate the
  rotation curve from the disk and halo particles
*/

int main(int argc, char ** argv){
  

}


/*
  Iteratively compute the l-th Legendre polynomial at x.

  Do this recursively using the Bonnet recursion relation.
*/

double polyLegendre(int l, double x){
  double legendre0, legendre1, legendreL;
  std::unordered_map<int,double> pn; // past computed values

  legendre0 = 1;
  legendre1 = x;
  legendreL = 0;

  pn.insert({0,legendre0});
  pn.insert({1,legendre1});

  if (l == 0)
    return legendre0;
  else if (l == 1)
    return legendre1;
  else if (pn.find(l) != pn.end()){
  }
  else{
    for (int i = 2; i < l + 1; i++){
      legendreL = (2 * l - 1)*x/l * polyLegendre(l - 1,x)/l -\
	(l - 1)/l * polyLegendre(l - 2,x);
      pn.insert({i,legendreL});
    }
  }

  return legendreL;
}



/*
  Force calculation at s,z.  We have fr and fz as the radial
  and vertical forces in an axisymmetric potential, -psi.
*/

void force(double s, double z, std::vector<double> &fr, \
	   std::vector<double> &fz, std::vector<double> &psi){



}

