#include "../include/Compile_Time_Options.h"
#include "../include/Globals.h"
#include <vector>
#include <iostream>

/*
  General linear algebra routines go here.  
  The motivation for using our own is purely for OpenMP 
  parallelism and to dodge library dependencies for
  HPCVL.
*/


/*
  This function acts similarly to NumPy's linspace,
  but it returns an array of length n+1 (containing
  both boundaries a and b).
*/

std::vector<double> Global::LinearSpacing(double a, double b, int n){
  double dx;
  double width;
  std::vector<double> result;

  if (b < a){
    std::cout << "Error in LinearSpacing: b < a" << std::endl;
    exit(3);
  }
  else if (n == 0){
    std::cout << "Error in LinearSpaceing: Divide by 0" << std::endl;
    exit(4);
  }

  width = b - a;
  dx = width/n;
  
  for (int i = 0; i < n; i++){
    result.push_back(a + i*dx);
  }

  result.push_back(b);

  return result;

}
