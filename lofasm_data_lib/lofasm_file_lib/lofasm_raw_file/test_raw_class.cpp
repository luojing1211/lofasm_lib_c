#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>

#include "lofasm_raw_file.h"

int main(){
    lofasm_raw_file raw;
    string filename="20151012_213004.lofasm";
    raw.get_raw_hdr(filename);
    cout<<raw.numIntgr<<endl;
    raw.check_raw_file(filename);
    cout<<raw.numIntgr<<endl;
}
