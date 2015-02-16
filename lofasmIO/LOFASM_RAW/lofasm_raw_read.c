/******************************************************************************
Copyright : Center for Advanced Radio Astronomy, CARA, UTB. 

Filename : Source code lofasm_raw_read.c

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
#include "lofasm_data_IO.h"
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define PKT_PER_INTGR 17
#define INIT_INTGR_LIST_SIZE 200


/* Read raw data header */
int read_raw_hdr(hdrInfo *hdr, FILE *filePtr)
/*
To read LoFASM raw data header from one file. 

Input : hdrInfo *hdr, 
        FILE *filePtr

Output: Modified LoFASM header information structure
        int status
            status == 0 :   No error  
            status == 1 :   error
*/
{	
	int status = 0;
	char *buffer;
	size_t result;
	char Temp[20];
	char *buffer3Bytes;			 			// A buffer for 3 bytes 
	unsigned short buffer2Bytes;  			// A buffer for 2 bytes	
	
	// Check if the file is LoFASM raw data
	if(hdr-> fileType == 0)
	{
		fprintf(stderr,
				"Please make sure file %s has been properly.\n",
				hdr -> filename);
		status = 1;
		return status;
	}

	if(hdr-> fileType != 1)
	{
		fprintf(stderr, 
				"File %s is not a LoFASM raw data."
				" Please check your file type at header"
				" structure element fileType\n",hdr->filename);
		status = 1;	
		return status;
	}
	
	// Read header
	rewind(filePtr);   /*Move the file pointer to the beginning of file*/
	// Allocate buffer
	buffer = (char *)malloc (sizeof(char)*8);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (1);}
	
	/*** Read header ***/
	// Read file signature 
	result = fread(buffer,1,8,filePtr);
	hdr-> hdrSig = buffer;
	// Read header version
	result = fread(buffer,1,8,filePtr);
	hdr-> hdrVersion = atoi(buffer);
	
	// Read header lenght
	result = fread(buffer,1,8,filePtr);
	hdr -> hdrLength = atoi(buffer);

	// Read LoFASM station
	result = fread(buffer,1,8,filePtr);
	hdr -> lofasmStation = buffer;

	// Read Number of bins/Channels
	result = fread(buffer,1,8,filePtr);
	hdr -> numBin = atoi(buffer);
	
	// Read start frequency in unit of Mhz
	result = fread(buffer,1,8,filePtr);
	hdr -> freqStart = atof(buffer);

	// Read frequency steps in unit of Mhz
	result = fread(buffer,1,8,filePtr);
	strcpy(Temp,"0.");
	strcat(Temp,buffer);
	hdr -> freqStep = atof(Temp);

	// Read MJD integer part in days
	result = fread(buffer,1,8,filePtr);
	hdr -> mjdDay = atoi(buffer);

	// Read MJD fractional part in millisecond
	result = fread(buffer,1,8,filePtr);
	hdr -> mjdMsec = atoi(buffer);
	
	// Compute the full mjd date
	hdr -> startMJD = (double)hdr->mjdDay + (double)hdr->mjdMsec/SEC_PER_DAY/1000.0; 
	// Read integration time for each integration
	result = fread(buffer,1,8,filePtr);
	hdr -> intgrTime = atof(buffer);

	// Read data format version
	result = fread(buffer,1,8,filePtr);
	hdr -> dataFmtVersion = atoi(buffer);

	// Read notes
	result = fread(buffer,1,8,filePtr);
	hdr -> notes = buffer;

	free(buffer);
		
    status = 0;
    hdr->readFlag = 1;
	return status;
}

/* Check if a packet a header packet*/
int is_hdr_pkt(FILE *filePtr)
/*
	Check if the pointer points to the begin of a packet a header packet. 
	After this function, the pointer will be
	brought to the bigining of the initial point. 
	Input:    FILT *filepointer
	OutPut:   hdrpkt == 1 it is a header packet
			  hdrpkt == 0 it is not a header packet 
*/
{	
	int hdrPkt;
	char *buffer3Bytes;
	int hdrPktSig,hdrPktSig2;   // Two header signatures
	int backCheck;
	size_t result;
	/* First check if the pointer at begining of an integration */
	// Check condition 1. First 3 bytes matchs the signature. 
	//    			   2. After 5 bytes, the signature repeats. 

 	
	buffer3Bytes = (char *)malloc (sizeof(char)*3);	
	// Read first 3 bytes packet signature
	result = fread(buffer3Bytes,3,1,filePtr);
	hdrPktSig = byte3_2_int(buffer3Bytes);
	// Move 5 bytes to the next section
	fseek(filePtr,5,SEEK_CUR);
	// If it is matches packet signature
	result = fread(buffer3Bytes,3,1,filePtr);
	hdrPktSig2 = byte3_2_int(buffer3Bytes);
	
	// Bring pointer back to the early data see if it different
 	fseek(filePtr,-19,SEEK_CUR);
	result = fread(buffer3Bytes,3,1,filePtr);
	backCheck = byte3_2_int(buffer3Bytes);	

	/* Check the header signature if the header signature matches */
	if (hdrPktSig == 14613675 
		&& hdrPktSig == hdrPktSig2
		&& backCheck != hdrPktSig)
	{	
		hdrPkt = 1;
		
	}
	else
	{
		hdrPkt = 0;
	}
	/* Move back to the begining point*/
	fseek(filePtr,5,SEEK_CUR);

	free(buffer3Bytes);
	buffer3Bytes = NULL;
	return hdrPkt;
}



int check_raw_file(hdrInfo *hdr, FILE *filePtr)
/*
To check raw data file status:
	If the number of integration is in the hdr, the number of integration will 
	be uesd. If the number of integration is not in the hdr. It will go through 
	the file to calculated the number of integratoin.  
	Check: 	
		file size
		Number of integrations
		Intgeration ID
		Right format?
		Right header information?
		Bad package?
	This function should be used after reading the file header, and before 
	reading the data. 
*/
{
	int status=0;				 // Returning result. 0 no error, 1 with error. 
	double numPacket;				 // Number of packet in the file	
	int packetCounter = 0;       // packet Counter for eache integration
	int hdrPktSig;               // The signature for the header packet
	short hdrCntr;				 // A counter for header, FIXME. not sure in right value
	int i;						 // Loop counter
	int intgrIndex;             // The index of intgration
	char *buffer3Bytes;			 // A buffer for 3 bytes 
	unsigned short buffer2Bytes; // A buffer for 2 bytes
	size_t result;				 // For reading binary data	
	int badIntgrCounter=0;       // Bad integration counter 
	int lenBadArray;             // length of current bad integration array
	long size;

	printf("Checking raw file %s.\n", hdr -> filename);

	/* Check if the header have been read */
	if(hdr->readFlag != 1)
	{
		fprintf(stderr, "Please read raw file header first. To read file header"
						" use function read_raw_hdr()\n");
		exit(1);
	}

	/* Check file size */
	rewind(filePtr);         // Bring the file pointer to the beginning

	/* Calculate the file size */
	fseek(filePtr, 0L, SEEK_END);    
	hdr->fileSize = ftell(filePtr);
	/* Calculate the packet size */
	hdr->packetSize = hdr->numBin * 4;
				
	/* Bring the pointer back to the end of header */
	fseek(filePtr, hdr->hdrLength, SEEK_SET);

	numPacket = ((double)hdr->fileSize - (double)hdr->hdrLength)/
				(double)hdr->packetSize;
	/* Check if the data has uncompleted packet*/
	if(fmod(numPacket,1.0) != 0.0)
	{
		fprintf(stderr,"This file %s has uncompleted packet\n.", 
				hdr->filename);	
		status = 1;     // FIXME should we use the right error code
		return status;  
	}

	/* Check integration status*/
	/* Each integration contains 17 packets. First packet is the header packet,
	   which contains repeatly 3 bytes header signature, 3 bytes integration 
	   number, 2 bytes header counter
	   1. We have to determine if the packet is header packet.
	   2. We will check the information in header packet
 	   3. The missing packet will be calculated
	*/

	/* The number of integration is known. */
	if(hdr->numIntgr != 0 && hdr -> numIntgrFlag == 1)
	{
		/* Check if the integration number match the file size */
		hdr->intgrList = (fIntgrInfo *)calloc(hdr->numIntgr,sizeof(fIntgrInfo));
		/* Set the packet counter to 17, get ready for reading first packet of the
	 	file */
		packetCounter = PKT_PER_INTGR; // Set initial number 

		// Allocate the memory for the 3 bytes buffer data	
		buffer3Bytes = (char *)malloc (sizeof(char)*3);

		if(!buffer3Bytes)
		{
			fprintf(stderr, "Memeory error when allocating buffer3Bytes in"
							" check_raw_file().\n");
		}

		intgrIndex = 0;
		/* Check all the packets */
		for(i=0;i<numPacket;i++)
		{	
			/* Process the header packet. */
			if(is_hdr_pkt(filePtr))  
			{
				/* Check if the integration index bigger than then the number 
					of the integration */
				if(intgrIndex >= hdr->numIntgr)
				{
					fprintf(stderr, "The number of integration from header"
									" is not match the file\n");
					exit(1);
				}

				// Checking if the last integration has missing packet.
				if(packetCounter != PKT_PER_INTGR)  
				{
					printf("File %s has missing/additional packet at"
							" integration %d and it has %d packets.\n", 
							hdr-> filename,hdr->intgrList[intgrIndex-1],
							packetCounter);
					if(intgrIndex != 0)
						hdr -> intgrList[intgrIndex-1].badIntgrFlag = 1;
				}

				/* Record the integration postion */
				hdr->intgrList[intgrIndex].intgrPos = ftell(filePtr);

				/* Skip 3 bytes of header packet signature */
				fseek(filePtr,3,SEEK_CUR);

				/* Read the integration ID */
				result = fread(buffer3Bytes,3,1,filePtr);
				hdr -> intgrList[intgrIndex].intgrID = byte3_2_int(buffer3Bytes);

				/* Calculate the integration MJD*/
				hdr -> intgrList[intgrIndex].intgrMJD = 
						hdr -> startMJD + intgrIndex * hdr-> intgrTime/SEC_PER_DAY;
				// FIXME, above, is integration index the rghit thing to do. 

				
				packetCounter = 1;
				intgrIndex++;
				/* Bring the pointer to the next packet */
				fseek(filePtr,hdr->packetSize-6,SEEK_CUR);

			}
			else
			{
				packetCounter++;
				fseek(filePtr,hdr->packetSize,SEEK_CUR);
			}

		}
		/*Check if last integration a bad one */
		if(packetCounter != PKT_PER_INTGR)
		{
			printf("File %s has missing/additional packet at"
							" integration %d and it has %d packets.\n", 
							hdr-> filename,hdr->intgrList[intgrIndex-1],
							packetCounter);
			hdr -> intgrList[intgrIndex-1].badIntgrFlag = 1;
		}

	}



	/* The number of integration is not known.
	   The number of integration will be calculated by go thought the file
	   This will use some local lists */
	else
	{
		int intgrListSize;
		int *intgrPosLoc;
		int *intgrIDLoc;
		char *badIntgrLoc;
		int intgrCnt;

		intgrListSize = INIT_INTGR_LIST_SIZE;
		intgrPosLoc = (int *)malloc(sizeof(int)*intgrListSize);
		intgrIDLoc = (int *)malloc(sizeof(int)*intgrListSize);
		badIntgrLoc = (char *)calloc(intgrListSize,sizeof(char));


		if(!intgrPosLoc ||!intgrIDLoc || !badIntgrLoc)
		{
			fprintf(stderr,"Memeory error when allocating local integration" 
					" list.\n");
			exit(1);
		}

		intgrIndex = 0;
		intgrCnt = 0;
		// Set the packet counter to 17, get ready for reading first packet of the
		// file
		packetCounter = PKT_PER_INTGR; // Set the initial number
	
		// Allocate the memory for the 3 bytes buffer data	
		buffer3Bytes = (char *)malloc(sizeof(char)*3);
		if(!buffer3Bytes)
		{
			fprintf(stderr, "Memeory error when allocating buffer3Bytes in"
							" check_raw_file().\n");
		}
	
		/* Check all the packets */
		for(i=0;i<numPacket;i++)
		{	
			
			// This is a header packet.
			if(is_hdr_pkt(filePtr))  
			{	
				intgrCnt++;
				// Checking if the last integration has missing packet.
				if(packetCounter!=PKT_PER_INTGR)  
				{
					printf("File %s has missing/additional packet at"
							" integration %d and it has %d packets.\n", 
						hdr-> filename, intgrIDLoc[intgrIndex-1],
						packetCounter);
					badIntgrLoc[intgrIndex-1] = 1;
				}
			

				// Check if the integration list is full
				if(intgrIndex >= intgrListSize)
				{
					int *Temp = realloc(intgrPosLoc,2*intgrListSize);
					if(!Temp)
					{
						fprintf(stderr,"Memeory error in reallocate integration" 
										"Position Local array");
						exit(1);
					}
					intgrPosLoc = Temp;
					

					int *Temp2 = realloc(intgrIDLoc,2*intgrListSize);
					if(!Temp2)
					{
						fprintf(stderr,"Memeory error in reallocate integration" 
										" ID Local array");
						exit(1);
					}
					intgrIDLoc = Temp2;

					char *Temp3 = realloc(badIntgrLoc,2*intgrListSize);
					if(!Temp3)
					{
						fprintf(stderr,"Memeory error in reallocate bad"
										 "integration array");
						exit(1);
					}
					badIntgrLoc = Temp3;

					/* Change the size recorder to 2 */
					intgrListSize = 2*intgrListSize;
				}

				intgrPosLoc[intgrIndex] = ftell(filePtr);
				
				/* Skip the header packet signature */
				fseek(filePtr,3,SEEK_CUR);

				/* Get the integration ID*/
				result = fread(buffer3Bytes,3,1,filePtr);
				intgrIDLoc[intgrIndex] = byte3_2_int(buffer3Bytes);

				intgrIndex++;

				/* Reset the packet counter to 1 */
				packetCounter = 1;


				fseek(filePtr,hdr->packetSize-6,SEEK_CUR);

			}


			// If it is not header packet, count one more packet and go to next. 	
			else
			{
				packetCounter++;
				fseek(filePtr,hdr->packetSize,SEEK_CUR);
			}
		}

		if(packetCounter != PKT_PER_INTGR)
		{
			printf("File %s has missing/additional packet at"
							" integration %d and it has %d packets.\n", 
							hdr-> filename,intgrIDLoc[intgrIndex-1],
							packetCounter);
			badIntgrLoc[intgrIndex-1]= 1;
		}

		hdr->numIntgr = intgrCnt;

		if(hdr->numIntgr<=1)
		{
			fprintf(stderr,"Not enough data in file %s. Please check your data"
						   " file\n", hdr->filename);
			status = 1; //FIXME error code
			return status;
		}

		/* Allocate memory for integration list */
		hdr->intgrList = (fIntgrInfo *)calloc(hdr->numIntgr,sizeof(fIntgrInfo));

		for(i = 0; i < hdr->numIntgr; i++)
		{
			hdr -> intgrList[i].intgrID = intgrIDLoc[i];
			hdr -> intgrList[i].intgrPos = intgrPosLoc[i];
			hdr -> intgrList[i].intgrMJD = 
					hdr -> startMJD + (hdr -> intgrList[i].intgrID 
						- hdr -> intgrList[0].intgrID) * hdr-> intgrTime/SEC_PER_DAY;

			hdr -> intgrList[i].badIntgrFlag = badIntgrLoc[i];
		}
		// Free local memory
		free(intgrPosLoc);
		free(intgrIDLoc);
		free(badIntgrLoc);
	}
	
	// The last integrtion mjd is the end mjd of the file. 	
	hdr->endMJD = hdr->intgrList[hdr -> numIntgr-1].intgrMJD;

	
	printf("SUMMARY: File %s contains %d Intgrations\n",
			hdr->filename,hdr->numIntgr);

	hdr-> fileCheckFlag = 1;
	/*Bring the pointer to the begining of the file*/	
	fseek(filePtr,0L,SEEK_SET); //FIXME should this be here
	free(buffer3Bytes); 

	return status;
}

/* initialize data memory space */
 int init_raw_reading(LoFASMIO *IOpar)
 {

 	int status = 0;
 	int i,j;
 	int sumFrmCode;
 	//FIXME should we check if the header has read?
 	hdrInfo *hdr = &IOpar->fileQhead->hdr;
 	
 	dataIntgr *intgr = &IOpar->intgr;
 	intgr -> numChannel = hdr->numBin;
 	// Allocate the memory for the unorganised raw data block
 	/* malloc the memory for unsigned data part 1 */	
 	if(intgr -> unsgnData1 ==NULL)	
 	{
		
 		intgr -> unsgnNum = hdr -> numBin * 2;  //Caculate the number of udata
			
 		intgr -> unsgnData1 = (unsigned int *)malloc(sizeof(unsigned int)*
 							intgr -> unsgnNum); 
 		if(!intgr -> unsgnData1)
 		{	
 			fprintf(stderr,"Memory error when allocating unsgnData1\n");
 			exit(1);
 		}
		
 	}
 	/* malloc the memory for unsigned data part 2 */	
 	if(intgr -> unsgnData2 ==NULL)	
 	{
		
 		intgr -> unsgnNum = hdr -> numBin* 2;  //Caculate the number of udata	
 		intgr -> unsgnData2 = (unsigned int *)malloc(sizeof(unsigned int)*
 							intgr -> unsgnNum); 
 		if(!intgr -> unsgnData2)
 		{	
 			fprintf(stderr,"Memory error when allocating unsgnData2\n");
 			exit(1);
 		}
 	}
 	/* malloc the memory for signed data part 1*/
 	if(intgr -> sgnData1 ==NULL)
 	{
 		intgr -> sgnNum = hdr ->numBin * 6;  //Caculate the number of idata
 		/* malloc the memory for signed data */
 		intgr -> sgnData1 = (signed int *)malloc(sizeof(signed int)*
 							intgr -> sgnNum);
 		if(!intgr -> sgnData1)
 		{	
 			fprintf(stderr,"Memory error when allocating sgnData1\n");
 			exit(1);
 		}
 	}

 	/* malloc the memory for signed data part 2*/
 	if(intgr -> sgnData2 ==NULL)
 	{
 		intgr -> sgnNum = hdr -> numBin * 6;  //Caculate the number of idata
 		intgr -> sgnData2 = (signed int *)malloc(sizeof(signed int)*
 							intgr -> sgnNum);
 		if(!intgr -> sgnData2)
 		{	
 			fprintf(stderr,"Memory error when allocating sgnData2\n");
 			exit(1);
 		}
 	}



 	/*Depend on the frame name calloc the corresponding frame*/
 	// check frame and than check if the memory has been allocated. 	
 	// Calloc the right one
 	for(i=0;i<IOpar->lenFrmCode;i++)   /*loop over all the possible frame in the list*/
 	{	
 		sumFrmCode = IOpar->frmCode[i][0]+IOpar->frmCode[i][1]
 						+IOpar->frmCode[i][2];
		
 		/* Allocate the memory for AA data*/
 		if(sumFrmCode == 10 && intgr->AAdat==NULL)
 		{
 			intgr->AAdat = (unsigned int *)calloc(intgr->
							numChannel,sizeof(unsigned int));
			if(!intgr -> AAdat)
			{	
				fprintf(stderr,"Memory error when allocating AAdat\n");
				exit(1);
			}
		}
		/* Allocate the memory for BB data*/
		else if(sumFrmCode == 11 && intgr->BBdat==NULL)
		{
			intgr->BBdat = (unsigned int *)calloc(intgr->
							numChannel,sizeof(unsigned int));
			if(!intgr -> BBdat)
			{	
				fprintf(stderr,"Memory error when allocating BBdat\n");
				exit(1);
			}
		}
		/* Allocate the memory for CC data*/
		else if(sumFrmCode == 12 && intgr->CCdat==NULL)
		{
			intgr->CCdat = (unsigned int *)calloc(intgr->
							numChannel,sizeof(unsigned int));
			if(!intgr -> CCdat)
			{	
				fprintf(stderr,"Memory error when allocating CCdat\n");
				exit(1);
			}
		}
		/* Allocate the memory for DD data */
		else if(sumFrmCode == 13 && intgr->DDdat==NULL)
		{
			intgr->DDdat = (unsigned int *)calloc(intgr->
							numChannel,sizeof(unsigned int));
			if(!intgr -> DDdat)
			{	
				fprintf(stderr,"Memory error when allocating DDdat\n");
				exit(1);
			}
		}

		/* Allocate the memory for AB data*/
		else if(sumFrmCode==17 && intgr->ABdatRl==NULL && intgr->ABdatIm==NULL)
		{
			intgr->ABdatRl = (signed int *)calloc(intgr->
							numChannel,sizeof(signed int));
			intgr->ABdatIm = (signed int *)calloc(intgr->
							numChannel,sizeof(signed int));
			if(!intgr -> ABdatRl || !intgr->ABdatIm)
			{	
				fprintf(stderr,"Memory error when allocating ABdat\n");
				exit(1);
			}
		}
		/* Allocate the memory for AC data*/
		else if(sumFrmCode==19 && intgr->ACdatRl==NULL && intgr->ACdatIm==NULL)
		{
			intgr->ACdatRl = (signed int *)calloc(intgr->
							numChannel,sizeof(signed int));
			intgr->ACdatIm = (signed int *)calloc(intgr->
							numChannel,sizeof(signed int));
			if(!intgr -> ACdatRl || !intgr->ACdatIm)
			{	
				fprintf(stderr,"Memory error when allocating ACdat\n");
				exit(1);
			}
		}
		/* Allocate the memory for AD data*/
		else if(sumFrmCode==21 && intgr->ADdatRl==NULL && intgr->ADdatIm==NULL)
		{
			intgr->ADdatRl = (signed int *)calloc(intgr->
							numChannel,sizeof(signed int));
			intgr->ADdatIm = (signed int *)calloc(intgr->
							numChannel,sizeof(signed int));
			if(!intgr -> ADdatRl || !intgr->ADdatIm)
			{	
				fprintf(stderr,"Memory error when allocating ADdat\n");
				exit(1);
			}
		}

		/* Allocate the memory for BC data*/
		else if(sumFrmCode==23 && intgr->BCdatRl==NULL && intgr->BCdatIm==NULL)
		{
			intgr->BCdatRl = (signed int *)calloc(intgr->
							numChannel,sizeof(signed int));
			intgr->BCdatIm = (signed int *)calloc(intgr->
							numChannel,sizeof(signed int));
			if(!intgr -> BCdatRl || !intgr->BCdatIm)
			{	
				fprintf(stderr,"Memory error when allocating BCdat\n");
				exit(1);
			}
		}
		/* Allocate the memory for BD data*/
		else if(sumFrmCode==25 && intgr->BDdatRl==NULL && intgr->BDdatIm==NULL)
		{
			intgr->BDdatRl = (signed int *)calloc(intgr->
							numChannel,sizeof(signed int));
			intgr->BDdatIm = (signed int *)calloc(intgr->
							numChannel,sizeof(signed int));
			if(!intgr -> BDdatRl || !intgr->BDdatIm)
			{	
				fprintf(stderr,"Memory error when allocating BDdat\n");
				exit(1);
			}
		}
		/* Allocate the memory for CD data*/
		else if(sumFrmCode==27 && intgr->CDdatRl==NULL && intgr->CDdatIm == NULL)
		{
			intgr->CDdatRl = (signed int *)calloc(intgr->
							numChannel,sizeof(signed int));
			intgr->CDdatIm = (signed int *)calloc(intgr->
							numChannel,sizeof(signed int));
			if(!intgr -> CDdatRl || !intgr->CDdatIm)
			{	
				fprintf(stderr,"Memory error when allocating CDdat\n");
				exit(1);
			}
		}
		else
		{
			printf("No frame need to be read.\n");
		}
	}

	intgr->initFlag = 1;
	return status; 
		
}


int read_raw_intgr(LoFASMIO *IOpar, FILE *filePtr, int intgrIndex)
/* This have to be used after the init_raw_reading function */
{
	int status;
	int i,j;
	int index,offset;
	int sumFrmCode;
	long ptrPosition;
	hdrInfo *hdr = &IOpar->currentFile->hdr;
 	dataIntgr *intgr = &IOpar->intgr;


 	/* Check if data container has been allocated. */
 	if(intgr->initFlag != 1)
 	{
 		fprintf(stderr, "Please initialize raw reading first.\n");
 		status = 1;
 		exit(1);
 	}

 	intgr->MJD = hdr -> intgrList[intgrIndex].intgrMJD;

 	/*Move the file pointer the right place */
 	ptrPosition = ftell(filePtr);
	if(ptrPosition != hdr -> intgrList[intgrIndex].intgrPos)
	{
		fprintf(stderr, "The pointer is not pointing to the selected" 
						" integration. Move file pointer to the selected"
						" integration.\n");
		fseek(filePtr,hdr -> intgrList[intgrIndex].intgrPos,SEEK_SET);
	}
	

	/*  Check if the pointer at the begining of integration*/
	if(1!= is_hdr_pkt(filePtr))
	{
		fprintf(stderr,"The pointer is not at the begining of the"
					" integration. Please check your integraion position.\n");
		status = 1;
		exit(1);
	}

  	/* Bad integration not reading any data. Since the spectrum is already in 
	calloc to be zero */
	if(hdr -> intgrList[intgrIndex].badIntgrFlag == 1)
	{	
		status = 1; 
		for(i = 0;i<intgr->numChannel;i++)
		{	
			if(intgr->AAdat!= NULL)
			{
				intgr->AAdat[i] = 0;
			}

			if(intgr->BBdat!= NULL)
			{
				intgr->BBdat[i] = 0;
			}
			if(intgr->CCdat!= NULL)
			{
				intgr->CCdat[i] = 0;
			}
			if(intgr->DDdat!= NULL)
			{
				intgr->DDdat[i] = 0;
			}
			if(intgr->ABdatRl!= NULL)
			{
				intgr->ABdatRl[i] = 0;
			}
			if(intgr->ACdatRl!= NULL)
			{
				intgr->ACdatRl[i] = 0;
			}
			if(intgr->ADdatRl!= NULL)
			{
				intgr->ADdatRl[i] = 0;
			}
			if(intgr->BCdatRl!= NULL)
			{
				intgr->BCdatRl[i] = 0;
			}
			if(intgr->BDdatRl!= NULL)
			{
				intgr->BDdatRl[i] = 0;
			}
			if(intgr->CDdatRl!= NULL)
			{
				intgr->CDdatRl[i] = 0;
			}
			if(intgr->ABdatIm!= NULL)
			{
				intgr->ABdatIm[i] = 0;
			}
			if(intgr->ACdatIm!= NULL)
			{
				intgr->ACdatIm[i] = 0;
			}
			if(intgr->ADdatIm!= NULL)
			{
				intgr->ADdatIm[i] = 0;
			}
			if(intgr->BCdatIm!= NULL)
			{
				intgr->BCdatIm[i] = 0;
			}
			if(intgr->BDdatIm!= NULL)
			{
				intgr->BDdatIm[i] = 0;
			}
			if(intgr->CDdatIm!= NULL)
			{
				intgr->CDdatIm[i] = 0;
			}
		}
		fseek(filePtr,hdr->intgrList[intgrIndex+1].intgrPos,SEEK_SET);   
		return status;
	}
	
	// Skip the header packet
	fseek(filePtr,hdr->packetSize,SEEK_CUR);

	// Read the unsigned data part 1
	fread(intgr->unsgnData1,sizeof(unsigned int),intgr->unsgnNum,filePtr);
	
	// Read the signed data part 1
	fread(intgr->sgnData1,sizeof(signed int),intgr->sgnNum,filePtr);
	
	// Read the unsigned data part 2
	fread(intgr->unsgnData2,sizeof(unsigned int),intgr->unsgnNum,filePtr);
	
	// Read the signed data part 2
	fread(intgr->sgnData2,sizeof(signed int),intgr->sgnNum,filePtr);
	
	/*Construct the frames depend on the frame code*/
	for(i=0;i<IOpar->lenFrmCode;i++)
	{

		sumFrmCode = IOpar->frmCode[i][0]+IOpar->frmCode[i][1]
						+IOpar->frmCode[i][2];
		/* Read Frame AA if AA is there*/		
		if(sumFrmCode == 10)
		{
			if(intgr->AAdat==NULL)
			{
				fprintf(stderr,"Please allocate the memory for AAdat.\n");
				exit(1);
			}

			for(j=0;j<intgr->numChannel/2;j++)
			{	
				// Read from the first data packet for AA

				index = 2*j+IOpar -> frmCode[i][2];
				offset = IOpar ->frmCode[i][0]-1;
				intgr->AAdat[2*j] = intgr->unsgnData1[index+offset];
				
				swap4((char *)&intgr->AAdat[2*j]);

				 // Read from the second data packet for AA
				offset = IOpar ->frmCode[i][1]-9;
				intgr->AAdat[2*j+1] = intgr->unsgnData2[index+offset];
				swap4((char *)&intgr->AAdat[2*j+1]);
			}
		}

		/*Read Frame BB if BB is there*/
		else if(sumFrmCode == 11 )
		{
			if(intgr->BBdat==NULL)
			{
				fprintf(stderr,"Please allocate the memory for BBdat.\n");
				exit(1);
			}
			for(j=0;j<intgr->numChannel/2;j++)
			{	
				// Read from the first data packet for BB
				index = 2*j+IOpar -> frmCode[i][2];
				offset = IOpar ->frmCode[i][0]-1;
				intgr->BBdat[2*j] = intgr->unsgnData1[index+offset];
				swap4((char *)&intgr->BBdat[2*j]);

				 // Read from the second data packet for BB
				offset = IOpar ->frmCode[i][1]-9;
				intgr->BBdat[2*j+1] = intgr->unsgnData2[index+offset];
				swap4((char *)&intgr->BBdat[2*j+1]);
			}
		}
		/*Read Frame CC if CC is there*/
		else if(sumFrmCode == 12)
		{
			if(intgr->CCdat==NULL)
			{
				fprintf(stderr,"Please allocate the memory for CCdat.\n");
				exit(1);
			}
			for(j=0;j<intgr->numChannel/2;j++)
			{	
				// Read from the first data packet for CC
				index = 2*j+IOpar -> frmCode[i][2];
				offset = IOpar ->frmCode[i][0]-1;
				intgr->CCdat[2*j] = intgr->unsgnData1[index+offset];
				swap4((char *)&intgr->CCdat[2*j]);

				 // Read from the second data packet for CC
				offset = IOpar ->frmCode[i][1]-9;
				intgr->CCdat[2*j+1] = intgr->unsgnData2[index+offset];
				swap4((char *)&intgr->CCdat[2*j+1]);
			}
		}
		/*Read Frame DD if DD is there*/
		else if(sumFrmCode == 13)
		{
			if(intgr->DDdat==NULL)
			{
				fprintf(stderr,"Please allocate the memory for DDdat.\n");
				exit(1);
			}
			for(j=0;j<intgr->numChannel/2;j++)
			{	
				// Read from the first data packet for DD
				index = 2*j+IOpar -> frmCode[i][2];
				offset = IOpar ->frmCode[i][0]-1;
				intgr->DDdat[2*j] = intgr->unsgnData1[index+offset];
				swap4((char *)&intgr->DDdat[2*j]);

				 // Read from the second data packet for DD
				offset = IOpar ->frmCode[i][1]-9;
				intgr->DDdat[2*j+1] = intgr->unsgnData2[index+offset];
				swap4((char *)&intgr->DDdat[2*j+1]);
			}
		}
		/*Read Frame AB if AB is there*/
		else if(sumFrmCode==17)
		{
			if( intgr->ABdatRl==NULL || intgr->ABdatIm==NULL)
			{
				fprintf(stderr,"Please allocate the memory for ABdat.\n");
				exit(1);
			}
			for(j=0;j<intgr->numChannel/2;j++)
			{	
				
				index = 2*j;
				// Read from the first data packet for AB
				offset = IOpar ->frmCode[i][0]-3;
				intgr->ABdatRl[2*j] = intgr->sgnData1[index+offset];
				swap4((char *)&intgr->ABdatRl[2*j]);

				intgr->ABdatIm[2*j] = intgr->sgnData1[index+offset+1];
				swap4((char *)&intgr->ABdatIm[2*j]);
				 
				// Read from the second data packet for AB
				offset = IOpar ->frmCode[i][1]-11;
				intgr->ABdatRl[2*j+1] = intgr->sgnData2[index+offset];
				swap4((char *)&intgr->ABdatRl[2*j+1]);
				
				intgr->ABdatIm[2*j+1] = intgr->sgnData2[index+offset+1];
				swap4((char *)&intgr->ABdatIm[2*j+1]);
			}
		}
		/*Read Frame AC if AC is there*/
		else if(sumFrmCode==19)
		{
			if( intgr->ACdatRl==NULL || intgr->ACdatIm==NULL)
			{
				fprintf(stderr,"Please allocate the memory for ACdat.\n");
				exit(1);
			}
			for(j=0;j<intgr->numChannel/2;j++)
			{	
				
				index = 2*j;
				// Read from the first data packet for AC
				offset = IOpar->frmCode[i][0]-3;
				intgr->ACdatRl[2*j] = intgr->sgnData1[index+offset];
				swap4((char *)&intgr->ACdatRl[2*j]);

				intgr->ACdatIm[2*j] = intgr->sgnData1[index+offset+1];
				swap4((char *)&intgr->ACdatIm[2*j]);
				 
				// Read from the second data packet for AC
				offset = IOpar ->frmCode[i][1]-11;
				intgr->ACdatRl[2*j+1] = intgr->sgnData2[index+offset];
				swap4((char *)&intgr->ACdatRl[2*j+1]);
				
				intgr->ACdatIm[2*j+1] = intgr->sgnData2[index+offset+1];
				swap4((char *)&intgr->ACdatIm[2*j+1]);
			}
		}
		/*Read Frame AD if AD is there*/
		else if(sumFrmCode==21)
		{
			if( intgr->ADdatRl==NULL || intgr->ADdatIm==NULL)
			{
				fprintf(stderr,"Please allocate the memory for ADdat.\n");
				exit(1);
			}
			for(j=0;j<intgr->numChannel/2;j++)
			{	
				index = 2*j;
				// Read from the first data packet for AD
				offset = IOpar ->frmCode[i][0]-3;
				intgr->ADdatRl[2*j] = intgr->sgnData1[index+offset];
				swap4((char *)&intgr->ADdatRl[2*j]);

				intgr->ADdatIm[2*j] = intgr->sgnData1[index+offset+1];
				swap4((char *)&intgr->ADdatIm[2*j]);
				
				// Read from the second data packet for AD 
				offset = IOpar ->frmCode[i][1]-11;
				intgr->ADdatRl[2*j+1] = intgr->sgnData2[index+offset];
				swap4((char *)&intgr->ADdatRl[2*j+1]);
				
				intgr->ADdatIm[2*j+1] = intgr->sgnData2[index+offset+1];
				swap4((char *)&intgr->ADdatIm[2*j+1]);
			}
		}
		/*Read Frame BC if BC is there*/
		else if(sumFrmCode==23)
		{
			if( intgr->BCdatRl==NULL || intgr->BCdatIm==NULL)
			{
				fprintf(stderr,"Please allocate the memory for BCdat.\n");
				exit(1);
			}
			for(j=0;j<intgr->numChannel/2;j++)
			{	

				index = 2*j;
				// Read from the first data packet for BC
				offset = IOpar ->frmCode[i][0]-3;
				intgr->BCdatRl[2*j] = intgr->sgnData1[index+offset];
				swap4((char *)&intgr->BCdatRl[2*j]);

				intgr->BCdatIm[2*j] = intgr->sgnData1[index+offset+1];
				swap4((char *)&intgr->BCdatIm[2*j]);
				 
				// Read from the second data packet for BC
				offset = IOpar ->frmCode[i][1]-11;
				intgr->BCdatRl[2*j+1] = intgr->sgnData2[index+offset];
				swap4((char *)&intgr->BCdatRl[2*j+1]);
				
				intgr->BCdatIm[2*j+1] = intgr->sgnData2[index+offset+1];
				swap4((char *)&intgr->BCdatIm[2*j+1]);
			}
		}

		/*Read Frame BD if BD is there*/
		else if(sumFrmCode==25)
		{
			if( intgr->BDdatRl==NULL || intgr->BDdatIm==NULL)
			{
				fprintf(stderr,"Please allocate the memory for BDdat.\n");
				exit(1);
			}
			for(j=0;j<intgr->numChannel/2;j++)
			{	

				index = 2*j;
				// Read from the first data packet for BD
				offset = IOpar ->frmCode[i][0]-3;
				intgr->BDdatRl[2*j] = intgr->sgnData1[index+offset];
				swap4((char *)&intgr->BDdatRl[2*j]);

				intgr->BDdatIm[2*j] = intgr->sgnData1[index+offset+1];
				swap4((char *)&intgr->BDdatIm[2*j]);
				
				// Read from the second data packet for BD 
				offset = IOpar ->frmCode[i][1]-11;
				intgr->BDdatRl[2*j+1] = intgr->sgnData2[index+offset];
				swap4((char *)&intgr->BDdatRl[2*j+1]);
				
				intgr->BDdatIm[2*j+1] = intgr->sgnData2[index+offset+1];
				swap4((char *)&intgr->BDdatIm[2*j+1]);
			}
		}
		/*Read Frame CD if CD is there*/
		else if(sumFrmCode==27)
		{
			if( intgr->CDdatRl==NULL || intgr->CDdatIm==NULL)
			{
				fprintf(stderr,"Please allocate the memory for CDdat.\n");
				exit(1);
			}
			for(j=0;j<intgr->numChannel/2;j++)
			{	

				index = 2*j;
				// Read from the first data packet for CD
				offset = IOpar ->frmCode[i][0]-3;
				intgr->CDdatRl[2*j] = intgr->sgnData1[index+offset];
				swap4((char *)&intgr->CDdatRl[2*j]);

				intgr->CDdatIm[2*j] = intgr->sgnData1[index+offset+1];
				swap4((char *)&intgr->CDdatIm[2*j]);
				 
				// Read from the second data packet for CD
				offset = IOpar->frmCode[i][1]-11;
				intgr->CDdatRl[2*j+1] = intgr->sgnData2[index+offset];
				swap4((char *)&intgr->CDdatRl[2*j+1]);
				
				intgr->CDdatIm[2*j+1] = intgr->sgnData2[index+offset+1];
				swap4((char *)&intgr->CDdatIm[2*j+1]);
			}
		}
	}

	return status;
}

