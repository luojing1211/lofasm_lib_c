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
#include "lofasm_dedsps_funcs.h"
using namespace std;

int main(){
	int i,j;
	ofstream outputfile("sim_flt2.dat");
    ofstream outputfile4("DMT2.dat");
    int outdataFbin;
    int outdataTbin;
    int status;
    clock_t t;
    /* Simulate filter bank data */
    cout<<"simulate data"<<endl;
    fltbank testData = simulate_flt_ez(1, 10, 0.09, 0,0.08, 800, 1000,10, 0, 100, 5);
    
     
    if (outputfile.is_open())    
    {
        for(i=0;i<800;i++){
            for(j=0;j<1000;j++){
                outputfile << testData.fltdata[i][j] <<" ";
            }
            outputfile<<endl;
        }
        outputfile.close();
    }
    else cout<< "Unable to open the file1";


    cout<<"perpare get DM_T_POWER\n"<<endl;

    cout<<"initialize DMS array "<<endl;
    /* Calculate DM step*/
    cout<<"Test DM step"<<endl;
    int dmStart = 0;
    int dmEnd = 10;
    double dmStep = cal_dmStep_min(testData.freqAxis.back(),testData.freqAxis.front(),
                            testData.timeStep);
    cout<<"dm step "<<dmStep<<endl;
    int dmNUM = (int)(dmEnd-dmStart)/dmStep;
    
    
    vector<DM_sftIndex> DMSarray(dmNUM,DM_sftIndex (0.0));
    for(i=0;i<dmNUM;i++){
        DMSarray[i].DM = i*dmStep+0;
        DMSarray[i].cal_sftIdx(testData.freqAxis,testData.timeStep,testData.freqAxis.front());
        DMSarray[i].get_smoothSize();
        DMSarray[i].cal_sltIdx(testData.freqAxis,testData.timeStep,testData.freqAxis.back());
        DMSarray[i].cal_normNum();
        cout<<DMSarray[i].normNum<<endl;
    }
    
    cout<<"Calculate cut freq index"<<endl;
    for(i=1;i<dmNUM;i++){
        DMSarray[i].freqCutTree = cal_cut_freq_index(DMSarray[i],DMSarray[i-1]);
        cout<<DMSarray[i].freqCutTree<<endl;
    }

    cout<<"initialize result data "<<endl;
    outdataFbin = testData.freqAxis.size();
    outdataTbin = testData.timeAxis.size();

    cout<<"Create DM_T_power data."<<endl;

    DM_time DMT(dmNUM,outdataTbin,testData.timeStep);
    DMT.set_dmAxis(0,dmStep);
    DMT.set_timeAxis(0.0);
    DMT.set_DM_time_power();
    DMT.set_normArray();
    t = clock();
    status = compute_DM_t_power_tree(testData, DMT, DMSarray);
    t = clock()-t;
    cout<<"it cost : "<<((float)t)/CLOCKS_PER_SEC<<"Secs"<<endl;
    cout<<"Writing data"<<endl;
    if (outputfile4.is_open())    
    {
        for(i=0;i<DMT.DM_time_power.size();i++){
            for(j=0;j<DMT.DM_time_power[0].size();j++){
                outputfile4 << DMT.DM_time_power[i][j] << " ";
            }
            outputfile4<<endl;
        }
        outputfile4.close();
    }
    else cout<< "Unable to open the file4";


    return 0;
}