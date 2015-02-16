/******************************************************************************
Copyright : Center for Advanced Radio Astronomy, CARA, UTB. 

Filename : Source code lofasm_data_IO.c

Author: Jing Luo UTB, Center for Advanced Radio Astronomy

Version : 2.0.0

Data : 1/27/2015

Description:
This program is a part for LoFASM c library.
It is designed for LoFASM data IO overall parameter contral. 
The accepted data formats are:
    1. LoFASM binary raw data
    2. LoFASM sigproc file
    3. LoFASM txt file
***********************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "lofasm_data_IO.h"
#include<string.h>


/************** Functions for reading ***************/
/************** Signal file operation ***************/
/* Included functions:
	Check_file_type(char *filename)
	lofasm_open_file()
*/
int check_file_type(char *filename)
/*
To check the input file type. 

Input : char *filename,
        LoFASMdrInfo *hdr

Output: int filetype
        filetype == 0 :  Unknown file type
        filetype == 1 :  LoFAMS raw data
        filetype == 2 :  LoFASM sigproc file
        filetype == 3 :  LoFASM ASCII file

*/
{
    int fileType;
    FILE *fPtr;
	char *buffer, *fileSig;
	size_t result;
	
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
		fileType = 1;
	else if (strncmp(fileSig,"sigproc",7) == 0) /*LoFASM sigproc data*/
		fileType = 2;
	else if (strncmp(fileSig,"ASCII",5) == 0)   /*LoFASM ASCII data*/
		fileType = 3;
	else 
	{
		printf("%s is a unknown type data file", filename);
		fileType = 0;
	}
	fclose(fPtr);
	free(buffer);
    return fileType;
}


int lofasm_open_file(hdrInfo *hdr, char * filename, FILE **fptr, 
					 char * mode)
/*
To check a file type and open file:
	1. File type
	2. Check data status (Miss packet or not)

Input : LoFASMParam *par,
		Char * filename,
		FILE **fptr,
		int mode
			"rb"
			"wb"
			"r"
			"w"

Output : File pointer for the input file. And the status
*/
{	
	int status=0;
	sprintf(hdr->filename,filename);
		
	hdr->fileType = check_file_type(filename);
	
	switch(hdr->fileType)
	{
		case 1:     /*** Open LoFASM raw data ***/
			printf("Opening LoFASM raw data header from %s\n",hdr->filename);
			// Open file
			*fptr = fopen(filename,mode);
			if( *fptr == NULL )
			{
				status = 1;				
				fprintf(stderr,"Open file %s failed\n",hdr->filename);
			}

			break;
		case 2:		/*** Read LoFASM sigproc data ***/	
			printf("Opening LoFASM sigproc data header from %s\n",
					hdr->filename);	
			printf("Opening LoFASM sigproc data is not available now.\n");
			break;
	
 		case 3:   	/*** Read ASCII data ***/
			printf("Opening LoFASM ASCII data header from %s\n", 
					hdr->filename);	
			printf("Opening LoFASM ASCII data is not available now.\n");
			break;
		default:
			fprintf(stderr,"Can not open unknown file %s\n, please check your"
							" file.\n",hdr->filename);	
			status = 1;			
			break;
	}
	 
	return status;
}


/**************** File queue set up ***********************/
/*
	Included functions:
		lofasm_create_file_Q()
		create_file_node()
		sort_file_node()
*/



fileNode * create_file_node(char *fileName, int index)
/* Create file node and read header */
{
	int status;
	FILE *fptr;
	fileNode *node;

    // Allocate memory for the new node
    node = malloc(sizeof(fileNode));
    // Memory error warning
    if (node == NULL) 
    {
    	fprintf(stderr,"Memory error when creating the file nodes.\n");
    	exit(1);
	}
	/* Point the next file to null */
	node -> nextFile = NULL;
	node -> lastFile = NULL;
	/* Put value into the element */
	node -> filename = fileName;
	status = lofasm_open_file(&node->hdr, node -> filename, &fptr,"r");
	status = lofasm_read_file_hdr(&node->hdr,fptr);
	node -> index = index ;
	/* Close the file */
	fclose(fptr);
	return node;
}

int free_file_nodes(fileNode ** node)
/*Free a file node and keep the file Q*/
{
	/*
	int status;
	if(node == NULL)
	{
		fprintf(stderr, "The node has been freed\n");
	}
	fileNode * lastFile = *node.lastFile;
	fileNode * nextFile = *node.nextFile;

	lastFile -> nextFile = nextFile;
	nextFile -> lastFile = lastFile;

	free(node);
	node =  NULL;
	*/
	return 0;
}

/* Build file queue */
int lofasm_set_file_read(LoFASMIO *IOpar,char *fileNames[], int numFiles
						,char sortFlag, char *sortKey)
{
	/*
		1. Build up the file queue.
		2. Read file header
		3. Sort file node
		4. Check data file
	*/
    int status;
    int i;
    FILE *fp;

    status = lofasm_create_file_Q(IOpar, fileNames, numFiles);

    if(sortFlag == 1)
    	status = sort_file_nodes(&IOpar->fileQhead, &IOpar->fileQend, sortKey);
    
    IOpar->currentFile = IOpar->fileQhead;

    while(IOpar->currentFile != NULL)
  	{
    	status = lofasm_open_file(&IOpar->currentFile->hdr, 
    								IOpar->currentFile->filename, &fp,"r");

    	status = check_raw_file(&IOpar->currentFile -> hdr,fp);
    	printf("startMjd %lf endMjd %lf %lf \n",IOpar->currentFile->hdr.startMJD, 
        IOpar->currentFile -> hdr.endMJD, IOpar->currentFile -> hdr.intgrTime);

    	fclose(fp);
    	IOpar->currentFile= IOpar->currentFile -> nextFile;
  	}

    IOpar->currentFile = IOpar->fileQhead;
    if(IOpar->currentFile == NULL)
    	printf("awesome\n");
	return status;
}


int lofasm_create_file_Q(LoFASMIO *IOpar,char *fileNames[], int numFiles)
{
	/*
		These funtion is to set up 
		Build up the file queue and read the all file header.
	*/
    int status;
    fileNode * currNode = NULL; 
    int i;

    /* Create the head node */
    if(currNode != NULL)
    {
    	fprintf(stderr,"Current node has been allocated.\n");
    	exit(1);
    }

  	currNode = create_file_node(fileNames[0],0);
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
		currNode -> nextFile = create_file_node(fileNames[i],i);
		currNode -> nextFile -> lastFile = currNode;
		/* Move current to next node */
		currNode = currNode -> nextFile;
	}
	IOpar-> fileQend = currNode;
	return status;
}

int sort_file_nodes(fileNode ** Qhead, fileNode **Qend, char *keyWord)
/* Sort the files by the key words. Only using after create the file Q*/
{
	int status;
	fileNode * head = *Qhead;
	fileNode * end =  *Qend;
	fileNode * curr = NULL; 
	fileNode * trail = NULL;
	fileNode * temp = NULL;
	int swapped = 1;
	int numNodes = 0;
	int i,nodeIndex;

	/* We are uing the bubble sort for sorting */
	curr = head;
	while(curr != NULL)
	{	
		numNodes = numNodes + 1;
		curr = curr -> nextFile; 
	}



	if((head == NULL) || (head->nextFile == NULL))
	{
		return;
	}
	/*Sort byt start MJD*/
	if(strcmp(keyWord,"STARTTIME") == 0)
	{
		for(i = 0; i<numNodes; i++)
		{
			curr = trail = head;

			while(curr -> nextFile != NULL)
			{
				if(curr->hdr.startMJD > curr -> nextFile ->hdr.startMJD)
				{
					temp = curr->nextFile;
					curr->nextFile = curr-> nextFile -> nextFile;
					temp -> nextFile = curr;

					if(curr == head)
						head = trail = temp;
					else
						trail -> nextFile = temp;
					curr = temp;
				}
				trail = curr;
				curr = curr -> nextFile;
			}
		}
		*Qhead = head;
		curr = head;
		curr-> lastFile = NULL;
		for(i = 0; i<numNodes-1; i++)
		{
			curr->index = i;
			curr->nextFile->lastFile = curr;
			curr = curr->nextFile;
		}

		end = curr;
		*Qend = end;
	}

	/* Sort by end mjd */
	else if(strcmp(keyWord,"ENDTIME") == 0)
	{
		for(i = 0; i<numNodes; i++)
		{
			curr = trail = head;

			while(curr -> nextFile != NULL)
			{
				if(curr->hdr.endMJD > curr -> nextFile ->hdr.endMJD)
				{
					temp = curr->nextFile;
					curr->nextFile = curr-> nextFile -> nextFile;
					temp -> nextFile = curr;

					if(curr == head)
						head = trail = temp;
					else
						trail -> nextFile = temp;
					curr = temp;
				}
				trail = curr;
				curr = curr -> nextFile;
			}
		}
		*Qhead = head;

		/* Link the queue back wards */
		curr = head;
		curr-> lastFile = NULL;
		for(i = 0; i<numNodes-1; i++)
		{
			curr->index = i;
			curr->nextFile->lastFile = curr;
			curr = curr->nextFile;
		}

		end = curr;
		*Qend = end;
	}

	else
	{
		fprintf(stderr,"Unrecognised key words\n");
		exit(1);
	}
	
	return status;
}

/********* parmeter set up ***********/
/*
Inculuded functions :
	lofasm_set_time()
	lofasm_set_freq()
	lofasm_set_frame()
	lofasm_check_frame_code()
*/
int lofasm_set_time(LoFASMIO *IOpar, double startMjd, double endMjd)
/*
	User select start mjd time and end of the mjd time for the data.
	Without using this function, the time select will be set as default. 
	If the start and end time are the same. Out put one time sample. 
*/
{
	if(startMjd > endMjd)  // The end time is before the start time 
	{
		printf("End Mjd time is before start mjd time. Switch start mjd and " 
				"end mjd.\n");

		IOpar -> startMjdSlct = endMjd;
		IOpar -> endMjdSlct = startMjd;
	}
	else
	{
		IOpar -> startMjdSlct = startMjd;
		IOpar -> endMjdSlct = endMjd;
	}
	return 0;
}


int lofasm_set_freq(LoFASMIO *IOpar, double startFreq, double endFreq)
/*
	User select the frequency band. With out using this function the frequency 
	band will set to default. 
*/
{
	if(startFreq > endFreq) // The end frequency bigger than start frequency
	{
		printf("End frequency is smaller then start frequency. Switch start frequency and " 
				"end frequency.\n");

		IOpar -> startFreqSlct = endFreq;
		IOpar -> endFreqSlct = startFreq;
	}
	else
	{
		IOpar -> startFreqSlct = startFreq;
		IOpar -> endFreqSlct = endFreq;
	}
	return 0;
}


int lofasm_set_frame(LoFASMIO *IOpar, char *spectrumName)
/* Get the spectrum name or beam name and setup */
{
	int i,j;
	int numFrm;
	int frmStrSize=2;
	int status;
	char **frmName;
	size_t lenSpectrumName;

	/* Calculate spectrum name length*/
	lenSpectrumName = len_string(spectrumName);
	printf("%d \n",lenSpectrumName);
	if(lenSpectrumName > MAX_FRAME_NAME_SIZE)
	{
		fprintf(stderr, "Spectrum name %s exceed the maximum frame name size %d"
				"\n",spectrumName, MAX_FRAME_NAME_SIZE);
		exit(1);
	}
	

	/* North and South prolarziation beam*/
	if(strcmp(spectrumName, "NS")==0)
	{
		IOpar-> lenFrmCode = 4;
		
		frmName = 
			(char **)malloc(IOpar->lenFrmCode*sizeof(char *));

		if(!frmName)
		{
			fprintf(stderr, "Memory error when allocate frame name.\n");
			exit(1);
		}

		for(i=0;i<IOpar -> lenFrmCode;i++)
		{
			frmName[i] = (char *)malloc(MAX_FRAME_NAME_SIZE*sizeof(char));
			
			if(!frmName[i])
			{
				fprintf(stderr, "Memory error when allocate frameName[i].\n",i);
				exit(1);
			}
		}
		strcpy(frmName[0],"AA");
		strcpy(frmName[1],"BB");
		strcpy(frmName[2],"CC");
		strcpy(frmName[3],"DD");

	}
	/* West and East prolarziation beam*/
	else if(strcmp(spectrumName,"EW")==0)
	{
		IOpar-> lenFrmCode = 4;
		frmName = 
			(char **)malloc(IOpar->lenFrmCode*sizeof(char *));

		if(!frmName)
		{
			fprintf(stderr, "Memory error when allocate frame name.\n");
			exit(1);
		}

		for(i=0;i<IOpar -> lenFrmCode;i++)
		{
			frmName[i] = (char *)malloc(MAX_FRAME_NAME_SIZE*sizeof(char));
			
			if(!frmName[i])
			{
				fprintf(stderr, "Memory error when allocate frameName[i].\n",i);
				exit(1);
			}
		}
		strcpy(frmName[0],"AA");
		strcpy(frmName[1],"BB");
		strcpy(frmName[2],"CC");
		strcpy(frmName[3],"DD");

	}

	else
	{
		IOpar-> lenFrmCode = 1;
		frmName = 
			(char **)malloc(IOpar->lenFrmCode*sizeof(char *));

		if(!frmName)
		{
			fprintf(stderr, "Memory error when allocate frame name.\n");
			exit(1);
		}

		for(i=0;i<IOpar -> lenFrmCode;i++)
		{
			frmName[i] = (char *)malloc(MAX_FRAME_NAME_SIZE*sizeof(char));
			printf("Address1 %s\n",frmName[i]);
			if(!frmName[i])
			{
				fprintf(stderr, "Memory error when allocate frameName[i].\n",i);
				exit(1);
			}
		}

		strcpy(frmName[0],spectrumName);

	}

	IOpar-> currSpectrum = spectrumName;

	/* Allocate frame code*/
	IOpar->frmCode = 
			(int **)calloc(IOpar->lenFrmCode,sizeof(int *));

	if(!IOpar -> frmCode)
	{
		fprintf(stderr, "Memory error when allocate frame code.\n");
		exit(1);
	}

	for(i=0;i<IOpar -> lenFrmCode;i++)
	{
		IOpar->frmCode[i] = (int *)calloc(3,sizeof(int));
			
		if(!IOpar->frmCode[i])
		{
			fprintf(stderr, "Memory error when allocate framecode[i].\n",i);
			exit(1);
		}

	}

	for(i=0;i<IOpar -> lenFrmCode;i++)
	{
		lofasm_check_frame(IOpar->frmCode[i],frmName[i]);
	}

	for(i=0;i<IOpar -> lenFrmCode;i++)
	{
		printf("%s\n",frmName[i]);

	}

	for(i=0;i<IOpar ->lenFrmCode;i++){
		free(frmName[i]);
		frmName[i] = NULL;
	}	
	free(frmName);
	frmName = NULL;
	return status;
}

/* Check frame name */
int lofasm_check_frame(int frmCode[],char * frmName)
/*
To identify the frame name and return the frame code

Return
Frame code:   
			An int array with 3 elements:
			frmCode[0] : First packet index in raw data
 			frmCode[1] : Second packet index in raw data
			frmCode[2] : Index parity, 0 even 1 odd, 3 no parity in this frame

If frame code is assigned as -1,-1,-1. That means it is an unknown frame name 
The understand the frame packet index please read documenation for Lofasm raw 
data
*/
{	
	int i;
	int temp;
	

	/* Check if frame name is in the right format*/
	
	/* Change lower case to upper case*/
	for(i=0;i<strlen(frmName);i++)
	{
		if(frmName[i]>='a' && frmName[i]<='z')
		{	
			frmName[i] = ('A' + frmName[i] - 'a');
		}
	}
	
	/* Change charactor position if the two charactors is not at right place*/
	if(frmName[1]-frmName[0]<0)
	{
		temp = frmName[0];
		frmName[0] = frmName[1];
		frmName[1] = temp;
	}
	
	/* Compar the input frame name with the standard frame name*/
	if(strcmp(frmName,"AA")==0)
	{
		frmCode[0] = 1;
		frmCode[1] = 9;
		frmCode[2] = 0;
		
	}
	else if(strcmp(frmName,"BB")==0)
	{
		frmCode[0] = 1;
		frmCode[1] = 9;
		frmCode[2] = 1;
	}
	else if(strcmp(frmName,"CC")==0)
	{
		frmCode[0] = 2;
		frmCode[1] = 10;
		frmCode[2] = 0;
	}
	else if(strcmp(frmName,"DD")==0)
	{
		frmCode[0] = 2;
		frmCode[1] = 10;
		frmCode[2] = 1;
	}
	else if(strcmp(frmName,"AB")==0)
	{
		frmCode[0] = 3;
		frmCode[1] = 11;
		frmCode[2] = 3;
	}
	else if(strcmp(frmName,"AC")==0)
	{
		frmCode[0] = 4;
		frmCode[1] = 12;
		frmCode[2] = 3;
	}
	else if(strcmp(frmName,"AD")==0)
	{
		frmCode[0] = 5;
		frmCode[1] = 13;
		frmCode[2] = 3;
	}
	else if(strcmp(frmName,"BC")==0)
	{
		frmCode[0] = 6;
		frmCode[1] = 14;
		frmCode[2] = 3;
	}
	else if(strcmp(frmName,"BD")==0)
	{
		frmCode[0] = 7;
		frmCode[1] = 15;
		frmCode[2] = 3;
	}
	else if(strcmp(frmName,"CD")==0)
	{
		frmCode[0] = 8;
		frmCode[1] = 16;
		frmCode[2] = 3;
	}
	else 
	{
		printf("Unknown frame %s\n",frmName);
		frmCode[0] = -1;
		frmCode[1] = -1;
		frmCode[2] = -1;
	}
	return 0;
}



