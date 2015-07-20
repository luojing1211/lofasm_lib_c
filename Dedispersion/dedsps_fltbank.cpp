#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<vector>
#include<algorithm>
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
/* Finish define the filter bank data class*/




void do_dedisperse(double DM, fltbank* inputdata, fltbank* outputdata);
vector<double> compute_timeDelay(double DM, vector<double> &freq);
/*Main function*/
int main(int argc, char* argv[]){
    int i,j;
    
    FILE *inputFile; 
    FILE *outputfile;
    double DM;
    double timeStep, freqStep;
    double tstart, fstart;
    int numTbin,numFbin;
    
    vector<double> timeDelay;
    /*Test value here*/
    DM = 0.0;
    tstart = 0.0;
    timeStep = 0.0833;
    numTbin = (int)(10.0/timeStep);    

    cout<<"help!"<<endl;
    fstart = 10.0;
    freqStep = 1.0;
    numFbin = (int)(55.0-10.0);     
    /* Construct fltbank data*/
    fltbank indata(numFbin,numTbin);
    
    indata.set_freqAxis(fstart,freqStep);
    indata.set_timeAxis(tstart,timeStep);
    cout<<"help!2"<<endl; 
    /* Load input data ++++++++++test here*/
    for(i=0;i<numFbin;i++){
        cout<<"help!3"<<i<<endl;
        for(j=50;j<70;j++){
            cout<<"help4! "<<j<<endl;
            indata.fltdata[i][j] = 3.0;   // Square pulse
        }
    }
    cout<<"help!"<<endl;  
    timeDelay = compute_timeDelay(DM, indata.freqAxis);
    for(i=0;i<numTbin;i++){
        cout<<"Time delay in secones "<<timeDelay[i]<<endl;
    }
    return 0;

}


/*Calculate the time delay*/
vector<double> compute_timeDelay(double DM, vector<double> &freq){
    int numfBin;
   
    numfBin = freq.size();
    vector<double> timeDelay(numfBin,0.0);
    for(int i=0;i<numfBin;i++){
        timeDelay[i] = -4.15e3*DM*(1.0/(freq[i]*freq[i])
                       -1.0/(freq[0]*freq[0]));
    }
    
    return timeDelay;

}

/*Do de-dispersion*/
void do_dedisperse(double DM ,fltbank* inputdata, fltbank* outputdata){
    int numfBin;
    int numtBin;
    int delayIndex;
    int i,j;
    int maxShift;
    double timeStep;

    vector<double> timeDelay;
    vector<int> shiftIndex;
    numfBin = inputdata->numFreqBin;
    numtBin = inputdata->numTimeBin;

    /* Computer time delay */
    timeDelay = compute_timeDelay(DM,inputdata->freqAxis);
    /* Do shift index */
    timeStep = inputdata-> timeStep;
    /*Compute shift index*/
    shiftIndex.resize(numfBin,0);    
    for(i=0;i<numfBin;i++){
        shiftIndex[i] = (int)(timeDelay[i]/timeStep);
    }

    maxShift = *std::max_element(shiftIndex.begin(),shiftIndex.end());
    if(outputdata->numTimeBin < inputdata->numTimeBin+maxShift){
        outputdata-> fltdata.resize(inputdata->numTimeBin+maxShift);
        outputdata-> numTimeBin = inputdata->numTimeBin+maxShift; 
    }
    return;    

}
