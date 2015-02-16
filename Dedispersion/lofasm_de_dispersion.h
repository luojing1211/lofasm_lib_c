/*  
-Header_File lofasm_de_dispersion.h    

-Abstract     
    Header file of LoFASM filter bank data de-disperion. 
   

-Author_and_Institution
    Jing Luo    Center for Advanced Radio Astronomy (CARA)

-Version
    LoFASM DATA I/O library 2.0.0. 26-JAM-2015
*/


#ifndef LOFASM_DE_DISPERSION_H_
#define LOFASM_DE_DISPERSION_H_

#include<stdio.h>
#include "lofasm_data_IO.h"

#define MAX_NUM_FILE 5
#define MAX_FILENAME_SIZE 256

typedef struct{
	char paramFile[MAX_FILENAME_SIZE];
	char *dataFileNames[MAX_NUM_FILE];
	int numDataFile;

	double DMstart;
	int numDM;
	double DMstep;

	double freqStart;
	double freqEnd;

	double timeIntrv;
	
	char spectrumName[10];

}dedispersion_param;

int dedsps_read_flags(dedispersion_param *dedspsPar, int argc, char *argv[]);

int dedsps_read_para_file(dedispersion_param *dedspsPar, FILE *fp);

void dedsps_check_line(dedispersion_param *dedspsPar, char *str, FILE *fp);

#endif