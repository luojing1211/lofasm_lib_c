/*  
-Header_File lofasm_data_type.h    

-Abstract     
    Header file of LoFASM 2D data array define. 
    Unsigned data structure
    signed data structure
    float data structure

-Author_and_Institution
    Jing Luo    Center for Advanced Radio Astronomy (CARA)

-Version
    LoFASM DATA Type library 2.0.0. 10-FEB-2015
*/

#ifndef LOFASM_DATA_TYPE_H_
#define LOFASM_DATA_TYPE_H_

#include<stdio.h>

typedef struct {
	union
	{
		unsigned int ** usData;
		signed int **sData;
		long **lData;
		float **fData;
		double **dData;
	} data;

	char dtype[20];
	int numFreqBin;
	int numIntgr;
	char alctFlag;
}dataArray2D;

typedef struct {
	union
	{
		unsigned int *usData;
		signed int *sData;
		long *lData;
		float *fData;
		double *dData;
	} data;


	char dtype[20];
	int lenArray;
	
	char alctFlag;
}dataArray1D;



/***************************** Functions **************************************/

/** Allocate the different type of data **/
dataArray2D * allocate_2d_array(int numFreqBin, int numIntgr, char *dtype);

dataArray1D * allocate_1d_array(int lenArray, char *dtype);

/** Free different type of data **/

void free_2d_array(dataArray2D * array2D);

void free_1d_array(dataArray1D * array1D);




#endif


