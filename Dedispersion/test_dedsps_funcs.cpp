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
    ofstream outputfile("sim_flt.dat");

	s = 5;
    vector<float> out;
    
    for(i=1; i<100; ++i) v.push_back(float(i));
    out = smooth_data(v,s);
    for(i=0;i<out.size();i++){
    	cout<<out[i]<<" ";
    }
    cout<<endl;
    

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
    else cout<< "Unable to open the file";
    
    return 0;
}