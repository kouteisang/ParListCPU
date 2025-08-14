#include <iostream>
#include "header.h"
#include <set>
#include "Util/MemoryUtils.h"

using namespace std;

int main(int argc, char* argv[]){
    // std::set<int> a;
    int* a = new int[1000000];

    for (int i = 0; i < 1000000; i ++){
        a[i] = (i);
    }
    long double mem = GetPeakRSSInMB();
    cout << "mem = " << mem << " MB" << endl; 
    return 0;
}