#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
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
    ofstream outputfile("dedsps_flt.dat");
    double DM;
    double timeStep, freqStep;
    double tstart, fstart;
    int numTbin,numFbin;
    
    vector<double> timeDelay;
    /*Test value here*/
    DM = atof(argv[1]);
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
        for(j=50;j<70;j++){
            indata.fltdata[i][j] = 3.0;   // Square pulse
        }
    }

    cout<<"Compute time delay!"<<endl;  
    timeDelay = compute_timeDelay(DM, indata.freqAxis);

//    for(i=0;i<numFbin;i++){
//        cout<<"Time delay in secones "<<timeDelay[i]<<"for freq "
//            <<indata.freqAxis[i]<< endl;
//    }


    cout<<"Do Dedispsion"<<endl;

    fltbank outdata(numFbin,numTbin);
    do_dedisperse(DM,&indata,&outdata);
    cout<<"size out data"<<outdata.fltdata[0].size()<<endl;
    if (outputfile.is_open())    
    {
        for(i=0;i<numFbin;i++){
            for(j=0;j<outdata.fltdata[0].size();j++){
                outputfile << outdata.fltdata[i][j] <<" ";
            }
        outputfile<<endl;
        }
        outputfile.close();
    }
    else cout<< "Unable to open the file";
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
    /*Check out put data freqency axis*/
    if(outputdata->numFreqBin!=numfBin){
        outputdata->fltdata.resize(numfBin);
        outputdata->numFreqBin = numfBin;
        outputdata->set_freqAxis(inputdata->freqAxis[0],inputdata->freqStep);
    }
    /* Computer time delay */
    timeDelay = compute_timeDelay(DM,inputdata->freqAxis);
    /* Do shift index */
    timeStep = inputdata-> timeStep;
    /*Compute shift index*/
    shiftIndex.resize(numfBin,0);    
    for(i=0;i<numfBin;i++){
        shiftIndex[i] = (int)(timeDelay[i]/timeStep);
    }
    /* Find the max time shift */ 
    maxShift = *std::max_element(shiftIndex.begin(),shiftIndex.end());
    /*Resize the output data size*/
    if(outputdata->numTimeBin < inputdata->numTimeBin+maxShift){
        for(i=0;i<numfBin;i++){
            outputdata-> fltdata[i].resize(inputdata->numTimeBin+maxShift); 
        }
        outputdata-> numTimeBin = inputdata->numTimeBin+maxShift;
        /*Form a new time Axis */
        outputdata-> set_timeAxis(inputdata->timeAxis[0],inputdata->timeStep);
    }
  
    /* move the data to the out data array*/
    for(i=0;i<numfBin;i++){
        for(j=0;j<numtBin;j++){
            outputdata->fltdata[i][j+shiftIndex[i]] = inputdata->fltdata[i][j]; 
        }
    }
    
    
    return;    

}
