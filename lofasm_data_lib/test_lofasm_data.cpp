#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>

#include "lofasm_data_class.h"
using namespace std;

int main(){
    //test read file header
    int ft;
    string filename="./lofasm_raw_file/20151012_213004.lofasm";
    lofasm_file hdr;
    lofasm_data ldata;
    int i;
    ft = hdr.check_file_type(filename);
    cout<<ft<<endl;
    hdr.read_hdr(filename);
    ldata.add_file(filename);

    cout<<ldata.fileList.size()<<endl;
    cout<<ldata.fileList[0].startMJD<<endl;
    ldata.add_file(filename);
    for(i=0;i<2;i++){
       cout<<ldata.fileList[i].startMJD<<endl;
    }
    ldata.open_file(0);
    cout<<ldata.currFileSize;
    ldata.read_one_intgr(2);
    for (i=0;i<ldata.numfbin;i++){
        cout<<ldata.itgr.AA[i]<<" ";
    }
}
