/*  
-Header_File lofasm_dedisperse_fltbank.h    

-Abstract     
    Header file of lofasm_dedisperse_fltbank.c  
    Defined class:
    Defined functions:

-Author_and_Institution
    Jing Luo    Center for Advanced Radio Astronomy (CARA)

-Version
    LoFASM dedispersion library 2.0.0. 10-FEB-2015
*/


#ifndef __LOFASM_DEDISPERSE_FLTBANK_H__
#define __LOFASM_DEDISPERSE_FLTBANK_H__


#include<vector>
using namespace std;

/*Define the filtbank data class*/
class fltbank  
{
        int numFreqBin;
        int numTimeBin;
    public:
        vector<vector<float>> fltdata;
        vector<double> freqAxis;
        double freqStep;

        vector<double> timeAxis;
        double timeStep;
        fltbank (int numfbin,int numtbin);
        void set_freqAxis(double fStart, double fStep);
        void set_timeAxis(double tStart, double tStep);
        void set_data();
}

fltbank::fltbank(int numfbin,int numtbin){
    numFreqBin = numfbin;
    numTimeBin = numtbin;
    fltdata.resize(numFreqBin, vector<float> (numTimeBin,0.0));
}

void fltbank::sef_freqAxis(double fstart, double fStep){
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

void fltbank::sef_timeAxis(double tStart, double tStep){
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


