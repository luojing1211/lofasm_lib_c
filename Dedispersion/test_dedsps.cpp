#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <numeric>
#include "lofasm_dedsps_class.h"
using namespace std;

int main(int argc, char *argv[]){
	double DMstart, DMstep;
	double fstart,freqStep;
	double tstart,timeStep;
	int numDM, numTbin, numFbin;
	int i,j;
	vector<DM_sftIndex> DMsft;
    /*Test data here*/
	DMstart = 0;
	DMstep = 0.1;
    numDM = 100;

    numTbin = 1000;
    numFbin = 800;
    
    fstart = 10.0;
    freqStep = 0.0976;

    tstart = 0.0;
    timeStep = 0.0833;
    fltbank indata(numFbin,numTbin);
    
    indata.set_freqAxis(fstart,freqStep);
    indata.set_timeAxis(tstart,timeStep);
   
    DMsft.resize(numDM,DM_sftIndex (0.0, timeStep));
    for(i=0;i<numDM;i++){
    	DMsft[i].DM = DMstart+i*DMstep;
    	DMsft[i].cal_sftIdx(indata.freqAxis);
    	DMsft[i].get_smoothSize();
    }
    
    for(i=0;i<numDM;i++){
    	for(j=0;j<numFbin;j++){
    		cout<<DMsft[i].DM<<" "<<DMsft[i].smoothSize[j]<<" ";
    	}
    	cout<<endl;
    }
    return 0;
}
