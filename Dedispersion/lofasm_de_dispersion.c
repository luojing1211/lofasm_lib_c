/* Dedispersion code for lofasm. 
Copy right : Center for Advanced Radio Astronomy
Author: Jing Luo, Nan Yan, Teviet, Fredrick A. Jenet 
*/

#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include "lofasm_data_IO.h"
#include "lofasm_data_type.h"
#include "lofasm_de_dispersion.h"


int main(int argc, char *argv[])
{
	FILE *fp=NULL;
	LoFASMIO IOpar = {};

	char *filenames[argc-1];
  	char *frameName=NULL; 

  	int numDM;
  	int numFile;


	double *DMarray=NULL;
	double DMstep;

	double *freqArray=NULL;
	double *timeArray=NULL;
	double timeStart, timeEnd;
	double freqStart, freqEnd, freqStep;
	double timeInter;
	double intgrTime;
	double shiftIndex;


	int numTimeBin;
	int numFreqBin;

	dataArray2D *time_DM=NULL;

	dataArray1D *inputData=NULL;
	dataArray1D *normWeight=NULL;
	dataArray1D *timeDelay=NULL;
	dataArray2D *shiftIndexI=NULL;

	int status;
	int i,j,k;

/************************* File Information Check ***************************/
  	/* Build file queue This part will be in the dedispersion set up part */
  	for(i = 1;i<argc;i++)
  	{
    	filenames[i-1] = argv[i];
  	}
  	numFile = argc-1;
  	/* Initial the reading queue */
  	status = lofasm_set_file_read(&IOpar,filenames,numFile, 1, "STARTTIME");
 /********* Set up for parameters *********************************************/
  	/*Change here hard coded*/

  	intgrTime = IOpar.currentFile->hdr.intgrTime;
  	timeStart = IOpar.fileQhead->hdr.startMJD;
  	freqStart = 5.0;
  	freqEnd = 88.0;

  	freqStep = IOpar.currentFile->hdr.freqStep;
  	numFreqBin = (freqEnd - freqStart)/freqStep;

  	status = lofasm_set_freq(&IOpar,freqStart,freqEnd);

  	timeInter = 3*3600.0;   // 3 hours dedisperion data FIXME need to be 
  						   // changed
  	numTimeBin = (int)(timeInter/intgrTime);
  	
  	frameName = "AA";

  	status = lofasm_set_frame(&IOpar,frameName);
/************************* Allocate memorys ********************************/
  	/* Allocate freqency array */
  	
  	freqArray = (double *)malloc(numFreqBin*sizeof(double));

  	if(!freqArray)
  	{
      	fprintf(stderr, "Memory error when allocate the frequency array\n");
      	exit(1);
  	}

  	for(i = 0;i<numFreqBin;i++)
  	{
      	freqArray[i] = freqStart+ i * freqStep;
  	}

  	/* Allocate DMarray */
  	
  	numDM = 10;
  	DMstep = 10.0;
  	DMarray = (double *)malloc(numDM*sizeof(double));

  	if(!DMarray)
  	{
      	fprintf(stderr, "Memory error when allocate the DM array\n");
      	exit(1);
  	}
  
  	for(i = 0;i<numDM;i++)
  	{
      	DMarray[i] = 0+ i * DMstep;
  	}
  	
  	timeArray = (double *)malloc(numTimeBin*sizeof(double));


  	if(!timeArray)
  	{
      	fprintf(stderr, "Memory error when allocate the time axis array\n");
      	exit(1);
  	}

   	for(i = 0;i<numTimeBin;i++)
  	{
      	timeArray[i] = timeStart + i * intgrTime;
  	} 	
	
  	/* Allocate result and input data */
  	time_DM = allocate_2d_array(numTimeBin,numDM,"UNSIGNED_INT");
  	
  	inputData = allocate_1d_array(numFreqBin,"UNSIGNED_INT");

  	normWeight = allocate_1d_array(numTimeBin,"UNSIGNED_INT");

  	timeDelay = allocate_1d_array(numFreqBin,"DOUBLE");
	shiftIndexI = allocate_2d_array(numFreqBin,numDM,"SIGNED_INT");

/*Calculate shiftIndex */

	for(k = 0;k<numDM;k++)
	{
		for(i=0;i<numFreqBin;i++)
		{
			timeDelay->data.dData[i] = -4.15e3*DMarray[k]*(1.0/(freqArray[numFreqBin-1]
				*freqArray[numFreqBin-1])-1.0/(freqArray[i]*freqArray[i]));
	
			shiftIndex = (timeDelay->data.dData[i])/intgrTime;
			shiftIndexI->data.sData[k][i] = (int)shiftIndex;
			//printf("%d ",shiftIndexI->data.sData[k][i]);
		}
		//printf("\n");
	}

	
/* Init reading */
	status = init_raw_reading(&IOpar);

	int readIndex[2];   //FIXME, This need to be in the freqency selection part

	readIndex[0] = (int)((freqStart-IOpar.currentFile->hdr.freqStart)
						/(IOpar.currentFile->hdr.freqStep));

	readIndex[1] = (int)((freqEnd-IOpar.currentFile->hdr.freqStart)
						/(IOpar.currentFile->hdr.freqStep));

	//printf("Index %d %d\n",readIndex[1]-readIndex[0],numFreqBin);
	int intgrIndex=0;
	double currMJD;
	int targetIndex;
	int fltDataIndex;

	status = lofasm_open_file(&IOpar.currentFile->hdr,IOpar.currentFile->filename,
								&fp, "r");
	printf("current file %p\n", IOpar.currentFile);


	while(IOpar.currentFile!=NULL)
	{


		status = read_raw_intgr(&IOpar,fp,intgrIndex);

		 currMJD = IOpar.intgr.MJD;
		 printf("Mjd %lf.\n",currMJD);
		 fltDataIndex = (currMJD - timeStart)/intgrTime;

		if(fltDataIndex >= numTimeBin)
		{
			printf("Data exced the time_DM array\n");
			break;
		}
		/*Read in data */
		for(i=0;i<numFreqBin;i++)
		{
			inputData->data.usData[i] = IOpar.intgr.AAdat[readIndex[0]+i]; 
			//printf("%u ", inputData->data.usData[i]);
		}	
		
		/* Do De-dispersion */
		printf("Do De-dispersion for MJD %lf\n.",currMJD);
		for(k = 0;k<numDM;k++)
		{
			for(i=0;i<numFreqBin;i++)
			{
				targetIndex = fltDataIndex+shiftIndexI->data.sData[k][i];
			
				if(targetIndex >= numTimeBin || inputData->data.usData[i]==0)
				{
					continue;
				}

				time_DM->data.usData[k][targetIndex] = time_DM->data.usData[k][targetIndex]
				  												+inputData->data.usData[i];

				normWeight->data.usData[targetIndex] = 
										normWeight->data.usData[targetIndex]+1; 
			}

			//printf("Normalizing\n");

			for(j = 0;j < numTimeBin;j++)
      		{
        		if(normWeight->data.usData[j]!= 0)
        		{
          			time_DM->data.usData[k][j] = time_DM->data.usData[k][j]
          										/normWeight->data.usData[j];
          		}
          	}

		}


	// 	/* prepare for the next integration */
	 	intgrIndex++;
		
		if(intgrIndex == IOpar.currentFile->hdr.numIntgr)
		{
			printf("Read next file.\n");
			IOpar.currentFile = IOpar.currentFile->nextFile;
			printf("The file now %p.\n",IOpar.currentFile);
			intgrIndex = 0;
			if(IOpar.currentFile!=NULL)
				status = lofasm_open_file(&IOpar.currentFile->hdr,IOpar.currentFile->filename,
									&fp, "r");
		}
		
	}


	FILE *fpw;
	printf("Writing data.\n");

	fpw = fopen("Result_DM_T.dat","w");
	for(i=0;i<numDM;i++)
  	{
      	for(j=0;j<numTimeBin;j++)
      	{
        	fprintf(fpw, "%u ",time_DM->data.usData[i][j]);
      	}
      	fprintf(fpw,"\n");
  	}  

  	
  	free_2d_array(time_DM);
  	free_1d_array(inputData);
	free_1d_array(normWeight);
	free_1d_array(timeDelay);
	free_2d_array(shiftIndexI);

	free(time_DM);
	free(inputData);
	free(normWeight);
	free(timeDelay);
	free(shiftIndexI);
	

	return 0;
}


int dedsps_read_flags(dedispersion_param *dedspsPar, int argc, char *argv[])
/* This program is to read the command line flags */
{
	int status;
	int i;

	for(i = 1;i<argc;i++)
	{
		if(strcmp(argv[i],"-f")==0)
		{
			//while(strcmp(argv[i][0],"-")!=0)
			status = 0;
		}
		else if(strcmp(argv[i],"-p")==0)
		{
			status = 0;
		}
		else
		{
			fprintf(stderr, "Unknown flag %s.\n"
							"Please check your command input\n",argv[i]);
			exit(1);
		}
	}

	return status;

}
