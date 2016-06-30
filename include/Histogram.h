#ifndef HISTOGRAM_HEADER
#define HISTOGRAM_HEADER


#include "Compile_Time_Options.h"
#include <vector>

/*
  This specifies the histogram class.  The motivation
  for not using GSL's implementation is twofold:

  (1) Histogram binning is embarassingly parallel
  and lends itself well to an OpenMP implementation.

  (2) HPCVL (the cluster at Queen's) has embarassingly
  outdated compilers and libraries making it a 
  Herculean effort to actually install anything. Also,
  since Gadget 2 depends on GSL 1.16 and they are 
  currently at version 2.1 at the time of writing,
  it is better for cross-platform compatibility 
  to just avoid linking the library in the first place.
*/

template <typename T> class Histogram2D{
 private:
  std::vector<T> binsx;
  std::vector<T> binsy;
  std::vector<std::vector<int> > counts;
 public:
  // Constructor (need to write destructor to extend to objects)
  Histogram2D(std::vector<T> &datax, std::vector<T> &datay, std::vector<T> &binsx, std::vector<T> &binsy);

  // Return the counts
  std::vector<std::vector<int> > Counts(){return counts;}
  
  // Return the bin edges used
  std::vector<T> XBins(){return binsx;}
  std::vector<T> YBins(){return binsy;}

  // Print to space separated ascii file
  void PrintASCII(char * path);
};

#endif
