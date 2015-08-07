#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <numeric>
#include "lofasm_dedsps_class.h"
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
    return 0;
}