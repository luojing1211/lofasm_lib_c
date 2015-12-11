#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "lofasm_dedsps_class.h"
#include "lofasm_dedsps_funcs.h"
using namespace std;

int main(int argc, char* argv[]){
	int i,j;

    ofstream outputfile("dedsps_flt.dat");
    double DM;
    double dmStep;
    double timeStep, freqStep;
    double tstart, fstart;
    int numTbin,numFbin;
    int cutIndex;
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

    DM_sltIndex DMsft(DM);
    //DMsft.cal_sftIdx(indata.freqAxis,timeStep,indata.freqAxis.front());
    //DMsft.get_smoothSize();

    DMsft.cal_sltIdx(indata.freqAxis,timeStep,indata.freqAxis.back());

    dmStep = cal_dmStep_min(indata.freqAxis.back(),indata.freqAxis.front(),timeStep);
    cout<<dmStep;
    DM_sltIndex DMsft2(DM+dmStep);
    //DMsft.cal_sftIdx(indata.freqAxis,timeStep,indata.freqAxis.front());
    //DMsft.get_smoothSize();

    DMsft2.cal_sltIdx(indata.freqAxis,timeStep,indata.freqAxis.back());
    cutIndex = cal_cut_freq_index(DMsft2,DMsft);
    for(i=0;i<DMsft.sltIdx.size();i++){
        cout<<"slc0 "<<DMsft.sltIdx[i][0 ]- DMsft2.sltIdx[i][0]<<" slc1 ";
        cout<<DMsft.sltIdx[i][1] - DMsft2.sltIdx[i][1]<<" "<<i<<" ";
        cout<<cutIndex<<endl;
    }
    DMsft.cal_normNum();
    cout<<DMsft.normNum;
    return 0;

}
