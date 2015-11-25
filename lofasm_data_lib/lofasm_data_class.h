/*Dedispersion cpp version header file */
#ifndef LOFASM_DATA_CLASS_H_
#define LOFASM_DATA_CLASS_H_

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#define SEC_PER_DAY 86400.0
#define MAX_FILENAME_SIZE 80
using namespace std;


class lofasm_file
/*
A class for lofasm data header
*/
{
    public:
        int fileType;            // Data file type
        char filename[MAX_FILENAME_SIZE]; // file Name
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

        int check_file_type(char *filename);
        void read_hdr(char * filename);
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
        int currFileIdx;
        double currMjd;
        double currTimeStep;
        long currFileSize;
        int numfbin;   // number of frequency bin for the file
        int numtbin;
        lofasm_hdr * currHdr;
        FILE * currfp;

        vector <unsigned int> AA, BB, CC, DD;
        vector <signed int> ABrl, ACrl, ADrl, BCrl, BDrl, CDrl;
        vector <signed int> ABim, ACim, ADim, BCim, BDim, CDim;
        vector <double> freqAxis;

        void add_file(char *filename);   // add file to fileList
        void open_next_file();
        void open_file(int fileIdx);
        void check_file(FILE *fp); //Should it be here?
        void get_beam();
        void get_one_intgr(FILE *fp);
    private:
        void is_hdr_pkt(FILE *fp);


};
#endif
