#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
#include"lofasm_dedsps_class.h"
using namespace std;

/* Class methods for fltbank class*/
/* Initialize the filter bank data class */
fltbank::fltbank(int numfbin,int numtbin){
    numFreqBin = numfbin;
    numTimeBin = numtbin;
    // Initial values are in 0.0
    fltdata.resize(numFreqBin, vector<float> (numTimeBin,0.0));
}
/* Create frequency axis */
void fltbank::set_freqAxis(double fStart, double fStep){
    if(numFreqBin <= 0){
        cout<<"The number of frequency bin should be more than zero"<<endl;
        exit(1);
    }
    freqAxis.resize(numFreqBin,0.0);
    freqStep = fStep;
    for(int i =0;i<numFreqBin;i++){
        freqAxis[i] = fStart+i*freqStep;
    }
}
/* Create time axis */
void fltbank::set_timeAxis(double tStart, double tStep){
    if(numTimeBin <= 0){
        cout<<"The number of time bin should be more than zero"<<endl;
        exit(1);
    }
    timeAxis.resize(numTimeBin,0.0);
    timeStep = tStep;
    for(int i =0;i<numTimeBin;i++){
        timeAxis[i] = tStart+i*timeStep;
    }
}
/* Finish define the fltbank data class methods*/


/*Class methods for DM_sftIndex*/
DM_sftIndex::DM_sftIndex(double dm){
    DM = dm;

}

void DM_sftIndex::cal_sftIdx(vector<double> freqAxis, double timeStep){
    int Nf;   //Number of frequency bin
    int i;
    double sftbin;
    tStep = timeStep;
    Nf = freqAxis.size();
    sftIdx.resize(Nf,0);
    for(i=0;i<Nf;i++){
        timeDelay = -4.15e3*DM*(1.0/(freqAxis[i]*freqAxis[i])
                       -1.0/(freqAxis[0]*freqAxis[0]));
        sftbin = timeDelay/tStep;
        sftIdx[i] = (int)trunc(sftbin); 
    }
}

void DM_sftIndex::get_smoothSize(){
    int i;
    int idxDiff;
    int size;
    if(sftIdx.size()<1){
        cout<<"Plesase calculate sht shift index first!"<<endl; 
    }
    smoothSize.resize(sftIdx.size(),0);
    for(i=0;i<smoothSize.size()-1;i++){
        idxDiff = sftIdx[i+1]-sftIdx[i];
        size = idxDiff-1;
        smoothSize[i+1] = size;

    }
}
/* Finish define the DM_sftIndex class methods*/





