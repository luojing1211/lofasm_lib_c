#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
#include"lofasm_dedsps_class.h"
#include "lofasm_data_class.h"
using namespace std;


int main(){
    int i,j;
    int numIntgr;
    int numfbin;
    lofasm_data ldata;
    string filename;

    filename = "/Users/jingluo/Research_codes/LoFASM/lofasm_lib_c/lofasm_data_lib/lofasm_raw_file/20151012_213004.lofasm";

    ldata.add_file(filename);
    cout<<"Hello";
    ldata.open_file(0);
    numIntgr = ldata.currFileInfo -> numIntgr;
    numfbin = ldata.numfbin;
    fltbank lofasmData(numfbin,numIntgr);

    lofasmData.set_freqAxis(ldata.currFileInfo->startFreq,ldata.currFileInfo->freqStep);
    lofasmData.set_timeAxis(ldata.currFileInfo->startMJD*86400.0,ldata.currFileInfo->timeStep);
    // Test AA

    for (j=0;j<numIntgr;j++){
        ldata.read_one_intgr(j);
        for (i=0;i<numfbin;i++){
            lofasmData.fltdata[i][j] = (float)ldata.itgr.AA[i];
        }

    }

    ldata.currFile.close();
    for (j=0;j<10;j++){
        for(i=0;i<numfbin;i++){
            cout<<lofasmData.fltdata[i][j];
        }
    }
    return 0;


}
