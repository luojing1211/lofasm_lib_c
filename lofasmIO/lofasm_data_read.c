/******************************************************************************
Copyright : Center for Advanced Radio Astronomy, CARA, UTB. 

Filename : Source code lofasm_data_read.c

Author: Jing Luo UTB, Center for Advanced Radio Astronomy

Version : 2.0.0

Data : 1/27/2015

Description:
This program is a part for LoFASM c library.
It is designed for LoFASM data IO read part functions. 
The accepted data formats are:
    1. LoFASM binary raw data
    2. LoFASM sigproc file
    3. LoFASM txt file

Function list:
	lofasm_read_file_hdr()
	lofasm_print_hdr_info()
	lofasm_read_unsignedint_data()
***********************************************************************/


#include<stdio.h>
#include<stdlib.h>
#include "lofasm_data_IO.h"
#include "lofasm_data_type.h"
#include<string.h>

#define MAX_HDR_KEY_LEN 20
#define MAX_HDR_INFO_LEN 20

/* Header information */

int lofasm_read_file_hdr(hdrInfo *hdr, FILE *fp)
/*
A wrapper for reading different type of files' header. 

Input : LoFASMdrInfo *hdr,char *filename

Output : Fill up LoFASM file header information
		 int status
			 status == 0 :   No error  
            	 status == 1 :   error
*/
{
	int status=0;		
	
	switch(hdr->fileType)
	{
		case 1:     //  Read LoFASM raw data
			printf("Reading LoFASM raw data header from %s\n", hdr->filename);
			
			status = read_raw_hdr(hdr,fp);
			if (status!= 0)
			{
				fprintf(stderr,"Reading header filed, due to an error."
							   "See above.\n");
				exit(1);
			}	
			break;
		case 2:		// Read LoFASM sigproc data	
			printf("Reading LoFASM sigproc data header from %s\n",
					hdr->filename);	
			printf("Reading LoFASM sigproc data is not available now.\n");
			break;
	
 		case 3:   	// Read ASCII data
			printf("Reading LoFASM ASCII data header from %s\n", 
					hdr->filename);	
			printf("Reading LoFASM ASCII data is not available now.\n");
			break;
		default:
			fprintf(stderr,"Unknown file %s\n",hdr->filename);	
			status = 1;			
			break;
	}
	hdr->readFlag = 1;	
	return status;
}

int lofasm_print_hdr_info(LoFASMIO *IOpar, char *key)
/* To print the header information on the screen. 
   It assumes that the header has already been read. 
*/

{
	int status;
	int i;
	size_t keyLen;

	keyLen = strlen(key);
	/* Change lower case to upper case*/
	for(i=0;i<keyLen;i++)
	{
		if(key[i]>='a' && key[i]<='z')
		{	
			key[i] = ('A' + key[i] - 'a');
		}
	}
	
	printf("\n");
	if(strcmp(key,"FILE_NAME")==0)
	{
		printf("%20s %20s\n",key, IOpar->hdr.filename);
	}
	else if(strcmp(key,"HEADER_SIGNATURE")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"HEADER_VERSION")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"HEADER_LENGTH")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"LOFASM_STATION")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"NUM_BIN")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"FREQ_START")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"FREQ_STEP")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"MJD_DAY")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"MJD_MILLISECOND")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"INTGT_TIME")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"DATA_FORMAT_VERSION")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"FILE_TYPE")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"FILE_SIZE")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"NUM_INTGR")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"PACKET_SIZE")==0)
	{
		printf("display");
	} 
	else if(strcmp(key,"INTGR_SIZE")==0)
	{
		printf("display");
	}
	else if(strcmp(key,"NOTES")==0)
	{
		printf("display");
	}

	return status;
	
}


/******** Read data functions *********/

/* Included Functions:
	lofasm_read_unsignedint_data()
*/
int lofasm_read_fltbank_data(LoFASMIO *IOpar, int intgrReadNum, 
								dataArray2D *fltData, int startFltIndex,
								FILE *fp)
{
	int status;
	hdrInfo *hdr = &IOpar->currentFile->hdr;   
	dataIntgr *intgr = &IOpar->intgr;
	int intgrIndex;
	int fptrPos;
	int numBins;
	char *valStr;
	int i,j;
	int beam;
	int value;


	numBins = hdr -> numBin;
	fptrPos = ftell(fp);
	sprintf(valStr, "%d", fptrPos);
	intgrIndex = search_intgr_list(hdr->intgrList,"POS",valStr);

	/* Reading data from different type of file */
	switch(hdr->fileType)
	{
		case 1:     /*** Read LoFASM raw data ***/
		/***** Check if the read condition has been set up *****/
			if(intgr->initFlag != 1)
			{
				fprintf(stderr, "Please initialize data container first.\n");
				exit(1);
			}
			/* Check if the 2d array has allocated */
			if(fltData->alctFlag==0)
			{
				fprintf(stderr,"Filter bank data has not been allocated."
								"Allocatind filter bank data.\n");
		
				fltData = allocate_2d_array(numBins,intgrReadNum,
											IOpar->spectrumDtype);
			}


			/* Check if the data type match the spectrum name */
			if(strcmp(fltData->dtype,IOpar->spectrumDtype)!=0)
			{
				fprintf(stderr,"The data type of 2d array do not match the" 
						" spectrum.\n");
				exit(1);	
			}

			/*Check if the array space is enough*/
			if(numBins > fltData -> numFreqBin)
			{
				fprintf(stderr, "Filter band data array dose not have" 
								"enough space for %i freqency bins. Please"
								" reallocate filter band data array\n",numBins);

				exit(1);
			}

			if(startFltIndex+intgrReadNum > fltData -> numIntgr)
			{
				fprintf(stderr, "Filter band data array dose not have" 
								"enough space for %i integrations. Please"
								" reallocate filter band data array\n",intgrReadNum);
				exit(1);
			}

			/******* Read data *******/

			/**FIXME frame dependent. */
			for(i = 0;i<intgrReadNum;i++)
			{
				/* Check file condition. */
				intgrIndex++;

				if(intgrIndex >= hdr->numIntgr)
				{
					fprintf(stderr, "Finished reading file %s.\n"
									"Jump to next file %s.\n",
									IOpar->currentFile->filename,
									IOpar->currentFile->nextFile->filename);
					/* Jump to next file */
					fclose(fp);
					IOpar->currentFile = IOpar->currentFile->nextFile;
					status = lofasm_open_file(&IOpar->currentFile->hdr, 
  									IOpar->currentFile ->filename, &fp,"r");
					intgrIndex = 0;
				}

				status = read_raw_intgr(IOpar,fp,intgrIndex);
				
				if(strcmp(IOpar->currSpectrum,"EW")==0||
				   strcmp(IOpar->currSpectrum,"NS")==0)
				{	
					if(fltData -> data.fData==NULL)
					{
						fprintf(stderr, "The array is not in the right data type" 
										" %s\n", fltData -> dtype);
						exit(1);
					}
					beam = lofasm_form_beam(IOpar);
					for(j=0;j<numBins;j++)
					{
						fltData -> data.fData[startFltIndex+intgrReadNum][j] = 0;
					}
				}

				else if(strcmp(IOpar->currSpectrum,"AA")==0||
						strcmp(IOpar->currSpectrum,"BB")==0||
						strcmp(IOpar->currSpectrum,"CC")==0||
						strcmp(IOpar->currSpectrum,"DD")==0)
				{
					if(fltData -> data.usData==NULL)
					{
						fprintf(stderr, "The array is not in the right data type" 
										" %s\n", fltData -> dtype);
						exit(1);
					}

					for(j=0;j<numBins;j++)
					{
						fltData -> data.usData[startFltIndex+intgrReadNum][j] = 0; 	
						value = 0;			
					}
				}

				else
				{
					if(fltData -> data.sData==NULL)
					{
						fprintf(stderr, "The array is not in the right data type" 
										" %s\n", fltData -> dtype);
						exit(1);
					}

					for(j=0;j<numBins;j++)
					{
						fltData -> data.sData[startFltIndex+intgrReadNum][j] = 0; 	
						value = 0;			
					}
				}
			}

			
			break;
		case 2:		/*** Read LoFASM sigproc data ***/	
			printf("Reading LoFASM sigproc data is not available now.\n");
			break;
	
 		case 3:   	/*** Read ASCII data ***/
			printf("Reading LoFASM ASCII data is not available now.\n");
			break;
		default:
			fprintf(stderr,"Can not Read unknown file %s\n, please check your"
							" file.\n",hdr->filename);	
			status = 1;			
			break;
	}
	
}


int lofasm_form_beam(LoFASMIO *IOpar)
{
	return 0;
}
