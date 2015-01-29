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
#include<string.h>



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

int lofasm_print_hdr_info(LoFASMIO *IOpar,char *key)
/* To print the header information on the screen. 
   It assumes that the header has already been read. 
*/

{
	int status;
	int i;
	/* Change lower case to upper case*/
	for(i=0;i<strlen(key);i++)
	{
		if(key[i]>='a' && key[i]<='z')
		{	
			key[i] = ('A' + key[i] - 'a');
		}
	}
	
	printf("\n");
	if(strcmp(key,"FILE_NAME")==0)
	{
		printf("FILE_NAME %s\n",IOpar->hdr.filename);
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
int lofasm_read_unsignedint_data(LoFASMIO *IOpar, int intgrReadNum, 
								unsigned int *udata, FILE *fp)
/* Read N integration of unsigned int data
	Unsigned int data includs:
								Frame AA
								Frame BB
								Frame CC
								Frame DD
*/
{
	int status;
	hdrInfo *hdr = &IOpar->hdr;  // Fix me for muiltple file. 
	dataIntgr *intgr = &IOpar->intgr;
	int intgrIndex;


	/* Reading data from different type of file */
	switch(hdr->fileType)
	{
		case 1:     /*** Read LoFASM raw data ***/
			if(intgr->initFlag != 1)
			{
				fprintf(stderr, "Please initialize data container first.\n");
				exit(1);
			}
			/* Check if the *udata has enought space for the */
			
			/* Here put a check flag for the memory size. */




			status = read_raw_intgr(IOpar,fp,0);


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