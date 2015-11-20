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

int main(int argc, char *argv[]){
    ifstream datafile("fld.dat");
    ifstream freqfile("freq.dat");
    ofstream outputfile4("DMTLWA.dat");
    ofstream outputfile1("DedspsLWA.dat");
    int numTbin,numFbin;
    double fstart,fsecond,fstep;
    double tstart,tstep;
    int i,j;
    double DMv;
    numTbin = 64;
    numFbin = 128;
    tstart = 0;
    tstep = double(1.0)/3.951551372963/64.0;
    fltbank folddata(128,64);
    // Get frequency
    if (freqfile.is_open())
    {
        freqfile>>fstart;
        freqfile>>fsecond;
        freqfile.close();
    }
    else  cout << "Unable to open file freq.dat";

    fstep = fsecond-fstart;

    folddata.set_freqAxis(fstart,fstep);
    folddata.set_timeAxis(tstart,tstep);
    // Get data
    if (datafile.is_open())
    {
        for(i=0;i<numFbin;i++)
        {
            for(j=0;j<numTbin;j++)
            {
                datafile >> folddata.fltdata[i][j];
            }
        }
        datafile.close();
    }
    else  cout << "Unable to open file fld.dat";

    cout<<"Get narrow band of data"<<endl;
    int freqSI;  //start frequency index
    int freqEI;  // End frequency index
    int numBin;
    double fstartband;
    freqSI = atoi(argv[2]);
    freqEI = atoi(argv[3]);
    numBin = freqEI-freqSI+1;
    fstartband = folddata.freqAxis[freqSI];
    fltbank banddata(numBin,64);
    banddata.set_freqAxis(fstartband,fstep);
    banddata.set_timeAxis(tstart,tstep);

    for(i=0;i<numBin;i++)
    {
        cout<< banddata.freqAxis[i]<<" ";
        for(j=0;j<numTbin;j++)
        {
            banddata.fltdata[i][j] = folddata.fltdata[i+freqSI][j];
        }
    }

    // GET dm array set up
    /*double dmStart = 0;
    double dmEnd = 10;

    double dmStep = cal_dmStep_min(folddata.freqAxis.back(),folddata.freqAxis.front(),
                            folddata.timeStep);

    cout<<"dm step "<<dmStep<<endl;
    int dmNUM = (int)((dmEnd-dmStart)/dmStep);

    vector<DM_sltIndex> DMSarray(dmNUM,DM_sltIndex (0.0));
    for(i=0;i<dmNUM;i++){
        DMSarray[i].DM = i*dmStep+0;
        DMSarray[i].cal_sftIdx(folddata.freqAxis,folddata.timeStep,folddata.freqAxis.front());
        DMSarray[i].get_smearSize();
        DMSarray[i].cal_sltIdx(folddata.freqAxis,folddata.timeStep,folddata.freqAxis.back());
        DMSarray[i].cal_normNum();
        //cout<<DMSarray[i].normNum<<endl;
    }

    cout<<"Calculate cut freq index"<<endl;
    for(i=1;i<dmNUM;i++){
        DMSarray[i].freqCutTree = cal_cut_freq_index(DMSarray[i],DMSarray[i-1]);
        //cout<<DMSarray[i].freqCutTree<<endl;
    }


    cout<<"initialize result data "<<endl;
    int outdataFbin = folddata.freqAxis.size();
    int outdataTbin = folddata.timeAxis.size()+100;

    cout<<"Create DM_T_power data."<<endl;

    DM_time DMT(dmNUM,outdataTbin,folddata.timeStep);
    DMT.set_dmAxis(0,dmStep);
    DMT.set_timeAxis(0.0);
    DMT.set_DM_time_power();
    DMT.set_normArray();
    int status = compute_DM_t_power_tree_band(folddata, DMT, DMSarray);*/
    DM_time* DMT = dm_search_tree(banddata,0,10,0);
    cout<<"write data\n";
    if (outputfile4.is_open())
    {
        for(i=0;i<DMT->DM_time_power.size();i++){
            for(j=0;j<DMT->DM_time_power[0].size();j++){
                outputfile4 << DMT->DM_time_power[i][j] << " ";
            }
            outputfile4<<endl;
        }
        outputfile4.close();
    }
    else cout<< "Unable to open the file4";


    //get dedsps
    cout<<"get dedispered filter bank data"<<endl;
    fltbank outdata(numFbin,numTbin+100);
    DMv = atof(argv[1]);
    DM_sltIndex DMR (DMv);
    do_dedsps_curve(banddata, outdata, DMR);

    if (outputfile1.is_open())
    {
        for(i=0;i<outdata.fltdata.size();i++){
            for(j=0;j<outdata.fltdata[0].size();j++){
                outputfile1 << outdata.fltdata[i][j] << " ";
            }
            outputfile1<<endl;
        }
        outputfile1.close();
    }
    else cout<< "Unable to open the file DedspsLWA.dat";


    return 0;
}
