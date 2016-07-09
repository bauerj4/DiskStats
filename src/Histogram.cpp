#include "../include/Compile_Time_Options.h"
#include "../include/Globals.h"
#include "../include/Histogram.h"
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>

// 2D histogram constructor

template <typename T>
Histogram2D<T>::Histogram2D(std::vector<T> &datax, std::vector<T> &datay, \
			    std::vector<T> &binsx, std::vector<T> &binsy){

  std::pair<T,T> p;

  /*
    It is equally efficient to loop through the data and ask which bin each point is in
    as it is to loop through each bin and ask which points are in it.  For OpenMP,
    it is better if the outer loop has more elements (to allow for more threads)
    so we do the former.
  */

  if (datax.size() != datay.size()){
    std::cout << "Error in Histogram2D<T>(): dimensions of data are incompatible." << std::endl;
    exit(2);
  }
  
  /*
    Set empty counts
  */
  for (int i = 0; i < binsx.size() - 1; i++){
    std::vector<int> c;
    counts.push_back(c);
    for (int j = 0; j < binsy.size() - 1; j++){
      counts[i].push_back(0);
    }
  }

  for (int i = 0; i < datax.size(); i++){
    p.first  = datax[i];
    p.second = datay[i];

    for (int j = 0; j < binsx.size() - 1; j++){
      for (int k = 0; k < binsy.size() - 1; k++){
	if (p.first >= binsx[j] && p.first < binsx[j+1] &&\
	    p.second >= binsy[k] && p.second < binsy[k+1]){
	  counts[j][k]++;
	}
      }
    }
  }
}

template <typename T>
void Histogram2D<T>::PrintASCII(char * filepath){
  std::ofstream outfile(filepath);
  std::cout << "Printing a (" << counts.size() << "," << counts[0].size() << ") matrix.\n";
  for (int i = 0; i < counts.size(); i++){
    for (int j = 0; j < counts[0].size(); j++){
      if (j != counts[0].size() - 1)
	outfile << counts[j][i] << " ";
      else
	outfile << counts[j][i] << "\n";
    } 
  }
}

template <typename T>
void Histogram2D<T>::PrintASCII(std::string filepath){
  std::ofstream outfile(filepath.c_str());
  std::cout << "Printing a (" << counts.size() << "," << counts[0].size() << ") matrix.\n";
  for (int i = 0; i < counts.size(); i++){
    for (int j = 0; j < counts[0].size(); j++){
      if (j != counts[0].size() - 1)
	outfile << counts[j][i] << " ";
      else
        outfile << counts[j][i] << "\n";
    }
  }
}


template class Histogram2D<double>;
template class Histogram2D<int>;
