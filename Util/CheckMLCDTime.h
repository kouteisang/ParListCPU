#ifndef CheckMLCD
#define CheckMLCD

#include <vector>
#include <map>
#include <utility> // for std::pair


void checktime(std::vector<int> ks, std::vector<int> lmds, std::vector<std::vector<int>> cores){

    std::map<std::pair<int, int>, std::vector<int>> dataMap;
    uint n_cores = cores.size();
    uint n_layer = cores[0].size();

    for(uint i = 0; i < n_cores; i ++){

        for(uint j = 0; j < ks.size(); j ++){
            uint k = ks[j];
            uint lmd = lmds[j];
            bool f = false;
            uint t = 0;
            std::pair<int, int> key = {k, lmd};


            for(uint p = 0; p < n_layer; p ++){
                if(cores[i][p] >= k){
                    t++;
                    if(t >= lmd){
                        f = true;
                        break;
                    }
                }
            }

            if(f){
                dataMap[key].push_back(i);
            }
        }

    }

}

#endif