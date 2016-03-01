#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
#include"lofasm_dedsps_class.h"
#include "lofasm_data_class.h"
#include "lofasm_dedsps_funcs.h"
using namespace std;


int main(int argc, char *argv[]){
    int i,j;
    int numIntgr;
    int numfbin;
    double tstart, tstep;
    double fstart, fstep;
    lofasm_data ldata;
    string filename;
    ofstream outputfile1("lofasm_dedsps_test.dat");
    ofstream outputfile2("lofasm_band_data.dat");
    filename ="/Users/jingluo/Research_codes/LoFASM/lofasm_lib_c/test_data/20151012_213004.lofasm";
    ldata.add_file(filename);
    cout<<"Hello";
    ldata.open_file(0);
    numIntgr = ldata.currFileInfo -> numIntgr;
    tstart = ldata.currFileInfo -> startMJD * 86400.0;
    tstep = ldata.currFileInfo -> timeStep;
    fstart = ldata.currFileInfo->startFreq;
    fstep = ldata.currFileInfo->freqStep;
    numfbin = ldata.numfbin;
    fltbank lofasmData(numfbin,numIntgr);
    fltbank *banddata;
    lofasmData.set_freqAxis(ldata.currFileInfo->startFreq,ldata.currFileInfo->freqStep);
    lofasmData.set_timeAxis(ldata.currFileInfo->startMJD*86400.0,ldata.currFileInfo->timeStep);
    // Test AA
    // Load data to memory
    for (j=0;j<numIntgr;j++){
        ldata.read_one_intgr(j);
        for (i=0;i<numfbin;i++){
            lofasmData.fltdata[i][j] = (float)ldata.itgr.AA[i];
        }

    }
    ldata.currFile.close();

    // get narrow band of data
    cout<<"Get narrow band of data"<<endl;
    double freqS;  //start frequency
    double freqE;  // End frequency
    int numBin;
    double fstartband;
    double tstartband;
    freqS = atof(argv[1]);
    freqE = atof(argv[2]);
    tstartband = ldata.currFileInfo->startMJD*86400.0;
    banddata = lofasmData.get_freq_band(freqS, freqE);
    //banddata.set_freqAxis(fstartband,fstep);
    //Sbanddata.set_timeAxis(tstart,tstep);
    if (outputfile2.is_open())
    {
        for(i=0;i<banddata->fltdata.size();i++){
            for(j=0;j<banddata->fltdata[0].size();j++){
                outputfile2 << banddata->fltdata[i][j] << " ";
            }
            outputfile2<<endl;
        }
        outputfile2.close();
    }

    // init de-dispersion
    DM_time* DMT = dm_search_tree(*banddata,0,10,0);
    cout<<"write data\n";
    if (outputfile1.is_open())
    {
        for(i=0;i<DMT->DM_time_power.size();i++){
            for(j=0;j<DMT->DM_time_power[0].size();j++){
                outputfile1 << DMT->DM_time_power[i][j] << " ";
            }
            outputfile1<<endl;
        }
        outputfile1.close();
    }
    else cout<< "Unable to open the file4";



    return 0;
}
