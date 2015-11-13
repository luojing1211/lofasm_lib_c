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


/*Class methods for DM_sltIndextIndex*/
DM_sltIndex::DM_sltIndex(double dm){
    DM = dm;

}

void DM_sltIndex::cal_sftIdx(vector<double> freqAxis, double timeStep, double refFreq){
    int Nf;   //Number of frequency bin
    int i;
    double sftbin;
    int mid;
    tStep = timeStep;
    Nf = freqAxis.size();
    sftIdx.resize(Nf,0);

    for(i=0;i<Nf;i++){
        timeDelay = -4.15e3*DM*(1.0/(freqAxis[i]*freqAxis[i])
                    -1.0/(refFreq*refFreq));
        sftbin = timeDelay/tStep;
        sftIdx[i] = (int)trunc(sftbin);
    }


}

void DM_sltIndex::get_smearSize(){
    int i;
    int idxDiff;
    int size;
    if(sftIdx.size()<1){
        cout<<"Plesase calculate sht shift index first!"<<endl;
    }
    smearSize.resize(sftIdx.size(),0);
    for(i=0;i<smearSize.size()-1;i++){
        idxDiff = sftIdx[i+1]-sftIdx[i];
        size = idxDiff-1;
        if(size<0) size = 0;
        smearSize[i+1] = size;

    }
}

void DM_sltIndex::cal_sltIdx(vector<double> freqAxis, double timeStep, double refFreq){
    /* This function will assume that the positive select Index means signal arrive
       later. */
    int Nf;   //Number of frequency bin
    int i;
    double sftbin;
    double chan0;  //A imaginary freqency channal before the first channel
    int chan0SltIdx;
    int mid;
    int idxDiff;
    int size;  // Smear size;

    tStep = timeStep;
    Nf = freqAxis.size();
    sltIdx.resize(Nf, vector<int> (2,0));

    for(i=0;i<Nf;i++){
        timeDelay = 4.15e3*DM*(1.0/(freqAxis[i]*freqAxis[i])
                    -1.0/(refFreq*refFreq));
        sftbin = timeDelay/tStep;
        sltIdx[i][0] = (int)trunc(sftbin);

    }

    /* calcaulate smear out */
    /* Get first channel smear out*/
    chan0 = freqAxis[0]-freqAxis[1]+freqAxis[0];
    timeDelay = 4.15e3*DM*(1.0/(chan0*chan0)-1.0/(refFreq*refFreq));
    chan0SltIdx = (int)trunc(timeDelay/tStep);
    idxDiff = chan0SltIdx - sltIdx[0][0];
    size = idxDiff-1;
    if(size<0) size = 0;
    /* Get smear out size and second select index*/
    for(i=0;i<Nf-1;i++){
        sltIdx[i][1] = sltIdx[i][0]+size;
        // For the next channel;
        idxDiff = sltIdx[i][0]- sltIdx[i+1][0];
        size = idxDiff-1;
        if(size<0) size = 0;
    }

    // Get the last channel select index end.
    sltIdx[Nf-1][1] = sltIdx[Nf-1][0]+size;

}


void DM_sltIndex::cal_normNum(){
/* The sltIdx, select index, should be calculated first*/
    int slcNumPfreq;
    int totalSlcNum;
    int i;
    for(i=0;i<sltIdx.size();i++){
        slcNumPfreq = sltIdx[i][1]-sltIdx[i][0]+1;
        normNum += slcNumPfreq;
    }
}
/* Finish define the DM_sftIndex class methods*/

/* Define class function for DM_time class*/
DM_time::DM_time(int numDMbin, int numTBin, double tStep){
    numDM = numDMbin;
    numTimeBin = numTBin;
    timeStep = tStep;
}

void DM_time::set_timeAxis(double timeStart){
    if(numTimeBin<1){
        cout<<"Please make sure number of time bin is not zero."<<endl;
        exit(1);
    }
    timeAxis.resize(numTimeBin,0.0);
    for(int i =0;i<numTimeBin;i++){
        timeAxis[i] = timeStart+i*timeStep;
    }
}

void DM_time::set_dmAxis(double dmStart,double DMStep){
    int i;
    if(numDM<1){
        cout<<"Please make sure number of DM is not zero."<<endl;
        exit(1);
    }
    dmAxis.resize(numDM,0.0);
    for(i =0;i<numDM;i++){
        dmAxis[i] = dmStart+DMStep*i;
    }
}

void DM_time::set_normArray(){
    normArray.resize(numTimeBin,0.000001);// initial as 0.000001 a small number
}

void DM_time::set_DM_time_power(){
    DM_time_power.resize(numDM, vector<float> (numTimeBin,0.0));
}
/* Finish define the class function for class DM_time*/
