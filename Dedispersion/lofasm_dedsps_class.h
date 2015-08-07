
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

/*Define DM Vs shift index class*/
class DM_sftIndex
{
    public:
        double DM;
        double timeDelay;
        double tStep;
        int lastSft1Idx;
        vector<int> sftIdx;
        vector<int> smoothSize;

        DM_sftIndex (double dm, double tStep);
        void cal_sftIdx(vector<double> freqAxis);
        void get_smoothSize();
};
/*Finish define DM_sftIndex*/


/* Declear functions */
vector<float> smooth_data(vector<float> data, int smoothSize);
#endif


