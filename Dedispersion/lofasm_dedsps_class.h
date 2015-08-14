
/*Dedispersion cpp version header file */
#ifndef LOFASM_DEDSPS_CLASS_H_
#define LOFASM_DEDSPS_CLASS_H_

#include<math.h>
#include<string>
#include<vector>
using namespace std;

/*Define the filtbank data class*/
class fltbank
{
    public:
        int numFreqBin;
        int numTimeBin;
        vector< vector<float> > fltdata;
        vector<double> freqAxis;
        double freqStep;

        vector<double> timeAxis;
        double timeStep;
        fltbank (int numfbin,int numtbin);
        void set_freqAxis(double fStart, double fStep);
        void set_timeAxis(double tStart, double tStep);
};
/* Finish define fltbank data class*/

/*Define DM with shift index class*/
class DM_sftIndex
{
    public:
        double DM;
        double timeDelay;
        double tStep;
        int lastSft1Idx;
        vector<int> sftIdx;
        vector<int> smoothSize;

        DM_sftIndex (double dm);
        void cal_sftIdx(vector<double> freqAxis, double timeStep);
        void get_smoothSize();
};
/*Finish define DM_sftIndex*/

/* Define DM vs time class*/
class DM_time
{
    public:
        vector<double> dmAxis;
        double dmStep;
        int numDM;
        double dmStart;

        vector<double> timeAxis;
        double timeStart;
        double timeStep;
        int numTimeBin;

        vector< vector<float> > DM_time_power;
        vector<float> normArray;
        
        DM_time (int numDMbin, int numTBin, double tStep);
        void set_timeAxis(double timeStart);
        void set_dmAxis(double dmStart,double dmStep);
        void set_normArray();
        void set_DM_time_power();
       
};

#endif


