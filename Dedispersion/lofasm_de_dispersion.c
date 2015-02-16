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
	dedispersion_param dedspsPar = {};

	char *filenames[argc-1];
  	char *frameName=NULL; 

  	int numDM;
  	int numFile;


	double *DMarray=NULL;
	double DMstep;
	double DMstart;

	double *freqArray=NULL;
	double *timeArray=NULL;
	double timeStart, timeEnd;
	double freqStart, freqEnd, freqStep;
	double timeInter;
	double intgrTime;
	double shiftIndex;


	int numTimeBin;
	int numFreqBin;
	int maxShift;

	dataArray2D *time_DM=NULL;

	dataArray1D *inputData=NULL;
	unsigned int *normWeight;
	dataArray1D *timeDelay=NULL;
	dataArray2D *shiftIndexI=NULL;

	int status;
	int i,j,k;

	status = dedsps_read_flags(&dedspsPar,argc,argv);
	printf("%d\n",dedspsPar.numDataFile);
	/***** test *****/
	for(i = 0; i<dedspsPar.numDataFile;i++)
	{
		printf("File name %s\n",dedspsPar.dataFileNames[i]);
	}

	printf("par file name %s\n",dedspsPar.paramFile);


	status = dedsps_read_parameter_file(&dedspsPar, dedspsPar.paramFile);
	
/************************* File Information Check ***************************/
  	/* Initial the reading queue */
  	status = lofasm_set_file_read(&IOpar,dedspsPar.dataFileNames,
  								dedspsPar.numDataFile, 1, "STARTTIME");


 /********* Set up for parameters *********************************************/
  	/*Change here hard coded*/
  	intgrTime = IOpar.currentFile->hdr.intgrTime;
  	timeStart = IOpar.fileQhead->hdr.startMJD;
  	freqStart = dedspsPar.freqStart;
  	freqEnd = dedspsPar.freqEnd;

  	freqStep = IOpar.currentFile->hdr.freqStep;
  	numFreqBin = (freqEnd - freqStart)/freqStep;

  	status = lofasm_set_freq(&IOpar,freqStart,freqEnd);

  	timeInter = dedspsPar.timeIntrv * 3600.0;   
  	
  	numTimeBin = (int)(timeInter/intgrTime);

  	numDM = dedspsPar.numDM;
  	DMstep = dedspsPar.DMstep;
  	DMstart = dedspsPar.DMstart;


  	printf("frame name %s\n",dedspsPar.spectrumName);

  	status = lofasm_set_frame(&IOpar,dedspsPar.spectrumName);


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
  	
  
  	DMarray = (double *)malloc(numDM*sizeof(double));

  	if(!DMarray)
  	{
      	fprintf(stderr, "Memory error when allocate the DM array\n");
      	exit(1);
  	}
  
  	for(i = 0;i<numDM;i++)
  	{
      	DMarray[i] = DMstart + i * DMstep;
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

  	//normWeight = allocate_1d_array(numTimeBin,"UNSIGNED_INT");

  	normWeight = (unsigned int *)malloc(numTimeBin*sizeof(unsigned int));


  	timeDelay = allocate_1d_array(numFreqBin,"DOUBLE");
	shiftIndexI = allocate_2d_array(numFreqBin,numDM,"SIGNED_INT");

/*Calculate shiftIndex */

	for(k = 0;k<numDM;k++)
	{
		for(i=0;i<numFreqBin;i++)
		{
			timeDelay->data.dData[i] = -4.15e3*DMarray[k]*(1.0/(freqArray[i]*freqArray[i])
								-1.0/(freqArray[0]*freqArray[0]));
	
			shiftIndex = (timeDelay->data.dData[i])/intgrTime;
			shiftIndexI->data.sData[k][i] = (int)shiftIndex;
		}
	}

	maxShift = shiftIndexI->data.sData[0][0];
	printf("maxShift %d\n",maxShift);
	
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
	int freqCutIndex;

	status = lofasm_open_file(&IOpar.currentFile->hdr,IOpar.currentFile->filename,
								&fp, "r");
	printf("current file %p\n", IOpar.currentFile);


	while(IOpar.currentFile!=NULL)
	{


		status = read_raw_intgr(&IOpar,fp,intgrIndex);

		currMJD = IOpar.intgr.MJD;
		//printf("Mjd %lf.\n",currMJD);
		fltDataIndex = (currMJD - timeStart)*SEC_PER_DAY/intgrTime;

		if(fltDataIndex >= numTimeBin)
		{//FIXME. Need to load more DMtIME ARRAY
			printf("Data exced the time_DM array\n");
			break;
		}
		/*
		if(fltDataIndex + shiftIndexI->data.sData[numDM-1][numFreqBin-1])
		{

		}
		*/
		/*Read in data */
		for(i=0;i<numFreqBin;i++)
		{
			inputData->data.usData[i] = IOpar.intgr.AAdat[readIndex[0]+i]; 
		}	
		
		/* Do De-dispersion */
		printf("Do De-dispersion for MJD %lf index %d\n.",currMJD,fltDataIndex);
		
		for(k = 0;k<numDM;k++)
		{
			for(i=0;i<numFreqBin;i++)
			{
				targetIndex = fltDataIndex+shiftIndexI->data.sData[k][i];
				//targetIndex[1] = fltDataIndex+shiftIndexI->data.sData[k][i+1];
				//printf("Index diffe %d freq1 %lf freq2 %lf\n",targetIndex[1]-targetIndex[0],freqArray[i],freqArray[i+1]);
				if(targetIndex >= numTimeBin || inputData->data.usData[i]==0)
				{
					continue;
				}
				
				time_DM->data.usData[k][targetIndex = time_DM->data.usData[k][targetIndex[0]]+inputData->data.usData[i];
				//
				normWeight[targetIndex] = 
										normWeight[targetIndex]+1; 
			}
			
			for(j = 0;j < numTimeBin;j++)
      		{
        		if(normWeight[j]!= 0)
        		{
          			time_DM->data.usData[k][j] = time_DM->data.usData[k][j]
          										/normWeight[j];
          			normWeight[j]=0;
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
	//free_1d_array(normWeight);
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
	int j;

	char datafFlag = 0;
	char parfFlag = 0;

	for(i = 1;i<argc;i++)
	{
		if(strcmp(argv[i],"--f")==0)
		{
			j = 0;
			printf("argc %d.\n",argc);

			while(i+j+1<argc && strncmp(argv[i+j+1],"--",2)!=0)
			{
			
				if(j >= MAX_NUM_FILE)
				{
					fprintf(stderr, "Too many data files. If you want to have"
									" more files please change <MAX_NUM_FILE>"
									" in lofasm_de_dispersion.h.\n");
					exit(1);
					status = 1;
				}
				dedspsPar->dataFileNames[j] = argv[i+j+1];
				j++;
			}
			
			dedspsPar -> numDataFile = j;

			i = i+j;

			datafFlag = 1;
		}
		else if(strcmp(argv[i],"--p")==0)
		{
			j = 0;
			while(i+j+1<argc && strncmp(argv[i+j+1],"--",2)!=0)
			{
				if(j >= 1)
				{
					fprintf(stderr, "Too many parameter files. Only one"
									" parameter file is acceptable\n");
					exit(1);
					status = 1;
				}
				sprintf(dedspsPar->paramFile,argv[i+j+1]);

				j++;
			}

			i = i+j;
			parfFlag = 1;
		}
		else
		{
			fprintf(stderr, "Unknown flag %s.\n"
							"Please check your command input\n",argv[i]);
			exit(1);
			status = 1;
		}
	}

	if(parfFlag == 0 || datafFlag ==0)
	{
		fprintf(stderr, "Input flag: Please check you command flag. [--f] data"
						" file flag and [--p] parameter file flag are required."
						"\n");
		exit(1);
	}
	return status;

}



int dedsps_read_parameter_file(dedispersion_param *dedspsPar, char filename[])
/* Read dedispertion parameter file*/
{
	char str[1024];
	int status=0;
	char nline;
	FILE *fp;
	

	fp = fopen(filename,"r");

	if(fp!=NULL)
	{
		    /* Read lines */
    	while (!feof(fp))
    	{
        	nline = fscanf(fp,"%s",str);
        	if(nline==1)
        	{    
            	dedsps_check_line(dedspsPar,str,fp);
        	}
    	}
    	fclose(fp);
	}
	else
	{
		 perror(dedspsPar->paramFile);
	}
	return status;
}

void dedsps_check_line(dedispersion_param *dedspsPar, char *str, FILE *fp)
{
	
	if(str!=NULL)
	{
		if(str[0]=="#")
			fgets(str,1000,fp);
		else if(strncmp(str,"TIME_LENGTH",11)==0)
		{
			fscanf(fp, "%lf", &dedspsPar->timeIntrv);
		}
		else if(strncmp(str,"DM_START",8)==0)
		{
			fscanf(fp, "%lf",&dedspsPar->DMstart);
		}
		else if (strncmp(str,"NUM_DM",5)==0)
		{
			fscanf(fp, "%d",&dedspsPar->numDM);
		}

		else if (strncmp(str,"DM_STEP",7)==0)
		{
			fscanf(fp, "%lf",&dedspsPar->DMstep);
		}

		else if (strncmp(str,"FREQ_START",10)==0)
		{
			fscanf(fp, "%lf",&dedspsPar->freqStart);
		}

		else if (strncmp(str,"FREQ_END",8)==0)
		{
			fscanf(fp, "%lf",&dedspsPar->freqEnd);
		}

		else if (strncmp(str,"SPCTM_NAME",10)==0)
		{
			fscanf(fp, "%s",dedspsPar->spectrumName);
		}
		else
		{
			fprintf(stderr, "Unknown parameter %s"
							" Please check your parameter file.\n",str);
			exit(1);
		}
	
	}

	return ;
}

