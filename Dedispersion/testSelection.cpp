#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <numeric>
#include "lofasm_dedsps_class.h"
#include "lofasm_dedsps_funcs.h"
using namespace std;


int main(){
	int dmStart = 0;
    int dmEnd = 1000;
    int numFbin = 800;
    vector<double> freq(numFbin,0.0);

    double timeStep = 0.08;
    double freqStep = 0.09;
    double dmStep;
    int dmNum;
    int i,j;
    for(i=0;i<numFbin;i++){
    	freq[i] = 10.0+i*freqStep;
    }


    dmStep = cal_dmStep_min(freq.back(), freq.front(), timeStep);
    dmNum = (int)(dmEnd-dmStart)/dmStep;
    cout<<dmNum<<" "<<dmStep<<endl;

    vector<DM_sltIndex> DMSarray(dmNum,DM_sltIndex (0.0));
    for(i=dmNum-10;i<dmNum;i++){
        DMSarray[i].DM = i*dmStep+0;
        DMSarray[i].cal_sftIdx(freq, timeStep, freq.front());
        DMSarray[i].get_smearSize();
        DMSarray[i].cal_sltIdx(freq,timeStep,freq.back());
        DMSarray[i].cal_normNum();
    }

    cout<<"sefes";
    for(j=dmNum-10;j<dmNum-1;j++){
        for(i=0;i<freq.size();i++){
    	//cout<<DMSarray[dmNum-2].sltIdx[i][0]<<" "<<DMSarray[dmNum-2].sltIdx[i][1]<<"  ";
    	//cout<<DMSarray[dmNum-1].sltIdx[i][0]<<" "<<DMSarray[dmNum-1].sltIdx[i][1]<<endl

    		cout<<"DM now "<<DMSarray[j+1].DM<<" DM last "<<DMSarray[j].DM<<" ";
    		cout<<DMSarray[j+1].sltIdx[i][0]-DMSarray[j].sltIdx[i][0]<<" ";
    	    cout<<DMSarray[j+1].sltIdx[i][1]-DMSarray[j].sltIdx[i][1]<<endl;
    	}
    	cout<<endl;
    }

    return 0;
}
