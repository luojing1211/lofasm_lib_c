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
vector<float> smooth_data(vector<float> &data, int smoothSize){
	/*
	data is a 1-D vector. 
	*/
	int i,j;
	size_t lenData;
   
    vector<float> result(data.size(),0.0);
	result = data;
    /* smooth size smaller then 0, no need to smooth*/
    if(smoothSize<=0){
        return data;
    }
    /*Check data length*/
    lenData = data.size();
	if((int)lenData <= smoothSize){
		cout<<"Not enought data for smoothing.\n"<<endl;
	    exit(1);
	}
    
    vector<float> smthblk(smoothSize,0.0);  // smooth data block
    
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

double compute_time_delay(double freq, double freqRef, double dm){
    double timeDelay;
    timeDelay = 4.15e3*dm*(1.0/(freq*freq)-1.0/(freqRef*freqRef));
    return timeDelay;
}

double cal_dmStep_min(double freqMax, double freqMin, double timeStep){
    /* Calculate the smallest dm step*/
    double dmstep;
    cout<<timeStep<<endl;
    cout<<freqMax<<endl;
    cout<<freqMin<<endl;
    dmstep = timeStep/(-4.15e3*(1.0/(freqMax*freqMax)
                       -1.0/(freqMin*freqMin)));
    return dmstep;
}

int check_data_size(fltbank & data, DM_time & DMT, vector<DM_sftIndex> & DMsftArray){
    return 0;
}

/* Create DM_T Plot*/
int compute_DM_t_power_dommy(fltbank & data, DM_time & DMT, vector<DM_sftIndex> & DMsftArray){
    int status;
    int i,j,k;
    int numfBin, numtBin, numDM;
    int sftI;
    numfBin = data.numFreqBin;
    numtBin = data.numTimeBin;
    numDM = DMT.numDM;
    
    /**
    for(i=0;i<numDM;i++){
        cout<<DMsftArray[i].DM<<endl;
        for(j=0;j<numfBin;j++){
            cout<<DMsftArray[i].sftIdx[j]<<" ";  
        }
        cout<<endl;
    }
    */

    /* Dommy way of do it */
    for(i=0;i<numDM;i++){
        for(j=0;j<numfBin;j++){
            sftI = DMsftArray[i].sftIdx[j];
            for(k=0;k<numtBin;k++){
                DMT.DM_time_power[i][k+sftI] += data.fltdata[j][k];
                DMT.normArray[k+sftI]+= 1.0;
            }
        }
        /*Normalize */
        for(k=0;k<DMT.numTimeBin;k++){
            DMT.DM_time_power[i][k] = DMT.DM_time_power[i][k]/DMT.normArray[k];
        }
        fill(DMT.normArray.begin(), DMT.normArray.end(), 0.000001);
    }
    return 0;
}

int compute_DM_t_power_tree_dommy(fltbank & data, DM_time & DMT, vector<DM_sftIndex> & DMsftArray){
    int status;
    int i,j,k;
    int loop1,loop2;
    int dmIdx;
    int numfBin, numtBin, numDM;
    int sftI;
    int sltI[2] = {0,0};
    int sltIp[2] = {0,0};   //selected index from last dm
    int numSub;
    int numAdd;
    float lastPower;
    float curPower;
    numfBin = data.numFreqBin;
    numtBin = data.numTimeBin;
    numDM = DMT.numDM;

    /* Tree method of computing DM_T_POWER */
    // Calculate first DM for T_Power array
    /* loop over time bin last*/
    
    /* Do first dm */
    for(i=0;i<numtBin;i++){
        for(j=0;j<numfBin;j++){
            sftI = DMsftArray[0].sftIdx[j];
            sltI[0] = DMsftArray[0].sltIdx[j][0];
            sltI[1] = DMsftArray[0].sltIdx[j][1];
            /* Do summation for this time bin */
            for(k=0;k<sltI[1]-sltI[0]+1;k++){
                DMT.DM_time_power[0][i] += data.fltdata[j][i+k+sltI[0]];
            }
        }
        DMT.DM_time_power[0][i] = DMT.DM_time_power[0][i]/(float)DMsftArray[0].normNum;
    }
    /*Dommy way */
    for(dmIdx=1;dmIdx<numDM;dmIdx++){
        for(i=0;i<numtBin;i++){
            for(j=0;j<numfBin;j++){
                sftI = DMsftArray[dmIdx].sftIdx[j];
                sltI[0] = DMsftArray[dmIdx].sltIdx[j][0];
                sltI[1] = DMsftArray[dmIdx].sltIdx[j][1];
                /* Do summation for this time bin */
                for(k=0;k<sltI[1]-sltI[0]+1;k++){
                    DMT.DM_time_power[dmIdx][i] += data.fltdata[j][i+k+sltI[0]];
                }
            }   
        DMT.DM_time_power[0][i] = DMT.DM_time_power[dmIdx][i]/(float)DMsftArray[dmIdx].normNum;
        }
    }

    // /*Do other dm*/
    
    // cout<<"Start tree method."<<endl;
    // for(dmIdx=1;dmIdx<numDM;dmIdx++){
    //     for(i=0;i<numtBin;i++){
    //         lastPower = DMT.DM_time_power[dmIdx-1][i]*DMsftArray[dmIdx-1].normNum;
    //         curPower = lastPower;
    //         for(j=0;j<numfBin;j++){
    //             sftI = DMsftArray[dmIdx].sftIdx[j];
    //             sltI[0] = DMsftArray[dmIdx].sltIdx[j][0];
    //             sltI[1] = DMsftArray[dmIdx].sltIdx[j][1];
    //             sltIp[0] = DMsftArray[dmIdx-1].sltIdx[j][0];
    //             sltIp[1] = DMsftArray[dmIdx-1].sltIdx[j][1];
    //             numSub = sltI[0]-sltIp[0];
    //             numAdd = sltI[1]-sltIp[1];
                    
    //             /*Substract the power we don't need*/
    //             for(loop1=0;loop1<numSub;loop1++){
    //                 curPower = curPower - data.fltdata[j][i+sltIp[0]+loop1]; 
    //             }
    //             /*Add new powers */
    //             for(loop2=0;loop2<numSub;loop2++){
    //                 curPower = curPower+ data.fltdata[j][i+sltIp[1]-loop2];
    //             }
    //         }
    //         DMT.DM_time_power[dmIdx][i] = curPower/(float)DMsftArray[dmIdx].normNum;    
    //     }    
    // }
    return 0;
}

int compute_DM_t_power_tree(fltbank & data, DM_time & DMT, vector<DM_sftIndex> & DMsftArray){
    int status;
    int i,j,k;
    int loop1,loop2;
    int dmIdx;
    int numfBin, numtBin, numDM;
    int sftI;
    int numSub;
    int numAdd;
    float lastPower;
    float curPower;

    numfBin = data.numFreqBin;
    numtBin = data.numTimeBin;
    numDM = DMT.numDM;

    vector< vector<int> > sltdiff;
    vector<int> sltIStart(numfBin,0);
    vector<int> sltIEnd(numfBin,0);

    vector<int> sltIpStart(numfBin,0);   //selected index from last dm
    vector<int> sltIpEnd(numfBin,0);
    sltdiff.resize(numfBin, vector<int>(2,0));

    /* Tree method of computing DM_T_POWER */
    // Calculate first DM for T_Power array
    /* loop over time bin last*/
    
    /* Do first dm */

    for(i=0;i<numtBin;i++){
        for(j=0;j<numfBin;j++){
            sftI = DMsftArray[0].sftIdx[j];
            sltIStart[j] = DMsftArray[0].sltIdx[j][0];
            sltIEnd[j] = DMsftArray[0].sltIdx[j][1];
            /* Do summation for this time bin */
            for(k=0;k<sltIEnd[j]-sltIStart[j]+1;k++){
                DMT.DM_time_power[0][i] += data.fltdata[j][i+k+sltIStart[j]];
            }
        }
        DMT.DM_time_power[0][i] = DMT.DM_time_power[0][i]/(float)DMsftArray[0].normNum;
        
    }

    /*Do other dm*/
    
    cout<<"Start tree method."<<endl;
    for(dmIdx=1;dmIdx<numDM;dmIdx++){
        cout<<" dm "<<dmIdx<<endl;
        for(j=0;j<numfBin;j++){
            sltIStart[j] = DMsftArray[dmIdx].sltIdx[j][0];
            sltIEnd[j] = DMsftArray[dmIdx].sltIdx[j][1];
            sltIpStart[j] = DMsftArray[dmIdx-1].sltIdx[j][0];
            sltIpEnd[j] = DMsftArray[dmIdx-1].sltIdx[j][1];
            sltdiff[j][0] = sltIStart[j]-sltIpStart[j];
            sltdiff[j][1] = sltIEnd[j]-sltIpEnd[j];
        }

        for(i=0;i<numtBin;i++){

            lastPower = DMT.DM_time_power[dmIdx-1][i]*DMsftArray[dmIdx-1].normNum;
            curPower = lastPower;

            for(j=0;j<numfBin;j++){
                /*Substract the power we don't need*/
                for(loop1=0;loop1<sltdiff[j][0];loop1++){
                    curPower = curPower - data.fltdata[j][i+sltIpStart[j]+loop1];
                }

                /*Add new powers */
                for(loop2=0;loop2<sltdiff[j][1];loop2++){
                    curPower = curPower+ data.fltdata[j][i+sltIEnd[j]-loop2];
                }
            
            }

            DMT.DM_time_power[dmIdx][i] = curPower/(float)DMsftArray[dmIdx].normNum;
        }     
            
        
            
            
    }
    return 0;
}

int do_dedsps_check(fltbank & indata, fltbank & outdata, DM_sftIndex & DMsft){
    /* Check the status of input parameters*/
    return 0;
}

int do_dedsps_curve(fltbank & indata, fltbank & outdata, DM_sftIndex & DMsft){
    /* The shift index should be calculated 
       Input data freqency size should be the same with sftIndex size*/
    int status;
    int i,j,k;
    int numfBin,numtBin;
    int maxSft;
    int targetIndex;
    int sftI;
    int sltI;
    vector<int> selectIdx;
    
    double tStep;

    numfBin = indata.freqAxis.size();
    numtBin = indata.timeAxis.size();
    tStep = indata.timeStep;
    selectIdx.resize(numfBin,0);
    maxSft = DMsft.sftIdx.back();
    for(i=0;i<numfBin;i++){
        selectIdx[i] = (int)trunc(compute_time_delay(indata.freqAxis[i], indata.freqAxis.back(), DMsft.DM)/tStep);
    }

    for(j=0;j<3;j++){
        targetIndex = j+maxSft;
        for(i=0;i<numfBin;i++){
            sftI = DMsft.sftIdx[i];
            //cout<<"freq bin "<<i<<" sSize "<<DMsft.smoothSize[i]<<" shft "<<sftI;
            sltI = selectIdx[i];
            for(k=0;k<=DMsft.smoothSize[i];k++){
                //cout<<" index "<<j<<" "<<targetIndex-sftI+k<<" ";
                cout<<sltI<<" "<<indata.fltdata[i][sltI+k+j]<<" ";
                outdata.fltdata[i][targetIndex] += indata.fltdata[i][sltI+k+j];   
            }
            cout<<endl; 
        }
    }
}

/* Do dedispersion */
int do_dedsps(fltbank & indata, fltbank & outdata, DM_sftIndex & DMsft){
    /* The shift index should be calculated 
       Input data freqency size should be the same with sftIndex size*/
    int status;
    int i,j;
    int numfBin,numtBin;


    if(indata.freqAxis.size()!=DMsft.sftIdx.size()){
    	cout<<"Input data Freqency bins not match shift index array bins"<<endl;
    	exit(1);
    }

    /* Adjust out put time length*/
    if(outdata.timeAxis.size()<indata.timeAxis.size()+ DMsft.sftIdx.back()){
    	cout<<"Not enough size for dedispersion. At least ";
    	cout<<indata.timeAxis.size()+ DMsft.sftIdx.back()<<" Time bins are needed";
    	cout<<endl;
    	exit(1);
    }

    for(i=0;i<indata.freqAxis.size();i++){
    	for(j=0;j<indata.timeAxis.size();j++){
    		outdata.fltdata[i][j+DMsft.sftIdx[i]]=indata.fltdata[i][j];
    	}
    }

	return status;
}

/* Simulate data */
fltbank simulate_flt_ez(double dm, double fstart, double fStep, double tstart, \
                     double tStep, int numfBin, int numtBin, float noiseAmp,   \
                     float noiseBias,float SNR, double highFreqTOA)
{
    
	fltbank result(numfBin,numtBin);  
	DM_sftIndex DMsft(dm);
    
    float signalAmp;
    int TOAindex;
    int i,j;
    int smear;
    int sft;

    double timeDelay;
    double freqCal;            // A fake freqency for calculate the last channal smear out
    int chan1smear;


    signalAmp = noiseAmp*SNR;

    /* Set filter bank data axises */ 
    result.set_freqAxis(fstart,fStep);
    result.set_timeAxis(tstart,tStep);
    
    freqCal = result.freqAxis.front()-fStep;
    /* Check signal arrival time */
    if(highFreqTOA<=result.timeAxis.front()||
    	highFreqTOA>=result.timeAxis.back()){
    	cout<<"Highest freqency arrival time overflow the time axis of filter";
        cout<<"bank data"<<endl;
        exit(1);
    }

    /* Fill data with noise first */
    /* initialize random seed: */
    /* Add noise */
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
    DMsft.cal_sftIdx(result.freqAxis, tStep,result.freqAxis.front());
    DMsft.get_smoothSize();

    /* Get smear for the first channal */
    int chan1sft;
    chan1sft = (int)trunc(compute_time_delay(result.freqAxis.front(), freqCal, dm)/tStep);
    
    /*get shift index*/
    for(i=0;i<numfBin;i++){
        timeDelay = 4.15e3*DMsft.DM*(1.0/(result.freqAxis[i]*result.freqAxis[i])
                       -1.0/(result.freqAxis.back()*result.freqAxis.back()));
        
        DMsft.sftIdx[i] = (int)trunc(timeDelay/tStep);
    }

    /* Add signal */
    for(i=numfBin-1;i>=0;i--){
    	smear = DMsft.smoothSize[i]+1;
    	sft = DMsft.sftIdx[i];
    	result.fltdata[i][TOAindex+sft] = signalAmp;
    	for(j=1;j<smear;j++){
    		result.fltdata[i][TOAindex+sft+j] = signalAmp;
    	}
    }	 
    /* Add first channal smear out */
    for(i=0;i<chan1sft-1;i++){
        result.fltdata[0][TOAindex+sft+i-1] = signalAmp;
    }
    return result;

}

