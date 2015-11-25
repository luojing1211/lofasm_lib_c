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

int lofasm_hdr::check_file_type(char *filename){
    FILE *fPtr;
    char *buffer, *fileSig;
    size_t result;
    int filetype;

// Open file and check if it is opened right
    fPtr = fopen(filename,"rb");
    if(fPtr == NULL){fputs("File error\n",stderr);exit(1);}
// Allocate buffer, buffer size is 8 characters
    buffer = (char *)malloc(sizeof(char)*8);
    if (buffer == NULL){fputs("Memory error",stderr);exit(1);}

    result = fread(buffer,1,8,fPtr); // Read file signature

    fileSig = del_blank(buffer);     // Delete the space in string
    free(buffer);
    buffer = NULL;
    // Indentify the file type
    if(strncmp(fileSig,"LoCo",4)==0){ /*LoFASM raw data*/
        filetype = 1;
    }
    else if (strncmp(fileSig,"sigproc",7) == 0) /*LoFASM sigproc data*/
       filetype = 2;
    else if (strncmp(fileSig,"ASCII",5) == 0)   /*LoFASM ASCII data*/
        filetype = 3;
    else
    {
       printf("%s is a unknown type data file", filename);
       filetype = 0;
    }
    fclose(fPtr);
    return filetype;
}

void lofasm_hdr::read_hdr(char *filename){
   /* A wrapper for reading different types of lofasm data file header. */
    fileType = check_file_type(filename);
    switch (fileType){
        case 1:
            cout<<fileType<<endl;
            get_raw_hdr(filename);
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

        while(it->startMJD>=fileHDR.startMJD&& it != fileList.end()){
            it++;
        }

        fileList.insert(it,fileHDR);
    }
}

void lofasm_data::open_file(int fileIdx){
/* Open a file by file index */

    if (fileIdx>=fileList.size()){
        cout << "Not enough files to open."<<endl;
        exit(1);
    }
    if (currfp!=NULL){
        fclose(currfp);
        currfp = NULL;
    }

    currHdr = &fileList[fileIdx];
    cout<<currHdr->filename;
    currfp = fopen(currHdr->filename,"rb");
    //if (!currfp){
    //    cout << "Open file "<<currHdr->filename<<"failed. Please check your file."<<endl;
    //    exit(1);
    //}
    currFile = currHdr->filename;

    numfbin = currHdr->numfBin;
    currTimeStep = currHdr->intgrTime;
    currFileIdx = fileIdx;

    cout<<currHdr->mjdMsec<<endl;
    /* Get file size */
    fseek (currfp, 0, SEEK_END);   // non-portable
    currFileSize = ftell (currfp);
    rewind(currfp);
}

void lofasm_data::check_file(){

}


void lofasm_data::open_next_file(){
    return;
}

void lofasm_data::get_one_intgr(FILE *fp)
{
    return;
}
void lofasm_data::get_beam()
{
    return;
}
