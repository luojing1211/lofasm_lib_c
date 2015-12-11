#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

#include "lofasm_data_class.h"
#include "lofasm_intgr.h"

int main(){
    lofasm_file fi;
    string filename = "../../test_data/20151012_213004.lofasm";
    cout<<fi.check_file_type(filename);

    return 0;
}
