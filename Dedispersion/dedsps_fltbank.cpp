#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<vector>

typedef std::vector< std::vector<unsigned int> > 2Darray;

using namespace std;

// Use vector here could be very convenient 
class fltBank_data{
	int numFreqBin, nuTimeBin;
	char dtype[20];
  public:
	void allocate_mem(int,int,char *);	
}
