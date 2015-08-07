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

	s = 5;
    vector<float> out;
    
    for(i=1; i<100; ++i) v.push_back(float(i));
    out = smooth_data(v,s);
    for(i=0;i<out.size();i++){
    	cout<<out[i]<<" ";
    }
    cout<<endl;
    

    fltbank testData = simulate_flt_ez(10, 10, 0.5, 0.0,0.08, 100, 1000,10, 0, 1, 0.2);

    for(i=0;i<100;i++){
    	for(j=0;j<1000;j++){
    		cout<< testData.fltdata[i][j]<<" ";
    	}
    	cout<<endl;
    }
    
    return 0;
}