/******************************************************************************
Copyright : Center for Advanced Radio Astronomy, CARA, UTB. 

Filename : Source code lofasm_data_IO.c

Author: Jing Luo UTB, Center for Advanced Radio Astronomy

Version : 2.0.0

Data : 10/16/2014

Description:
This program is a part for LoFASM c library.
It is designed for LoFASM data input and output over all contral. 
The accepted data formats are:
    1. LoFASM binary raw data
    2. LoFASM sigproc file
    3. LoFASM txt file
***********************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "lofasm_data_IO.h"
#include<string.h>


/******* Functions for reading ***************/

int lofasm_set_file_read(LoFASMIO *IOpar,char *fileNames[], int numFiles)
{
	/*
		These funtion is to set up 
		Build up the file queue and read the all file header.
	*/
    int status;
    fileNode * currNode = NULL; 
    //struct hdrinfo *hdr = &(IOpar->hdr);
    //struct dataIntgr *intgr = &(IOpar->intgr);
    int i;

    /* Create the head node */
    if(currNode != NULL)
    {
    	fprintf(stderr,"Current node has been allocated.\n");
    	exit(1);
    }

  	currNode = creat_file_node(fileNames[0],0);
  	IOpar-> fileQhead = currNode;

	/*Create file queue*/	
	for(i=1;i<numFiles;i++)
	{
		if(currNode->nextFile != NULL)
    	{
    		fprintf(stderr,"next file node has been allocated.\n");
    		exit(1);
    	}
		/* Create the next node under current-> next pointer */
		currNode -> nextFile = creat_file_node(fileNames[i],i);
		currNode -> nextFile -> lastFile = currNode;
		/* Move current to next node */
		currNode = currNode -> nextFile;
	}
	IOpar-> fileQend = currNode;
	return status;
}

