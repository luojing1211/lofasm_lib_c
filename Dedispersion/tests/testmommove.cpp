#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <numeric>
#include "lofasm_dedsps_class.h"
using namespace std;


int main(){
	vector<int> v;
	int s,i,j;
    
    s = 5;
    vector<int> out;
    vector<int> temp(s,0);
    
    for(i=1; i<100; ++i) v.push_back(i);
    out = v;
    copy(v.begin(),v.begin()+s,temp.begin());
    for(i = 0;i<v.size()-5;i++){
    	out[i] = accumulate(temp.begin(),temp.end(),0);
    	rotate(temp.begin(),temp.begin()+1,temp.end());
    	temp.back() = v[i+s];

    	for(j=0;j<out.size();j++){
    		cout<<out[j]<<" ";
    	}
    	cout<<"\n";       
    }
    
    return 0;
}