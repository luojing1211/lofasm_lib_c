/*Dedispersion cpp version header file */
#ifndef LOFASM_DATA_CLASS_H_
#define LOFASM_DATA_CLASS_H_

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#define SEC_PER_DAY 86400.0
class lofasm_hdr
/*
A class for lofasm data header
*/
{
    int fileType;            // Data file type
    char *filename;          // file Name
    char *hdrSig;            // Header signature
    int hdrVersion;          // File header version, mostly for raw data
    int hdrLength;           // File header lenght
    char *lofasmStation;     // LoFASM station name
    int numfBin;                 // Number of Channels/bins
    double freqStart;              // Start Frequency
    double freqStep;               // Frequency step
    int mjdDay;                 // Data taking date in mjd formate,
                                //   the integer part or mjd
    int mjdMsec;                // Data taking date in mjd formate,
                                //   the fractional part in milliseconds
    double startMJD;             // file start mjd date to milliseconds unit in day
    double endMJD;               // File end mjde data
    double intgrTime;              // Integration total time for each integration
    int dataFmtVersion;         // data format version
    char *notes;             // notes for the file
    long fileSize;               // File size
    int numIntgr;               // Number of integration in the file
    char numIntgrFlag;
    int packetSize;             // Size of each data packet in bytes
    int intgrSize;              // Size of each data integration in bytes
    int hdrPktSig;

    int numBadIntgr;
    int lenBadIntgrList;	// Current lenght of _bad_ integration ID
    int readFlag;           // If header has been correctly read, readFlag == 1
    int fileCheckFlag;

    void get_hdr(char * filename);
    void print_hdr();
};


class lofasm_data
/*
A class for lofasm data.

Public Members
----------
fileList

*/
{
    public:
        vector <lofasm_hdr> fileList;
        string currFile;
        double currMjd;
        double currTimeStep;
        size_t currFileSize;
        int numfbin;
        int numtbin;
        fstream currFile;
        lofasm_hdr * currHdr;

        vector <unsigned int> AA, BB, CC, DD;
        vector <signed int> ABrl, ACrl, ADrl, BCrl, BDrl, CDrl;
        vector <signed int> ABim, ACim, ADim, BCim, BDim, CDim;
        vector <double> freqAxis;

        void add_file();   // add file to fileList
        void get_next_file();
        void check_file(); //Should it be here?
        void read_polar();
        void get_beam();
        void read_intgrs(int numIntgr);


};
#endif
