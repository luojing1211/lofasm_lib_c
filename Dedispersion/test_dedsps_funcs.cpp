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
	vector<float> v;
	int s,i,j;
	int status;
	int outdataFbin,outdataTbin;
    ofstream outputfile("sim_flt.dat");
    ofstream outputfile2("smoothflt.dat");
    ofstream outputfile3("dedsps.dat");
	s = 5;
    vector<float> out;
    vector<float> out2;


    for(i=1; i<100; ++i) v.push_back(float(i));
    out = smooth_data(v,s);
    for(i=0;i<out.size();i++){
    	cout<<out[i]<<" ";
    }
    cout<<endl;
    
    
    cout<<"simulate data"<<endl;
    fltbank testData = simulate_flt_ez(10, 10, 0.09, 0.0,0.08, 800, 7000,10, 0, 10, 0.2);
    
   
    if (outputfile.is_open())    
    {
        for(i=0;i<800;i++){
            for(j=0;j<7000;j++){
                outputfile << testData.fltdata[i][j] <<" ";
            }
            outputfile<<endl;
        }
        outputfile.close();
    }
    else cout<< "Unable to open the file1";
    
    cout<<"smoothing data"<<endl;
    DM_sftIndex DMsft(10);
    DMsft.cal_sftIdx(testData.freqAxis,testData.timeStep);
    DMsft.get_smoothSize();
    
    fltbank indata(800,7000);

    indata.timeAxis = testData.timeAxis;
    indata.freqAxis = testData.freqAxis;
    if (outputfile2.is_open())    
    {
        for(i=0;i<800;i++){
    	    out2 = smooth_data(testData.fltdata[i],DMsft.smoothSize[i]);
    	    indata.fltdata[i] = out2;
    	    for(j=0;j<7000;j++){
    		    outputfile2 << out2[j] <<" ";
    	    }
            outputfile2<<endl;
        }
        outputfile2.close();
    }
    else cout<< "Unable to open the file2";
    /* Test dedispersion*/
    outdataFbin = testData.freqAxis.size();
    outdataTbin = testData.timeAxis.size()+ DMsft.sftIdx.back();
    fltbank dedspsd(outdataFbin,outdataTbin);
    
    cout<<"Do dedsps"<<endl;
    dedspsd.set_freqAxis(testData.freqAxis.front(),testData.freqStep);
    dedspsd.set_timeAxis(testData.timeAxis.front(),testData.timeStep);
     
    status = do_dedsps(indata, dedspsd, DMsft);
    
    if (outputfile3.is_open())    
    {
    	for(i=0;i<dedspsd.freqAxis.size();i++){
    		for(j=0;j<dedspsd.timeAxis.size();j++){
    			outputfile3 << dedspsd.fltdata[i][j] << " ";
    		}
    		outputfile3<<endl;
    	}
    	outputfile3.close();
    }
    else cout<< "Unable to open the file3";

    return 0;
}