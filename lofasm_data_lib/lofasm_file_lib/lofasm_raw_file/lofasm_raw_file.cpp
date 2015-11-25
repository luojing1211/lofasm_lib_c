#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>
#include <string.h>

#include "lofasm_raw_file.h"
using namespace std;

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

void lofasm_raw_file::get_raw_hdr(string fname){

  /*
  To read LoFASM raw data header from one file.

  */
    int status = 0;
    char *buffer = 0;
    size_t result;
    char Temp[20];

    ifstream infile(fname.c_str(), ios::binary);
    if (infile.is_open()){
        // Read header
        infile >> fileSig;
        if (fileSig != "LoCo") {
           cout << "This is not a LOFASM file!\n";
           exit(1);
        }
        buffer = new char[9];
        filename = fname;
        // Read header version
        infile.read(buffer,8);
        hdrVersion = atoi(buffer);
        cout<< " Header Version "<<hdrVersion<<endl;
        // Read header lenght
        infile.read(buffer,8);
        hdrLength = atoi(buffer);
        cout<< " Header Length "<<hdrLength<<endl;
        // Read LoFASM station
        infile.read(buffer,8);
        lofasmStation = buffer;
        cout<<" LoFAMS station "<<lofasmStation<<endl;

        // Read Number of frequency bins/Channels
        infile.read(buffer,8);
        numfBin = atoi(buffer);
        cout<<" Number of freq bin "<<numfBin<<endl;
        // Read start frequency in unit of Mhz
        infile.read(buffer,8);
        freqStart = atof(buffer);
        cout<<" Start Frequency "<<freqStart<<endl;

        // Read frequency steps in unit of Mhz
        infile.read(buffer,8);
        strcpy(Temp,"0.");
        strcat(Temp,buffer);
        freqStep = atof(Temp);
        cout<<" Start Step "<<freqStep<<endl;

        // Read MJD integer part in days
        infile.read(buffer,8);
        mjdDay = atoi(buffer);
        cout<<" Mjd Day "<<mjdDay<<endl;

        // Read MJD fractional part in millisecond
        infile.read(buffer,8);
        mjdMsec = atoi(buffer);
        cout<<" Mjd SEC "<<mjdMsec<<endl;
        // Compute the full mjd date
        startMJD = (double)mjdDay + (double)mjdMsec/SEC_PER_DAY/1000.0;
        // Read integration time for each integration
        infile.read(buffer,8);
        intgrTime = atof(buffer);
        cout<<" integration time  "<<intgrTime<<endl;
        // Read data format version
        infile.read(buffer,8);
        dataFmtVersion = atoi(buffer);
        cout<<" Data Formate version "<<dataFmtVersion<<endl;
        // Read notes
        infile.read(buffer,8);
        notes = buffer;
        cout<<" Notes "<<notes<<endl;

        free(buffer);
        readFlag = 1;
        infile.close();
    }
    else{
        cout<<"Unable to open file "<<fname<<endl;
        exit(1);
    }

}


/* Check if a packet a header packet*/
int lofasm_raw_file::is_hdr_pkt(ifstream& infile)
/*
	Check if the pointer points to the begin of a packet a header packet.
	After this function, the pointer will be
	brought to the bigining of the initial point.
	Input:    ifstream infile
	OutPut:   hdrpkt == 1 it is a header packet
			  hdrpkt == 0 it is not a header packet
*/
{
	int hdrPkt;
	char buffer3Bytes[4];
	int hdrPktSig,hdrPktSig2;   // Two header signatures
	int backCheck;
	size_t result;
	/* First check if the pointer at begining of an integration */
	// Check condition 1. First 3 bytes matchs the signature.
	//    			   2. After 5 bytes, the signature repeats.




	// Read first 3 bytes packet signature
	infile.read(buffer3Bytes,3);
	hdrPktSig = byte3_2_int(buffer3Bytes);
	// Move 5 bytes to the next section
	infile.seekg(5, infile.cur);
	// If it is matches packet signature
	infile.read(buffer3Bytes,3);
	hdrPktSig2 = byte3_2_int(buffer3Bytes);

	// Bring pointer back to the early data see if it different
  if (infile.tellg()<19){
      hdrPkt = 0;
      return hdrPkt;
  }
  infile.seekg(-19, infile.cur);	//
  infile.read(buffer3Bytes,3);
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
	infile.seekg(5,infile.cur);
	return hdrPkt;
}

int lofasm_raw_file::check_raw_file(string fname){
    double numPacket;
    int packetCounter = 0;
    int hdrPktSig;               // The signature for the header packet
    int i;						 // Loop counter
    int intgrIndex;             // The index of intgration
    char buffer3Bytes[4];		 // A buffer for 3 bytes
    int status = 0;
    intgr_info itgINFO;

    /* Check if the header have been read */
    if(readFlag != 1)
    {
        cout<<"File Header has not been read. Start reading header."<<endl;
        get_raw_hdr(fname);
    }

    cout<<"Checking raw file "<<fname<<endl;

    ifstream infile(fname.c_str(), ios::binary);
    if (infile.is_open()){
        /* Get file size; */
        infile.seekg (0, infile.end);
        fileSize = infile.tellg();

        /* Calculate the packet size */
        packetSize = numfBin * 4;

        numPacket = ((double)fileSize - (double)hdrLength)/(double)packetSize;

        /* Check if the data has uncompleted packet*/
        if(fmod(numPacket,1.0) != 0.0)
        {
            cout<<" File "<<fname<<" has incompleted packet."<<endl;
            status = 2;
        }

        /* Check integration status*/
        /* Each integration contains 17 packets. First packet is the header packet,
         which contains repeatly 3 bytes header signature, 3 bytes integration
         number, 2 bytes header counter
         1. We have to determine if the packet is header packet.
         2. We will check the information in header packet
         3. The missing packet will be calculated
        */
			  /* Bring the pointer back to the end of header */
				infile.seekg (hdrLength, infile.beg);
    		packetCounter = PKT_PER_INTGR;
    		intgrIndex = -1;

				for(i=0;i<numPacket;i++)
				{
						/* Process the header packet. */
						if(is_hdr_pkt(infile))
				    {
						    intgrIndex++;
		            /* Record the integration postion */
		 					  itgINFO.intgrPos = infile.tellg();
								/* Skip 3 bytes of header packet signature */
		 					  infile.seekg(3,infile.cur);

		 					  /* Read the integration ID */
		 					  infile.read(buffer3Bytes,3);
								itgINFO.intgrID = byte3_2_int(buffer3Bytes);

								/* Calculate the integration MJD*/
		 					  itgINFO.MJD = startMJD + intgrIndex * intgrTime/SEC_PER_DAY;
		            intgrList.push_back(itgINFO);

								// Checking if the last integration has missing packet.
							  if(packetCounter != PKT_PER_INTGR && intgrIndex != 0)
							  {
								    printf("File %s has missing/additional packet at"
									  " integration %d and it has %d packets.\n",
									  fname.c_str(),intgrIndex-1,packetCounter);
									  if(intgrIndex != 0)
											  intgrList[intgrIndex-1].badIntgr = 1;
							  }

		            // Reset packetCounter to 1
							  packetCounter = 1;
							  /* Bring the pointer to the next packet */
							  infile.seekg(packetSize-6,infile.cur);

						}
						else
						{
								packetCounter++;
								/*Go to next packet*/
								infile.seekg(packetSize,infile.cur);
						}

						if (infile.eof()){
						    if(packetCounter != PKT_PER_INTGR){
									  printf("File %s has missing/additional packet at"
									  " integration %d and it has %d packets.\n",
									  fname.c_str(),intgrIndex,packetCounter);
										intgrList[intgrIndex].badIntgr = 1;
								}
						}

			  }

    }


		if(numIntgr !=0 && intgrIndex >= numIntgr )
		{
				fprintf(stderr, "In file %s, the number of integration from header"
							 " is not match the file\n",fname.c_str());
				numIntgr = intgrIndex+1;
		}
		else{
		    numIntgr = intgrIndex+1;
		}

		infile.close();
    return status;
}
