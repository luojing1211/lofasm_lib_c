#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>

#include "lofasm_raw_file.h"
#include "lofasm_intgr.h"
int main(){
    lofasm_raw_file raw;
    lofasm_intgr itg;
    int i;
    string filename="20151012_213004.lofasm";
    raw.get_raw_hdr(filename);
    cout<<raw.numIntgr<<endl;
    raw.check_raw_file(filename);
    cout<<raw.numIntgr<<endl;
    ifstream file(filename.c_str(), ios::binary);
    get_lofasm_raw_intgr(raw,itg,0,file);
    for (i=0;i<itg.numfBin;i++){
        cout<<itg.ACrl[i]<<" ";
    }

    for (i=0;i<itg.numfBin;i++){
        cout<<itg.CDim[i]<<" ";
    }

}
