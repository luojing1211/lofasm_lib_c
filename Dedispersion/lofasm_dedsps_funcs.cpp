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





