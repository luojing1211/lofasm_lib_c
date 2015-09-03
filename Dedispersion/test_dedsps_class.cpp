#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
#include"lofasm_dedsps_class.h"
using namespace std;

int main(int argc, char* argv[]){
	int i,j;
 
    ofstream outputfile("dedsps_flt.dat");
    double DM;
    double timeStep, freqStep;
    double tstart, fstart;
    int numTbin,numFbin;
 
    /*Test value here*/
    DM = atof(argv[1]);
    tstart = 0.0;
    timeStep = 0.0833;
    numTbin = (int)(10.0/timeStep);    
   
    fstart = 10.0;
    freqStep = 0.0976; //MHz
    numFbin = 800;

    /* Time step*/
    timeStep = 0.0833;

    /* Construct fltbank data*/
    fltbank indata(numFbin,numTbin);
    
    indata.set_freqAxis(fstart,freqStep);
    indata.set_timeAxis(tstart,timeStep);
    
    DM_sftIndex DMsft(DM);
    DMsft.cal_sftIdx(indata.freqAxis,timeStep,indata.freqAxis.front());
    DMsft.get_smoothSize();
    for(i=0;i<DMsft.sftIdx.size();i++){
    	cout<<DMsft.sftIdx[i]<<" "<<DMsft.smoothSize[i]<<endl;
    }
    DMsft.cal_sltIdx(indata.freqAxis,timeStep,indata.freqAxis.back());
    
    for(i=0;i<DMsft.sftIdx.size();i++){
        cout<<DMsft.sltIdx[i][0]<<" "<<DMsft.sltIdx[i][1]<<endl;
    }
    DMsft.cal_normNum();
    cout<<DMsft.NormNum;
    return 0;

}