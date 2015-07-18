#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<vector>

using namespace std;

/*Define the filtbank data class*/
class fltbank
{
    public:
        int numFreqBin;
        int numTimeBin;
        vector< vector<float> > fltdata;
        vector<double> freqAxis;
        double freqStep;

        vector<double> timeAxis;
        double timeStep;
        fltbank (int numfbin,int numtbin);
        void set_freqAxis(double fStart, double fStep);
        void set_timeAxis(double tStart, double tStep);
        void set_data();
};

fltbank::fltbank(int numfbin,int numtbin){
    numFreqBin = numfbin;
    numTimeBin = numtbin;
    fltdata.resize(numFreqBin, vector<float> (numTimeBin,0.0));
}

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


void do_dedisperse(double DM, fltbank* inputdata, fltbank* outputdata);
/*Main function*/
int main(int argc, char* argv[]){
    int i,j;
    
    FILE *inputFile; 
    FILE *outputfile;
    double DM;
    double timeStep, freqStep;
    double tstart, fstart;
    int numTbin,numFbin;

    /*Test value here*/
    DM = 0.0
    tstart = 0.0;
    tstep = 0.0833;
    numTbin = (int)(10.0/tstep);    


    fstart = 10.0;
    fstep = 1.0;
    numFbin = (int)(55.0-10.0)     
    /* Construct fltbank data*/
    fltbank indata(numFbin,numTbin);
    
    indata.set_freqAxis(fstart,fstep);
    indata.set_timeAxis(tstart,tstep);
 
    /* Load input data ++++++++++test here*/
    for(i=0;i<numFbin;i++){
        for(j=50;j<70;i++){
            indata.fltdata[i][j] = 3.0;   // Square pulse
        }
    }
    
    do_dedisperse(DM, &indata, )
    return 0;

}
void compute_timeDelay(double DM, vector<double> &freq){

}

void do_dedisperse(double DM, fltbank* inputdata, fltbank* outputdata){
    int numfBin;
    int delayIndex;
    int i,j;
    numfBin = inputdata->numFreqBin;
    vector<double> timeDelay(numfBin,0.0);
    for(i=0;i<numfBin;i++){
        timeDelay[i] = -4.15e3*DM*(1.0/(inputdata->freqAxis[i]*inputdata->freqAxis[i])
                       -1.0/(inputdata->freqAxis[0]*inputdata->freqAxis[0]));
    } 

}
