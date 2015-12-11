#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>
#include <string.h>

#include "lofasm_intgr.h"
#include "lofasm_binary_utils.h"
using namespace std;
void lofasm_intgr::init_polar(int numFreqBin){
	  numfBin = numFreqBin;
		AA.resize(numfBin,0);
		BB.resize(numfBin,0);
		CC.resize(numfBin,0);
		DD.resize(numfBin,0);
		ABrl.resize(numfBin,0);
		ACrl.resize(numfBin,0);
		ADrl.resize(numfBin,0);
		BCrl.resize(numfBin,0);
		BDrl.resize(numfBin,0);
		CDrl.resize(numfBin,0);
		ABim.resize(numfBin,0);
		ACim.resize(numfBin,0);
		ADim.resize(numfBin,0);
		BCim.resize(numfBin,0);
		BDim.resize(numfBin,0);
		CDim.resize(numfBin,0);
}

void lofasm_intgr::set_freqAxis(double fstart, double fstep){
	  int i;
	  if (numfBin == 0){
			  cout<< "The number of frequency bins can not be zero."<<endl;
				exit(1);
		}
		freqAxis.resize(numfBin,0);
		for (i=0;i<numfBin;i++){
			  freqAxis[i] = fstart+i*fstep;
		}
}

void lofasm_intgr::form_beam(){
	  return;
}
/* Define function for read auto polarization from raw file*/
