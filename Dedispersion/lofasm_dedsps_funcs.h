#ifndef LOFASM_DEDSPS_FUNCS_H_
#define LOFASM_DEDSPS_FUNCS_H_

#include <math.h>
#include <string>
#include <vector>
#include "lofasm_dedsps_class.h"
using namespace std;

/* Smooth data */
vector<float> smooth_data(vector<float> &data, int smearSize);

/* Simulate data*/
fltbank simulate_flt_ez(double dm, double fstart, double fStep, double tstart,\
                     double tStep, int numfBin, int numtBin,  float noiseAmp, \
                     float noiseBias, float SNR, double highFreqTOA);

int do_dedsps_curve(fltbank & indata, fltbank & outdata, DM_sltIndex & DMsft);

int do_dedsps(fltbank & data, fltbank & outdata, DM_sltIndex & DMsft);

double compute_time_delay(double freq, double freqRef, double dm);

double cal_dmStep_min(double freqMax, double freqMin, double timeStep);

int check_data_size(fltbank & data, DM_time & DMT, vector<DM_sltIndex> & DMsftArray);

int compute_DM_t_power_dommy(fltbank & data, DM_time & DMT, vector<DM_sltIndex> & DMsftArray);

int compute_DM_t_power_tree_dommy(fltbank & data, DM_time & DMT, vector<DM_sltIndex> & DMsftArray);

int compute_DM_t_power_tree(fltbank & data, DM_time & DMT, vector<DM_sltIndex> & DMsftArray);

int compute_DM_t_power_tree_band(fltbank & data, DM_time & DMT, vector<DM_sltIndex> & DMsftArray);

int cal_cut_freq_index(DM_sltIndex & DMsft0, DM_sltIndex & DMsft1);
#endif
