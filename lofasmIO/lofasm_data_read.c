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
	
***********************************************************************/


#include<stdio.h>
#include<stdlib.h>
#include "lofasm_data_IO.h"
#include<string.h>