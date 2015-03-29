#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include "lofasm_data_IO.h"
#include "lofasm_data_type.h"


int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    LoFASMIO IOpar = {};
    
    double intgrMJD;
    char *filename[argc-1];
    int numFiles;
    char *freamName;
    int i,j;
    int status;

    filename[0] = argv[1];
    printf("%s\n",filename[0]);
    numFiles = 1;
    status = lofasm_set_file_read(&IOpar, filename, numFiles, 1, "STARTTIME");
    
    

    return status;
    			
}



