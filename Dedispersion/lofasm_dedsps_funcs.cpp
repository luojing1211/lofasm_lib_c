#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <time.h>
#include "lofasm_dedsps_class.h"
using namespace std;


/* Smooth data function */
vector<float> smooth_data(vector<float> data, int smoothSize){
	/*
	data is a 1-D vector. 
	*/
	int i,j;
	size_t lenData;
    vector<float> result; 
    vector<float> smthblk(smoothSize,0.0);  // smooth data block
    lenData = data.size();
	if(lenData<= smoothSize){
		cout<<"Not enought data for smoothing.\n"<<endl;
	    exit(1);
	}

    result = data;
    if(smoothSize<=0){
        return data;
    }
    
    /* Initialize the smooth block with the first smooth size
     data.*/
    copy(data.begin(),data.begin()+smoothSize,smthblk.begin());
   

	for(i=0;i<lenData-smoothSize+1;i++){
		
		result[i] = accumulate(smthblk.begin(),smthblk.end(),0); // Sum up smooth block
	    // Shift smooth block to next five data. 
	    
	    rotate(smthblk.begin(),smthblk.begin()+1,smthblk.end());
	    smthblk.back() = data[i+smoothSize];

	}
	return result;
}


/* Do dedispersion */


/* Simulate data */
fltbank simulate_flt_ez(double dm, double fstart, double fStep, double tstart, \
                     double tStep, int numfBin, int numtBin, float noiseAmp,   \
                     float noiseBias,float SNR, double highFreqTOA){
    
	fltbank result(numfBin,numtBin);
	DM_sftIndex DMsft(dm);
    
    float signalAmp;
    int TOAindex;
    int i,j;
    int smear;
    int sft;
    double timeDelay;

    signalAmp = noiseAmp*SNR;

    /* Set filter bank data axises */ 
    result.set_freqAxis(fstart,fStep);
    result.set_timeAxis(tstart,tStep);
    
    /* Check signal arrival time */
    if(highFreqTOA<=result.timeAxis.front()||
    	highFreqTOA>=result.timeAxis.back()){
    	cout<<"Highest freqency arrival time overflow the time axis of filter";
        cout<<"bank data"<<endl;
        exit(1);
    }

    /* Fill data with noise first */
    /* initialize random seed: */
    srand (time(NULL));
    for(i=0;i<numfBin;i++){
    	for(j=0;j<numtBin;j++){
    		result.fltdata[i][j] = ((static_cast <float> (rand())/   \
    			                    static_cast <float>(RAND_MAX))   \
    			                    *noiseAmp*2-noiseAmp+noiseBias);
    	}
    }
    /* Add signals */
    TOAindex =  (int)highFreqTOA/(double)tStep;
    
    
    /* Get smear*/
    DMsft.cal_sftIdx(result.freqAxis, tStep);
    DMsft.get_smoothSize();
    /*get shift index*/
    for(i=0;i<numfBin;i++){
        timeDelay = 4.15e3*DMsft.DM*(1.0/(result.freqAxis[i]*result.freqAxis[i])
                       -1.0/(result.freqAxis.back()*result.freqAxis.back()));
        DMsft.sftIdx[i] = (int)trunc(timeDelay/tStep);
    }


    for(i=numfBin-1;i>=0;i--){
    	smear = DMsft.smoothSize[i]+1;
    	sft = DMsft.sftIdx[i];
    	result.fltdata[i][TOAindex+sft] = signalAmp;
    	for(j=1;j<smear;j++){
    		result.fltdata[i][TOAindex+sft+j] = signalAmp;
    	}
    }	 

    return result;

}

