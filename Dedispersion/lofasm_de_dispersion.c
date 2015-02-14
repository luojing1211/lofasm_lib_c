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
	FILE *fp;
	LoFASMIO IOpar = {};

	char *filenames[argc-1];
  	char *frameName; 


	double *DMarray;
	double DMstep;

	double *freqArray;
	double *timeArray;
	double *timeDelay;


	dataArray2D *time_DM;



	dataArray1D *inputData;

	int status;

/************************* File Information Check ***************************/
  	/* Build file queue */
  	for(i = 1;i<argc;i++)
  	{
    	filenames[i-1] = argv[i];
  	}
  	numFile = argc-1;
  	/* Initial the reading queue */
  	status = lofasm_set_file_read(&IOpar,filenames,numFile, 1, "STARTTIME");




}
