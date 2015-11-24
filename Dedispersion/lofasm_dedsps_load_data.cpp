/* This file is for the propers of doing lofasm dedispersion continues.

*/

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <time.h>
#include <utility>
#include "lofasm_dedsps_class.h"
#include "lofasm_utils.h"
using namespace std;


fltbank* init_fltbank_class(int numfBin, int numtBin){
    fltbank* fb = new fltbank(numfBin,numtBin);
    return fb;

}


int main(){
     fltbank* dtapt1;
     fltbank* dtapt2;
     fltbank* testband;
     int i,j;
     dtapt1 = init_fltbank_class(100,10);
     dtapt1->set_freqAxis(0,1.0);
     dtapt1->set_timeAxis(1,1.0);
     for (i=0;i<dtapt1->numFreqBin;i++){
         for(j=0;j<dtapt1->numTimeBin;j++){
             dtapt1->fltdata[i][j]=j+1;
         }
     }
     dtapt2 = init_fltbank_class(100,10);
     dtapt2->set_freqAxis(0,1.0);
     dtapt2->set_timeAxis(20.5,1.0);
     for (i=0;i<dtapt2->numFreqBin;i++){
         for(j=0;j<dtapt2->numTimeBin;j++){
             dtapt2->fltdata[i][j]=j+20.5;
         }
     }
     testband = combine_fltbank_time(*dtapt1,*dtapt2,1);
     for (i=0;i<testband->numFreqBin;i++){
         for(j=0;j<testband->numTimeBin;j++){
             cout<<testband->fltdata[i][j]<<" ";
         }
     }
     delete dtapt1;
     delete testband;
}
