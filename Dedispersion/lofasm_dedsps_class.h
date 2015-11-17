
/*Dedispersion cpp version header file */
#ifndef LOFASM_DEDSPS_CLASS_H_
#define LOFASM_DEDSPS_CLASS_H_

#include<math.h>
#include<string>
#include<vector>
using namespace std;

/*Define the filtbank data class*/
class fltbank
/*
A class for filter bank data.
Data will be stored in a 2D c++ vector, x axis time and
y axis frequency.  The information of the data will also
be stored in the class members.
Public Members:
----------
numFreqBin : int
    Number of frequency bins/channels
numTimeBin : int
    Number of time sample
fltdata : 2D Vector float/vertor<vector <float>>
    Filter bank data
freqAxis : Vector double/verotr<double>
    frequency value/y axis unit(MHz)
freqStep : double
    Step for frequency axis unit(MHz)
timeAxis : double
    Time sample/x axis unit (s)
timeStep : double
    Step for time axis unit(s)
Public Method:
-------------
fltbank (int numfbin,int numtbin):
    Initialize fltbank class
void set_freqAxis(double fStart, double fStep):
    Set the frequency Axis
void set_timeAxis(double tStart, double tStep);
    Set the time Axis
*/
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
        void resize_time_bin(int newTimeBin);
};
/* Finish define fltbank data class*/

/*Define DM with select index class*/
class DM_sltIndex
/*
A class for one DM value and its selected data index depend
on the frequency and time step.
Backgroud:
Due to the dispersion effect, radio signal have different
velocity in the media. Higher frequency signals travel faster
then lower ones. The signal arrival time differency will have
a relationship:
dt = 4.15e3*DM*(1/f^2-1/f_ref)
Here DM is called dispersion measure unit ( pc*cm^-3)
Depend on the time step and frequency and frequency step. Data
could smear out for more then one time bin for one frequency bin.

The select index include the beginning smear index and end index in
filter bank data. In our de-dispersion algorithm. All the smeared
data will be selected and added to the non-disperion signal arrival time.
So we call it select index.

Here the reference frequency is the highest frequency

Public Members:
----------
DM : double
    DM value
timeDelay : double
    Signal arrival time delay at some frequency based on reference frequency.
tStep: double
    Time step for the filter bank data
lastSft1Idx : int
    For calculate the lowest frequency smear out index. It is for a imaginary
    frequency bin which has one frequency step below the lowest frequency.
normNum : int
    Total number of selected data. For the purpose of normalizing data.
freqCutTree : int
    Cutting frequency for tree method(our algorithm). Any frequency bin
    high then the cutting frequency will not do any operation in our algorithm.
sftIdx : vector<int>
    Singal arrival time delay in the sense of data index.
    shift index  = time delay/time step
smearSize : vector<int>
    Smear size. For the purpose of simulate data.
sltIdx : vector<vetor<int> >/numFreqBin*2 vector int
    Selected index vector
    sltIdx[frequency index][0] is the beginning index of signal for one freqency
    slfIdx[frequency index][1] is the end index of signal for one frequency
Public Method
----------
DM_sltIndex :
    Initialize DM_sltIndex class
void cal_sftIdx(vector<double> freqAxis, double timeStep, double refFreq) :
    Calculate shift index
void cal_sltIdx(vector<double> freqAxis, double timeStep, double refFreq) :
    Calculate selected index
void cal_normNum() :
    Calculate normalize number
void get_smearSize() :
    get the smearSize
*/
{
    public:
        double DM;
        double timeDelay;
        double tStep;
        int lastSft1Idx;
        int normNum;
        int freqCutTree; // Cutting frequency for tree method.
        vector<int> sftIdx;
        vector<int> smearSize; // Change to smearSize
        vector<vector<int> > sltIdx; /* For tree method to select the right data to add */


        DM_sltIndex (double dm);
        void cal_sftIdx(vector<double> freqAxis, double timeStep, double refFreq);
        // Calculate select index based on higher frequency.
        void cal_sltIdx(vector<double> freqAxis, double timeStep, double refFreq);
        // Calculate the normalize number
        void cal_normNum();
        void get_smearSize();
};
/*Finish define DM_sltIndex*/

/* Define DM vs time class*/
class DM_time
/*
A class for DM and time plot for DM searching. X axis time, y axis DM number.
Data Total power.

Public Members
-----------
dmAxis : vector<double>
    dm axis/y axis
dmStep : double
    dm step for dm axis
numDM : int
    Total DM trail
dmStart : double
    Start DM for searching
timeAxis : vector<double>
    time axis/x axis
timeStart : double
    Start time for time axis
timeStep : double
    Time step for time axis
numTimeBin : int
    The number of time bins
DM_time_power : vector< vector<float> >
    2D power vector, x direction is for each time bin
    y direction is for each dm bin
    It is the result for didispersion.
normArray : vector<float>
    Normalize number for each DM

Public Method :
----------
DM_time : Initialize DM_time Class
set_timeAxis : Allocate time axis for DM time power
set_dmAxis : Allocate dm axis for DM time power
set_normArray : Allocate the normalize number array
set_DM_time_power : Allocate the DM time power vector
*/
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
