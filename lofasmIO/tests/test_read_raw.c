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
    int intgrIndex;

    filename[0] = argv[1];
    printf("%s\n",filename[0]);
    numFiles = 1;
    status = lofasm_set_file_read(&IOpar, filename, numFiles, 1, "STARTTIME");
    
    //status = lofasm_set_freq(&IOpar,5.0,88.0);
    
    status = lofasm_set_frame(&IOpar,"AB");    
    status = lofasm_open_file(&IOpar.currentFile->hdr,IOpar.currentFile->filename,
                                &fp, "r");
    status = init_raw_reading(&IOpar);
    
    intgrIndex = 0;
    
    for(intgrIndex = 0; intgrIndex<IOpar.currentFile->hdr.numIntgr;intgrIndex++)
    {   
        printf("Integ pos %ld\n",IOpar.currentFile->hdr.intgrList[intgrIndex].intgrPos); 
        printf("Integration index  %d %ld\n",intgrIndex,ftell(fp));
        status = read_raw_intgr(&IOpar,fp,intgrIndex);
    }

    return status;
    			
}



