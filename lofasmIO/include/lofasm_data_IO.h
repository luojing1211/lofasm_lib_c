/*  
-Header_File lofasm_data_IO.h    

-Abstract     
    Header file of LoFASM data input and output library. 
    Compatible data type:
        LoFASM raw data, ASCII, sigproc

-Author_and_Institution
    Jing Luo    Center for Advanced Radio Astronomy (CARA)

-Version
    LoFASM DATA I/O library 2.0.0. 26-JAM-2015
*/


#ifndef LOFASM_DATA_IO_H_
#define LOFASM_DATA_IO_H_

#include<stdio.h>

#define SEC_PER_DAY 86400.0
#define INIT_INTGR_LIST_SIZE 200
#define INIT_BAD_INTGR_LIST_SIZE 10     
#define MAX_FILENAME_SIZE 256 
#define MAX_FRAME_NAME_SIZE 3 

/* An structure to store the integration information in the hdrInfo*/
typedef struct {
	int intgrID;    // The ID of the integration
    long int intgrPos;   // The integration beginning bit's location in the file
	double intgrMJD; // The MJD time of the integration
	char badIntgrFlag; // The flag for a bad integration
} fIntgrInfo;

/* Structure for one file information*/
typedef struct {
    char filename[MAX_FILENAME_SIZE];             // Current file name
    char *hdrSig;            // File type
	int fileType;            // Data file type
    int hdrVersion;          // File header version, mostly for raw data
    int hdrLength;           // File header lenght
    char *lofasmStation;     // LoFASM station name
    int numBin;                 // Number of Channels/bins 
    double freqStart;              // Start Frequency 
    double freqStep;               // Frequency step
    int mjdDay;                 // Data taking date in mjd formate, 
                                //   the integer part or mjd
    int mjdMsec;                // Data taking date in mjd formate,
                                //   the fractional part in milliseconds
    double startMJD;             // file start mjd date to milliseconds unit in day
	double endMJD;               // File end mjde data
	double intgrTime;              // Integration total time for each integration  
    int dataFmtVersion;         // data format version
    char *notes;             // notes for the file
    long fileSize;               // File size
    int numIntgr;               // Number of integration in the file
    char numIntgrFlag;
    int packetSize;             // Size of each data packet in bytes
    int intgrSize;              // Size of each data integration in bytes
	int hdrPktSig;

	fIntgrInfo *intgrList;       // list of intgration information. 

    int numBadIntgr;
	int lenBadIntgrList;	// Current lenght of _bad_ integration ID
    int readFlag;           // If header has been correctly read, readFlag == 1
	int fileCheckFlag; 
}hdrInfo;

/* Structure for file nodes */
typedef struct fNode {
	char * filename;         // The name of the file
	int index;               // The index of the file in the list
	hdrInfo hdr;             // Header information for the file
	struct fNode * nextFile; // Pointer to next file
	struct fNode * lastFile; // Pointer to last file
} fileNode;

/* Structure for LoFASM one integration data*/
typedef struct {
	char frameSetFlag;           // See if the frame has been set. 
	int numFrm;					// Number of frame name   
    int numChannel;             // Number of freqncy channel
    int intgrIndex;            // The index of the intgration of the file
    int intgrID;               // The integration ID 
	int badIntgrFlag;              // Bad integration flag
    double MJD;                // MJD for intgration to millionseconds
    int unsgnNum;              // Number of unsigned data in one packet
    int sgnNum;                // Number of signed data in one packet
    int pktIndex1;             // Packet index of selected frame.
    int pktIndex2;             // Index of second packet
    int indexParity;               //  parity of element index 0 even 1 odd
	char * hdrPktData;            // Data from header packet. 
    unsigned int * unsgnData1;     // Unsigned data part 1 in the integration 
	signed int * sgnData1;        // Signed data packet part 1
 	unsigned int * unsgnData2;     // Unsigned data part 2 in the integration
	signed int * sgnData2;        // Signed data packet part 2
    unsigned int * AAdat;   		// Spectrum of AA
    unsigned int * BBdat;   		// Spectrum of BB
	unsigned int * CCdat;   		// Spectrum of CC
	unsigned int * DDdat;   		// Spectrum of DD
	signed int * ABdatRl;		// Spectrum of AB real part
	signed int * ACdatRl;		// Spectrum of AC real part
	signed int * ADdatRl;		// Spectrum of AD real part
	signed int * BCdatRl;		// Spectrum of BC real part
	signed int * BDdatRl;		// Spectrum of BD real part
	signed int * CDdatRl;		// Spectrum of CD real part
	signed int * ABdatIm;		// Spectrum of AB imaginary part
	signed int * ACdatIm;		// Spectrum of AC imaginary part
	signed int * ADdatIm;		// Spectrum of AD imaginary part
	signed int * BCdatIm;		// Spectrum of BC imaginary part
	signed int * BDdatIm;		// Spectrum of BD imaginary part
	signed int * CDdatIm;		// Spectrum of CD imaginary part
    signed int * sgnPacket2;  // Signed data packet 2
    signed int * crsPwrRl;    // Real part cross power of two polarizations 
    signed int * crsPwrIm;    // Imaginary part cross power of two polarizations 
    void *frameData;
    float *beamData;
    
    int initFlag;             // If data structure has been correctly 
                               // initialized initFlag ==1
}dataIntgr;

typedef struct {
	char *mode;				// Read or write	
	char **fileNameList;     // List of file names
	int lenFileNameList;
	char *currSpectrum;
	int **frmCode;         // The code for each frame name.
	int lenFrmCode;
	char *spectrumDtype;
	double startMjdDeflt;     // The default start mjd time
	double endMjdDeflt;		  // The default end mjd time
	double startMjdSlct;	 // The selected start mjd time
	double endMjdSlct;  	 // The selected end mjd time     
	double startFreqDeflt;
	double endFreqDeflt;
	double startFreqSlct;
	double endFreqSlct;
	int numIntgr;			 // Number of intgeration in the block of data
	int numBins;             // Number of frequency bins in the block of data
	double *timeAxis;        // Time axis for the block of data 
	double *freqArray;        // Freqency array for the file
	hdrInfo hdr;
	hdrInfo *CurrentHdr;      		 // Header information of each file
	dataIntgr intgr;      // Data struct for each intgeration
	int initIOFlag;			// initialized initFlag ==1?
	fileNode * fileQhead;       // A linked file list. 
	fileNode * fileQend;
	fileNode * currentFile;
}LoFASMIO;



/************************** Functions **********************************/

/*** Initial reading data***/
fileNode * create_file_node(char *fileName, int index);

int lofasm_create_file_Q(LoFASMIO *IOpar,char *fileNames[], int numFiles);

int lofasm_set_file_read(LoFASMIO *IOpar,char *fileNames[], int numFiles,
						char sortFlag, char *sortKey);

int sort_file_nodes(fileNode **Qhead, fileNode **Qend, char *keyWord);


/*** Selete start and end time ***/
int lofasm_set_time(LoFASMIO *IOpar, double startMjd, double endMjd);


/*** Selete start and end frequency ***/
int lofasm_set_freq(LoFASMIO *IOpar, double startFreq, double endFreq);

int lofasm_init_read(LoFASMIO *IOpar,char **fileNames, int numFiles);

int lofasm_init_write(char *fileName_tmplt);


/*** Check file type ***/
int check_file_type(char *filename);

/*** Open file ***/
int lofasm_open_file(hdrInfo *hdr, char * filename, FILE **fptr, 
					 char * mode);
/*** Read header for different version of file***/
int lofasm_read_file_hdr(hdrInfo *hdr,FILE *fp);

/*** Check the frame name and return the frame code ***/
int lofasm_check_frame(int frmCode[],char * frmName);

int lofasm_set_frame(LoFASMIO *IOpar, char *spectrumName);

/*** Print header information ***/
int lofasm_print_hdr_info(LoFASMIO *IOpar,char *key);

/*** Read filt bank data from multiple files ***///FIXME, name of the function
int lofasm_init_reading(LoFASMIO *IOpar, double startMjd, double endMjd, 
						double startFreq, double endFreq, char *mod);

int lofasm_read_unsignedint_data(LoFASMIO *IOpar, int intgrReadNum, 
								unsigned int *udata, FILE *fp);

int lofasm_read_signedint_data(LoFASMIO *IOpar, signed int **idat, int numIntgr);

int lofasm_read_float_data(LoFASMIO *IOpar, float **fdat, int numIntgr);

int lofasm_get_beam(LoFASMIO *IOpar, float **fdat, int numIntgr, char *beamName);


/********* LoFASM header information *********/


/******** These functions are in the file lofasmIOV2.c ********/
/*** Read LoFASM raw data header ***/






/********* LoFASM Data struct initialize *********/
/*** Initialize data struct***/
int lofasm_init_data(LoFASMIO *IOpar);


/********* LoFASM file check *********/
/*** Check file status ***/
int lofasm_check_file(LoFASMIO *IOpar, FILE *filePtr);




/********* LoFASM data reading *********/





/************************ LoFASM Raw data **************************/
int check_raw_file(hdrInfo *hdr, FILE *filePtr);
int read_raw_hdr(hdrInfo *hdr, FILE *filePtr);

int is_hdr_pkt(FILE *filePtr);


int check_raw_intgr(dataIntgr *intgr, hdrInfo *hdr, FILE *filePtr);

int read_raw_intgr(LoFASMIO *IOpar, FILE *filePtr, int intgrIndex);

 int init_raw_reading(LoFASMIO *IOpar);



/*** Read sigproc (pulsar signal processing programs) LoFASM data ***/
int read_sigproc_one_intg(LoFASMIO *IOpar, FILE *filePtr);


/********* Basic operarions *********/
/*** Delete space in a string ***/
char * del_blank(char *str);

/*** For big endiant ***/
void swap2(char *word);

void swap4(char *word);

void swap8(char *dword);

void swapInt(int *word);
/*** Convert 3 bytes data to int ***/
int byte3_2_int(char *input);

int find_index_iArray(int *array,int numEle,int value);

size_t len_string(char *str);

int search_intgr_list(fIntgrInfo *intgrList, char *key,char *searchVal);
#endif


