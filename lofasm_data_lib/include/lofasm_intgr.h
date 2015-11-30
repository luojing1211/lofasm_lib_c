/*Header file for lofasm integration reading / writing */
#ifndef LOFASM_INTGR_H_
#define LOFASM_INTGR_H_
#define MAX_FILENAME_SIZE 80
#include <string>
using namespace std;

class lofasm_intgr
/* A class for lofasm data reading and writting*/
{
  public :
      int numfBin;
      double starFreq;
      double freqStep;
      vector <unsigned int> AA, BB, CC, DD;
      vector <signed int> ABrl, ACrl, ADrl, BCrl, BDrl, CDrl;
      vector <signed int> ABim, ACim, ADim, BCim, BDim, CDim;
      vector <signed int> AB, AC, AD, BC, BD, CD;
      vector <double> freqAxis;
      lofasm_intgr () : numfBin(0) {};
      void init_polar(int numFreqBin);
      void set_freqAxis(double fstart, double fstep);
      void form_beam();
      void get_polar_cross();
};
#endif
