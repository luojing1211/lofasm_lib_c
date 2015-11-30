/*Dedispersion cpp version header file */
#ifndef LOFASM_DATA_CLASS_H_
#define LOFASM_DATA_CLASS_H_

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include "lofasm_raw_file.h"
#include "lofasm_intgr.h"
#define SEC_PER_DAY 86400.0
#define MAX_FILENAME_SIZE 80

class test_class
{
    public:
        int a;
        lofasm_raw_file raw;
};

#endif
