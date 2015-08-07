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
	vector<float> smoothData;
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
   
    DMsft.resize(numDM,DM_sftIndex (0.0));
    for(i=0;i<numDM;i++){
    	DMsft[i].DM = DMstart+i*DMstep;
    	DMsft[i].cal_sftIdx(indata.freqAxis,timeStep);
    	DMsft[i].get_smoothSize();
    }
    cout<<"load data"<<endl;
    /*Load test data*/
    for(i=0;i<numFbin;i++){
    	for(j=0;j<numTbin;j++){
    		indata.fltdata[i][j] = 1;
    	}
    }
    cout<<"smoothdata"<<endl;
    /*smooth data*/
    for(i=0;i<numFbin;i++){
    	//cout<<"smoothing"<<endl;
    	smoothData = smooth_data(indata.fltdata[i],DMsft[99].smoothSize[i]);
    	//cout<<"Finish smoothing"<<endl;
    	cout<< DMsft[99].smoothSize[i]<<endl;
    	if(i<10){
    		for(j=0;j<numTbin;j++){
        	    cout<< smoothData[j]<<" ";
            }
            cout<<endl;
    	}
        
    }
    


    return 0;
}
