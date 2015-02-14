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

#include<stido.h>

#define MAX_FILENAME_SIZE 256

typedef struct{
	char * paramFile[MAX_FILENAME_SIZE];


}dedispersion_param;