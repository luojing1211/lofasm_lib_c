#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>

#include "lofasm_data_class.h"
#include "lofasm_raw_file.h"
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

int lofasm_file::check_file_type(string filename){
    int filetype;
		string fileSig;
		ifstream infile(filename.c_str(), ios::binary);
	  if (infile.is_open()){
			  // Read header
			  infile >> fileSig;
			  if (fileSig == "LoCo")
					  filetype = 1;
				else if (fileSig == "sigproc")
				    filetype = 2;
			  else if (fileSig == "ASCII" == 0)   /*LoFASM ASCII data*/
				    filetype = 3;
				else{
				    filetype = 0;
						cout<< "File "<<filename<<" type is unknown."<<endl;
        }
		}

		infile.close();
		return filetype;
}



void lofasm_file::read_hdr(string filename){
   /* A wrapper for reading different types of lofasm data file header. */
    fileType = check_file_type(filename);
    switch (fileType){
        case 1:
            cout<<fileType<<endl;
            rawFile.get_raw_hdr(filename);
						startMJD = rawFile.startMJD;
		        startFreq = rawFile.freqStart;
		        freqStep = rawFile.freqStep;
		        numFreqBin = rawFile.numfBin;
		        timeStep = rawFile.intgrTime;
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
void lofasm_file::print_hdr(){
    return;
}


void lofasm_file::check_file_info(string filename){
	  fileType = check_file_type(filename);
	  switch (fileType){
			  case 1:
					  cout<<fileType<<endl;
					  rawFile.check_raw_file(filename);
						numIntgr = rawFile.numIntgr;
						fileSize = rawFile.fileSize;
					  break;
			  case 2:
					  break;
			  case 3:
					  break;
			  default:
					  cout<<"Unknown file type. Please check your file type."<<endl;
					  exit(1);
					  break;
    }
}
/* Finish defined the lofasm_hdr class*/


/* Class methods for lofasm_data class*/
void lofasm_data::add_file(string filename){
    lofasm_file lfile;
    vector<lofasm_file>::iterator it;
    lfile.filename = filename;
    if (fileList.size()==0){
        lfile.read_hdr(filename);
				lfile.check_file_info(filename);
        fileList.push_back(lfile);
    }
    else{
        it = fileList.begin();
        lfile.read_hdr(filename);

        while(it->startMJD>=lfile.startMJD&& it != fileList.end()){
            it++;
        }
        lfile.check_file_info(filename);
        fileList.insert(it,lfile);
    }
}

void lofasm_data::open_file(int fileIdx){
/* Open a file by file index */

    if (fileIdx>=fileList.size()){
        cout << "Not enough files to open."<<endl;
        exit(1);
    }
    if (currFile.is_open()){
        currFile.close();
    }

    currFileInfo = &fileList[fileIdx];
		cout<<currFileInfo->filename;
    currFile.open(currFileInfo->filename.c_str(),ios::binary);
		if (!currFile.is_open()){
			  cout<< "Open file ["<<currFileInfo->filename<<"] filed."<<endl;
				exit(1);
		}

    currFilename = currFileInfo->filename;

    numfbin = currFileInfo->numFreqBin;
    currTimeStep = currFileInfo->timeStep;
    currFileIdx = fileIdx;
    currFileType = currFileInfo->fileType;
    currFileSize = currFileInfo->fileSize;
    currFile.seekg(0);
}

void lofasm_data::read_one_intgr(int intgrIdx){
	  switch (currFileType) {
	  	case 1:
			    get_lofasm_raw_intgr(currFileInfo->rawFile, itgr, intgrIdx,currFile);
				  break;
			case 2:
			    break;
			case 3:
			    break;
			default:
			    cout<<"Unknown file type."<<endl;
					exit(1);
	  }

}
