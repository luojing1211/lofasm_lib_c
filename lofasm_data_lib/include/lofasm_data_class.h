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
#include "lofasm_raw_file.h"
#include "lofasm_intgr.h"
#define SEC_PER_DAY 86400.0
#define MAX_FILENAME_SIZE 80
using namespace std;


class lofasm_file
/*
A class for lofasm fileinfo wapper
*/
{
    public:
        string filename; // file Name
        int fileType;            // Data file type

        lofasm_raw_file rawFile;
        //TODO : add other file type;

        double startMJD;
        double startFreq;
        double freqStep;
        int numFreqBin;
        double timeStep;
        int numIntgr;
        long fileSize;

        int check_file_type(string filename);
        void read_hdr(string filename);
        void print_hdr();
        void check_file_info(string filename);
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
        vector <lofasm_file> fileList;
        string currFilename;
        int currFileIdx;
        int currFileType;
        double currMjd;
        double currTimeStep;
        long currFileSize;
        int numfbin;   // number of frequency bin for the file
        int numtbin;
        lofasm_file * currFileInfo;
        lofasm_intgr itgr;
        ifstream currFile;

        void add_file(string filename);   // add file to fileList
        //void open_next_file();
        void open_file(int fileIdx);
        void read_one_intgr(int intgrIdx);
        //TODO: add read data block
};


#endif
