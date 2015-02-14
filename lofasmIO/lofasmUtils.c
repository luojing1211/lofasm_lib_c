/*
Copyright : Center for Advanced Radio Astronomy, CARA, UTB.

Filename : Source code lofasmUtils.c

Author : Jing Luo UTB, Center for Advanced Radio Astronomy

Version : 1.0.0

Date : 10/16/2014

Descriotion:
    A test file for LoFASMIO library 

 */



#include<stdio.h>
#include"lofasm_data_IO.h"
#include<stdlib.h>
#include<string.h>


char * del_blank(char *str)
/* Delete the blanks in a string*/
{
	char *out = str, *put = str;
	for(; *str!='\0';++str)
	{
		if(*str != ' ')
			*put++ = *str;
	}		
	*put = '\0';
	return out;
}

void swap2(char *word)
/* Swap 2 bytes data */
{
	char tmp;
	tmp = word[0];
	word[0] = word[1];
	word[1] = tmp;
}

void swap4(char *word)
/* Swap 4 bytes data */
{
	char tmp;
  	tmp = word[0]; word[0] = word[3]; word[3] = tmp;
  	tmp = word[1]; word[1] = word[2]; word[2] = tmp;
}

void swap8(char *dword)
/* Swap 8 bytes data */
{
  	char tmp;
  	tmp = dword[0]; dword[0] = dword[7]; dword[7] = tmp;
  	tmp = dword[1]; dword[1] = dword[6]; dword[6] = tmp;
  	tmp = dword[2]; dword[2] = dword[5]; dword[5] = tmp;
  	tmp = dword[3]; dword[3] = dword[4]; dword[4] = tmp;
}


void swapInt(int *word)
/* swap Big Endient of and int type*/
{
	swap4((char *)word);
}

int byte3_2_int(char *input)
/* A function to read 3 bytes data */
{
	unsigned int x = 0;
	char *y = (char *)(&x);
	y[0] = input[2];
	y[1] = input[1];
	y[2] = input[0];
	
 	return x;
}
/* Find if a value in an int array and return the index*/
int find_index_iArray(int *array,int numEle,int value)
{
	int i;
	

   	for (i=0; i<numEle; i++)
   	{
	 	if (array[i] == value)
	 	{
	    		return i;  /* The value has been found*/
	 	}
   	}
   	return(-1);  /* if it was not found */

}

size_t len_string(char *str)
/* Calculate the lenght of string */
{
	int len=0;
	while(*str)
	{
   		len++;
   		str++;
	}
	return len;
}

int search_intgr_list(fIntgrInfo *intgrList, char *key,char *searchVal)
/*Binsection searching for keywords and value in the intgr_list*/
{
	int i, j;
	int resultIndex;
	int searchID;
	int searchPos;
	double searchMJD;
	if(strcmp(key,"ID")==0)
	{
		searchID = atoi(searchVal);
	}
	else if(strcmp(key,"POS")==0)
	{
		searchPos = atoi(searchVal);
	}
	else if(strcmp(key,"MJD")==0)
	{
		searchMJD = atof(searchVal);
	}
	else
	{
		fprintf(stderr, "Unknow keywords %s\n",key);
		resultIndex = -1;
	}


	return resultIndex;

}


