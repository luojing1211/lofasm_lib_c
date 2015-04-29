/*
This is a dedispersed filter bank data constructor program. 

This filter bank data will have a input with 0 DM, The out put is the filter bank data
with a given DM value 
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"lofasm_data_IO.h"
#include"lofasm_data_type.h"
#include"lofasm_de_dispersion.h"

#define HOUR_IN_SEC 3600.0

int main(int argc, char *argv[])
{
    FILE *outfp = NULL;

    LoFASMIO IOpar = {};
    dedispersion_param dedspsPar = {};
    char outFilename[100];
        
    double DM;
    double freqStart;
    double freqEnd;
    double freqStep;
    double intgrTime;
    double timeDelay;
    double numFreqBin;
    int numTimeBin;
    int status;
    int i;
    int intgrIndex = 0;
    int numIntgr;
    double *freqArray=NULL;
    
    dataArray2D *fltb_non_dedsps = NULL;
    dataArray2D *fltb_dedsps = NULL;
    dataArray1D *shiftIndex = NULL;
    /* Testing phase: the second argument is only for DM*/
   
    DM = atof(argv[1]);
   
    sprintf(outFilename,"DM_%lf_FLT.dat",DM);
    //FIXME
    freqStart = 5.0;
    freqEnd = 88.0;
    freqStep = 0.097656;
    numFreqBin = (freqEnd-freqStart)/freqStep;
    intgrTime = 0.083886;    
    numTimeBin = 2*HOUR_IN_SEC/intgrTime;
    numIntgr = 100;
    printf("number freq bin %lf\n",numFreqBin);

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

    shiftIndex = allocate_1d_array(numFreqBin,"SIGNED_INT"); 
    fltb_non_dedsps = allocate_2d_array(numTimeBin,numFreqBin,"UNSIGNED_INT");
    fltb_dedsps = allocate_2d_array(numTimeBin,numFreqBin,"UNSIGNED_INT");
    


    for(i=0;i<numFreqBin;i++)
    {
        timeDelay = -4.15e3*DM*(1.0/(freqArray[i]*freqArray[i])
                               -1.0/(freqArray[0]*freqArray[0]));
        shiftIndex ->data.sData[i] = (int)timeDelay/intgrTime;
        printf("%d ",shiftIndex ->data.sData[i]);
    }
    
    while(1)
    {
        if(intgrIndex>numIntgr-1)
            break;
        
        for(i=0;i<numFreqBin;i++)
        {
            fltb_non_dedsps->data.usData[i][intgrIndex] = (unsigned int)(1000*exp(-pow((double)intgrIndex-(double)numIntgr/2.0,2)/(2*pow(1,2))));
        }  
        intgrIndex++;
        printf("Intgr index %d\n ",intgrIndex);
    }




    return 0;
}
