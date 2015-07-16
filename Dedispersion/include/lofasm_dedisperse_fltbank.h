/*  
-Header_File lofasm_dedisperse_fltbank.h    

-Abstract     
    Header file of lofasm_dedisperse_fltbank.c  
    Defined class:
    Defined functions:

-Author_and_Institution
    Jing Luo    Center for Advanced Radio Astronomy (CARA)

-Version
    LoFASM dedispersion library 2.0.0. 10-FEB-2015
*/


#ifndef __LOFASM_DEDISPERSE_FLTBANK_H__
#define __LOFASM_DEDISPERSE_FLTBANK_H__


#include<vector>


/*Define the filtbank data class*/
class fltbank  
{
        int numFreqBin;
        int numTimeBin;
    public:
        std::vector<std::vector<float>> fltdata;
        std::vector<double> freqAxis;
        double freqStep;

        std::vector<double> timeAxis;
        double timeStep;
        fltbank (int numfbin,int numtbin);
        void set_freqAxis(double fstart, double fStep, int numfbin);
        void set_timeAxis(double tStart, double tStep, int numTbin);
        void set_data();
}

