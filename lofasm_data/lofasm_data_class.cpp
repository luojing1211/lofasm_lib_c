#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>

#include "lofasm_data_class.h"
using namespace std;

char * del_blank(char *str)
/* Delete the blanks in a string*/
{
	  char *out = str, *put = str;
	  for(; *str!='\0';++str)
	  {
		    if(*str != ' ')
			  *put++ = *str;
	  }
	  *put = '\0';
	  return out;
}



/* Class methods for lofasm_hdr class*/

void lofasm_hdr::check_file_type(char *filename){
    FILE *fPtr;
    char *buffer, *fileSig;
    size_t result;
    int ftype;
// Open file and check if it is opened right
    fPtr = fopen(filename,"rb");
    if(fPtr == NULL){fputs("File error\n",stderr);exit(1);}

// Allocate buffer, buffer size is 8 characters
    buffer = (char *)malloc(sizeof(char)*8);
    if (buffer == NULL){fputs("Memory error",stderr);exit(1);}

    result = fread(buffer,1,8,fPtr); // Read file signature
    fileSig = del_blank(buffer);     // Delete the space in string

    // Indentify the file type
    if(strncmp(fileSig,"LoCo",4)==0) /*LoFASM raw data*/
        ftype = 1;
    else if (strncmp(fileSig,"sigproc",7) == 0) /*LoFASM sigproc data*/
        ftype = 2;
    else if (strncmp(fileSig,"ASCII",5) == 0)   /*LoFASM ASCII data*/
        ftype = 3;
    else
    {
        printf("%s is a unknown type data file", filename);
        ftype = 0;
    }
    fclose(fPtr);
    free(buffer);
    return ftype;
}

void lofasm_hdr::get_raw_hdr(char * fname){

  /*
  To read LoFASM raw data header from one file.

  */
    int status = 0;
    char *buffer;
    size_t result;
    char Temp[20];
    char *buffer3Bytes;			 			// A buffer for 3 bytes
    unsigned short buffer2Bytes;  			// A buffer for 2 bytes
    FILE *fp;

    strcpy(fname,filename);

    fp = fopen(fname,"rb");
    // Check if the file is LoFASM raw data
    if(fileType != 1)
    {
        fprintf(stderr,
          "File %s is not a LoFASM raw data."
          " Please check your file type at header"
          " structure element fileType\n",filename);
        exit(1);
    }

    // Read header
    rewind(fp);   /*Move the file pointer to the beginning of file*/
    // Allocate buffer
    buffer = (char *)malloc (sizeof(char)*8);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (1);}

    /*** Read header ***/
    // Read file signature
    result = fread(buffer,1,8,fp);
    strcpy(hdrSig,buffer);
    printf("Header sig %s\n",hdrSig);
    // Read header version
    result = fread(buffer,1,8,fp);
    hdrVersion = atoi(buffer);
    printf("Headear version %d\n", hdrVersion);
    // Read header lenght
    result = fread(buffer,1,8,fp);
    hdrLength = atoi(buffer);
    printf("Headear length %d\n",hdrLength);
    // Read LoFASM station
    result = fread(buffer,1,8,fp);
    strcpy(buffer,lofasmStation);
    printf("Lofasm Station %s\n",lofasmStation);
    // Read Number of bins/Channels
    result = fread(buffer,1,8,fp);
    numfBin = atoi(buffer);
    printf("Number of bins %d\n", numBin);
    // Read start frequency in unit of Mhz
    result = fread(buffer,1,8,fp);
    freqStart = atof(buffer);
      printf("Freq start %lf\n", freqStart);
    // Read frequency steps in unit of Mhz
    result = fread(buffer,1,8,fp);
    strcpy(Temp,"0.");
    strcat(Temp,buffer);
    freqStep = atof(Temp);
    printf("Freq step %lf\n",freqStep);

    // Read MJD integer part in days
    result = fread(buffer,1,8,fp);
    mjdDay = atoi(buffer);
    printf("Mjd start integer %d\n", mjdDay);
    // Read MJD fractional part in millisecond
    result = fread(buffer,1,8,fp);
    mjdMsec = atoi(buffer);
    printf("Mjd start fractional %d\n",mjdMsec);
    // Compute the full mjd date
    startMJD = (double)mjdDay + (double)mjdMsec/SEC_PER_DAY/1000.0;
    // Read integration time for each integration
    result = fread(buffer,1,8,fp);
    intgrTime = atof(buffer);
    printf("integration duration %lf\n",intgrTime);
    // Read data format version
    result = fread(buffer,1,8,fp);
    dataFmtVersion = atoi(buffer);
    printf("Data Format version %d\n",dataFmtVersion);
    // Read notes
    result = fread(buffer,1,8,fp);
    notes = buffer;
    printf("Notes %s\n",notes);

    free(buffer);
    readFlag = 1;

}

void lofasm_hdr::read_hdr(char *filename){
   /* A wrapper for reading different types of lofasm data file header. */
    fileType = check_file_type(filename);
    switch (fileType){
        case 1:
            get_raw_hdr(filename)
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            cout<<"Unknown file type. Please check you file."<<endl;
            exit(1);
            break;
    }

}
void lofasm_hdr::print_hdr(){
    return;
}

/* Finish defined the lofasm_hdr class*/


/* Class methods for lofasm_data class*/
void lofasm_data::add_file(char * filename){
    lofasm_hdr fileHDR;
    vector<lofasm_hdr>::iterator it;
    if (fileList.size()==0){
        fileHDR.read_hdr(filename);
        fileList.push_back(fileHDR);
    }
    else{
        it = fileList.begin();
        fileHDR.read_hdr(filename);
        while(*it.startMJD>=fileHDR.startMJD&& it != fileList.end()){
            it++;
        }
        fileList.insert(it,fileHDR)
    }
}

void lofasm_data::get_next_file(){
    
}

void lofasm_data::read_polar()
{
    return;
};
void lofasm_data::get_beam();
{
    return;
};
