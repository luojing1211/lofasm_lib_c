/******************************************************************************
Copyright : Center for Advanced Radio Astronomy, CARA, UTB. 

Filename : Source code lofasm_data_type.c

Author: Jing Luo UTB, Center for Advanced Radio Astronomy

Version : 2.0.0

Data : 2/10/2015

Description:
This program is a part for LoFASM c library.
It is designed for LoFASM data type functions
***********************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "lofasm_data_type.h"
#include<string.h>

/** allocate 2d array **/
dataArray2D * allocate_2d_array(int numFreqBin, int numIntgr, char *dtype)
{	
	dataArray2D *array2D=NULL;
	int i;


	array2D = (dataArray2D *)malloc(sizeof(dataArray2D));
	

	if(array2D == NULL)
	{
		fprintf(stderr,"Memory error when creating 2D array\n");
		exit(1);
	}
	
	array2D -> numFreqBin = numFreqBin;
	array2D -> numIntgr = numIntgr;


	if(strcmp(dtype,"UNSIGNED_INT")==0)
	{

		array2D -> data.usData = (unsigned int **)malloc(sizeof(unsigned int *)
						*array2D->numIntgr);

		if(!array2D -> data.usData)
		{
			fprintf(stderr,"Memory error when allocate unsigned int data array\n");
      		exit(1);
		}

		for(i=0;i<array2D -> numIntgr;i++)
		{
			array2D -> data.usData[i] = (unsigned int *)malloc(array2D ->numFreqBin*
													sizeof(unsigned int));
			if(!array2D->data.usData[i])
			{
				fprintf(stderr,"Memory error when allocate unsigned int data" 
							" array[%d]\n",i);
      			exit(1);
			}

		}


		strcpy(array2D->dtype,"UNSIGNED_INT");
	}

	else if(strcmp(dtype, "SIGNED_INT")==0)
	{
		array2D -> data.sData = (signed int **)malloc(sizeof(signed int *)
						*array2D->numIntgr);

		if(!array2D -> data.sData)
		{
			fprintf(stderr,"Memory error when allocate signed int data array\n");
      		exit(1);
		}

		for(i=0;i<array2D -> numIntgr;i++)
		{
			array2D -> data.sData[i] = (signed int *)malloc(array2D ->numFreqBin*
													sizeof(signed int));
			if(!array2D-> data.sData[i])
			{
				fprintf(stderr,"Memory error when allocate signed int data" 
							" array[%d]\n",i);
      			exit(1);
			}

		}

		strcpy(array2D->dtype,"SIGNED_INT");

	}

	else if(strcmp(dtype, "LONG")==0)
	{
		array2D -> data.lData = (long **)malloc(sizeof(long *)
						*array2D->numIntgr);

		if(!array2D -> data.lData)
		{
			fprintf(stderr,"Memory error when allocate float data array\n");
      		exit(1);
		}

		for(i=0;i<array2D -> numIntgr;i++)
		{
			array2D -> data.lData[i] = (long *)malloc(array2D ->numFreqBin*
													sizeof(long));
			if(!array2D->data.lData[i])
			{
				fprintf(stderr,"Memory error when allocate long data" 
							" array[%d]\n",i);
      			exit(1);
			}

		}

		strcpy(array2D->dtype,"LONG");

	}

	else if(strcmp(dtype, "FLOAT")==0)
	{
		array2D -> data.fData = (float **)malloc(sizeof(float *)
						*array2D->numIntgr);

		if(!array2D -> data.fData)
		{
			fprintf(stderr,"Memory error when allocate float data array\n");
      		exit(1);
		}

		for(i=0;i<array2D -> numIntgr;i++)
		{
			array2D -> data.fData[i] = (float *)malloc(array2D ->numFreqBin*
													sizeof(float));
			if(!array2D->data.fData[i])
			{
				fprintf(stderr,"Memory error when allocate float data" 
							" array[%d]\n",i);
      			exit(1);
			}

		}

		strcpy(array2D->dtype, "FLOAT");
	}

	else if(strcmp(dtype,"DOUBLE")==0)
	{
		array2D -> data.dData = (double **)malloc(sizeof(double *)*array2D->numIntgr);

		if(!array2D -> data.dData)
		{
			fprintf(stderr,"Memory error when allocate double data array\n");
      		exit(1);
		}

		for(i=0;i<array2D -> numIntgr;i++)
		{
			array2D -> data.dData[i] = (double *)malloc(array2D ->numFreqBin*sizeof(double));
			if(!array2D->data.dData[i])
			{
				fprintf(stderr,"Memory error when allocate unsigned int data" 
							" array[%d]\n",i);
      			exit(1);
			}

		}

		strcpy(array2D->dtype,"DOUBLE");
	}


	else
	{
		fprintf(stderr,"Unknown data type. Unable to allocate 2D array\n");
		exit(1);
	}
	
	array2D -> alctFlag = 1;
	return array2D;
}


/* Allocate 1d array */
dataArray1D * allocate_1d_array(int lenArray, char *dtype)
{
	dataArray1D *array1D = NULL;
	int i;

	

	array1D = (dataArray1D *)malloc(sizeof(dataArray1D));
	

	if(array1D == NULL)
	{
		fprintf(stderr,"Memory error when creating 1D array\n");
		exit(1);
	}

	array1D -> lenArray = lenArray;

	if(strcmp(dtype, "UNSIGNED_INT")==0)
	{
		array1D -> data.usData = (unsigned int *)malloc(lenArray*sizeof(unsigned int));
		if(array1D -> data.usData == NULL)
		{
			fprintf(stderr,"Memory error when creating 1D"
							"unsigned int array\n");
			exit(1);	
		}

		strcpy(array1D->dtype,"UNSIGNED_INT");

	}
	else if(strcmp(dtype, "SIGNED_INT")==0)
	{
		array1D -> data.sData = (signed int *)malloc(lenArray*sizeof(signed int));
		if(array1D -> data.sData == NULL)
		{
			fprintf(stderr,"Memory error when creating 1D"
							"signed int array\n");
			exit(1);	
		}

		strcpy(array1D->dtype,"SIGNED_INT");
	}

	else if(strcmp(dtype, "LONG")==0)
	{
		array1D -> data.lData = (long *)malloc(lenArray*sizeof(long));
		if(array1D -> data.lData == NULL)
		{
			fprintf(stderr,"Memory error when creating 1D"
							"long array\n");
			exit(1);	
		}
		
		strcpy(array1D->dtype,"LONG");
	}

	else if(strcmp(dtype, "FLOAT")==0)
	{
		array1D -> data.fData = (float *)malloc(lenArray*sizeof(float));
		if(array1D -> data.fData == NULL)
		{
			fprintf(stderr,"Memory error when creating 1D"
							"float array\n");
			exit(1);	
		}


		strcpy(array1D->dtype,"FLOAT");
	}

	else if(strcmp(dtype, "DOUBLE")==0)
	{
		array1D -> data.dData = (double *)malloc(lenArray*sizeof(double));
		if(array1D -> data.dData == NULL)
		{
			fprintf(stderr,"Memory error when creating 1D"
							"double array\n");
			exit(1);	
		}


		strcpy(array1D->dtype,"DOUBLE");
	}
	else
	{
		fprintf(stderr,"Unknown data type. Unable to allocate 1D array\n");
		strcpy(array1D->dtype,"NONE");
		exit(1);
	}

	array1D -> alctFlag = 1;
	return array1D;
}


/** Free different type of data **/

void free_2d_array(dataArray2D * array2D)
{
	int status = 0;
	int i;
	if(array2D->data.usData != NULL)
	{
		for(i=0;i<array2D->numIntgr;i++)
		{
			free(array2D->data.usData[i]);
			array2D->data.usData[i] = NULL;
		}

		free(array2D->data.usData);
		array2D->data.usData = NULL;


	}


	if(array2D->data.sData != NULL)
	{
		for(i=0;i<array2D->numIntgr;i++)
		{
			free(array2D->data.sData[i]);
			array2D->data.sData[i] = NULL;
		}

		free(array2D->data.sData);
		array2D->data.sData = NULL;
	}

	if(array2D->data.lData != NULL)
	{
		for(i=0;i<array2D->numIntgr;i++)
		{
			free(array2D->data.lData[i]);
			array2D->data.lData[i] = NULL;
		}

		free(array2D->data.lData);
		array2D->data.lData = NULL;
	}

	if(array2D->data.fData != NULL)
	{
		for(i=0;i<array2D->numIntgr;i++)
		{
			free(array2D->data.fData[i]);
			array2D->data.fData[i] = NULL;
		}

		free(array2D->data.fData);
		array2D->data.fData = NULL;
	}

	if(array2D->data.dData != NULL)
	{
		for(i=0;i<array2D->numIntgr;i++)
		{
			free(array2D->data.dData[i]);
			array2D->data.dData[i] = NULL;
		}

		free(array2D->data.dData);
		array2D->data.dData = NULL;
	}

	array2D->numFreqBin = 0;
	array2D->numIntgr = 0;

	strcpy(array2D->dtype,"NONE");
	array2D->alctFlag = 0;
}

void free_1d_array(dataArray1D * array1D)
{
	int status;

	if(array1D->data.usData != NULL)
	{
		free(array1D->data.usData);
		array1D->data.usData = NULL;
	}

	if(array1D->data.sData != NULL)
	{
		free(array1D->data.sData);
		array1D->data.sData = NULL;
	}

	if(array1D->data.lData != NULL)
	{
		free(array1D->data.lData);
		array1D->data.lData = NULL;
	}

	if(array1D->data.fData != NULL)
	{
		free(array1D->data.fData);
		array1D->data.fData = NULL;
	}

	if(array1D->data.dData != NULL)
	{
		free(array1D->data.dData);
		array1D->data.dData = NULL;
	}

	array1D->lenArray = 0;
	array1D->alctFlag = 0;

	strcpy(array1D->dtype,"NONE");

}
