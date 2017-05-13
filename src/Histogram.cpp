#include "../include/Compile_Time_Options.h"
#include "../include/Globals.h"
#include "../include/Histogram.h"
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>

// 1D histogram constructor

template <typename T>
Histogram1D<T>::Histogram1D(std::vector<T> &data, std::vector<T> &xbins, int lognorm){

  /*
    We are going to create a binary search tree via the STL map
    class.  The map will associate the bin edges with a key,
    and each data element will find the bin index corresponding
    to the ceiling of that element.  In the worst case where
    the element is compared against all keys, this is an order
    N operation.  The average for BST lookup is N log N.
  */

  std::map<double, int> histLookup;
  counts = std::vector<double>(xbins.size() - 1, 0);
  int i;
  std::pair<double,int> p;
  double c;

  bins = xbins;

  for (auto it = bins.begin(); it != bins.end(); ++it){
    i = it - bins.begin();
    histLookup.insert(std::pair<double,int>(bins[i], i));
  }

  /*
    Now use the map to perform a series of BST searches
    for the data elements in an average of N log N time.
  */


  for (auto val: data){
    auto indexIt = histLookup.upper_bound(val);
    i = indexIt->second;
    if (i > 0 && indexIt != histLookup.end())
      counts[i-1]++;
  }

  /*
    Now logscale the counts
  */

  if (lognorm == 1){
    for (auto it = counts.begin(); it != counts.end(); ++it){
      c = (double) *it;
      c = log10(c);
      *it = c;
    }
  }

  
}

// Overloaded constructor with weights

template <typename T>
Histogram1D<T>::Histogram1D(std::vector<T> &data, std::vector<T> &xbins,\
			    int lognorm, std::vector<double> weights){

  std::map<double, int> histLookup;
  std::map<int, int> actualCounts;
  counts = std::vector<double>(xbins.size() - 1, 0);
  int i;
  double c;
  std::pair<double,int> p;

  bins = xbins;

  /*
    If we have weighted by each bin in advance...
   */
  if (weights.size() == bins.size() - 1){
    std::cout << "Weights are by bin..." << std::endl;
    for (auto it = bins.begin(); it != bins.end(); ++it){
      i = it - bins.begin();
      histLookup.insert(std::pair<double,int>(bins[i], i));
    }

    for (auto val: data){
      auto indexIt = histLookup.upper_bound(val);
      i = indexIt->second;
      if (i > 0 && indexIt != histLookup.end())
	counts[i-1] += 1;
    }

    if (lognorm == 1){
      for (auto it = counts.begin(); it != counts.end(); ++it){
	c = (double) *it;
	c = log10(c * weights[it - counts.begin()]);
	*it = c;
      }
    }
  }
  
  /*
    Or if each point is weighted...
  */

  else if (weights.size() == data.size()){
    std::cout << "Weights are by point..." << std::endl;

    for (auto it = bins.begin(); it != bins.end(); ++it){
      i = it - bins.begin();
      histLookup.insert(std::pair<double,int>(bins[i], i));
    }

    int j = 0;
    for (auto val: data){
      auto indexIt = histLookup.upper_bound(val);
      i = indexIt->second;
      if (i > 0 && indexIt != histLookup.end()){
        counts[i-1] += weights[j];
	//std::cout << val << " " <<  weights[j] << std::endl;
	if (actualCounts.find(i - 1) != actualCounts.end())
	  actualCounts.at(i-1)++;
	else
	  actualCounts.insert({i-1,1});
      }
      j++;
    }

    for (int i = 0; i < counts.size(); i++){
      if (actualCounts.find(i) != actualCounts.end())
	counts[i] /= actualCounts.at(i);
    }

    if (lognorm == 1){
      for (auto it = counts.begin(); it != counts.end(); ++it){
        c = (double) *it;
        c = log10(c);
        *it = c;
      }
    }
  }


}



// Print to file 2 columns (bin centers, values)

template <typename T>
void Histogram1D<T>::PrintASCII(std::string filepath){
  std::ofstream histOut(filepath.c_str());
  
  for (int i = 0; i < counts.size(); i++){
    std::cout << i << "," << bins.size() << "," << counts.size() << std::endl;
    histOut << bins[i] + 0.5 * (bins[i+1] - bins[i]) << " " << counts[i] << std::endl;
  }
  histOut.close();
}

// 2D histogram constructor N log (Nx Ny)


template <typename T>
Histogram2D<T>::Histogram2D(std::vector<T> &datax, std::vector<T> &datay,	\
			    std::vector<T> &binsX, std::vector<T> &binsY){

  int i,j; // the indices of the elements
  std::map<double,int> histLookupX;
  std::map<double,int> histLookupY;

  binsx = binsX;
  binsy = binsY;
  counts = std::vector<std::vector<int> > (binsx.size() - 1, std::vector<int>(binsy.size() - 1,0));

  /*
    Construct X and Y BSTs
  */

  std::cout << "Constructing BSTs..." << std::endl;

  for (auto it = binsx.begin(); it != binsx.end(); ++it){
    i = it - binsx.begin();
    histLookupX.insert(std::pair<double,int>(binsx[i], i));
  }

  for (auto it = binsy.begin(); it != binsy.end(); ++it){
    i = it - binsy.begin();
    histLookupY.insert(std::pair<double,int>(binsy[i], i));
  }

  /*
    Now bin the data
  */

  std::cout << "Binning data..." << std::endl;
  for (int k = 0; k < datax.size(); k++){
    auto indexItX = histLookupX.upper_bound(datax[k]);
    auto indexItY = histLookupY.upper_bound(datay[k]);
    i = indexItX->second;
    j = indexItY->second;
    //std::cout << i << "," << j << std::endl;
    if (i > 0 && j > 0 && indexItX != histLookupX.end() && indexItY != histLookupY.end())
      counts[i-1][j-1]++;
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
  for (int i = 0; i < counts.size() + 1; i++){
    for (int j = 0; j < counts[0].size() + 1; j++){
      if (i != 0 && j != 0){
	if (j != counts[0].size())
	  outfile << counts[j-1][i-1] << " ";
	else
	  outfile << counts[j-1][i-1] << "\n";
      }
      else if (i != 0 && j == 0){
	outfile << (binsy[i - 1] + binsy[i])/2. << " ";
      }
      else if (i == 0 && j != 0){
	if (j != counts[0].size()){
          outfile << (binsx[j-1] + binsx[j])/2. << " ";
	}
        else{
          outfile << (binsx[j-1] + binsx[j])/2. << "\n";
	}
      }
      else if (i == 0 && j == 0){
	outfile << binsx.size() - 1 << " ";
      }
    }
  }
}

template class Histogram1D<double>;
template class Histogram1D<int>;

template class Histogram2D<double>;
template class Histogram2D<int>;

