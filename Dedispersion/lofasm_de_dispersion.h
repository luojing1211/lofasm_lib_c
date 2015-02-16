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

}dedispersion_param;

int dedsps_read_flags(dedispersion_param *dedspsPar, int argc, char *argv[]);

int dedsps_read_para_file(LoFASMIO *IOpar);

#endif