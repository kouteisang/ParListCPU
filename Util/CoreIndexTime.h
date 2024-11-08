#ifndef CoreIndexTime
#define CoreIndexTime

#include <vector>
#include <utility>
#include <map>

void getResCore(std::vector<std::vector<int>> klmd, std::vector<std::vector<std::pair<int, int>>> results, int n_layer){

    std::map<std::pair<int, int>, std::vector<int>> dataMap;
    for(uint l = 1; l <= n_layer; l ++){
        std::vector<int> ks = klmd[l];

        for(uint i = 0; i < ks.size(); i ++){
            uint k = ks[i];
            std::pair<int, int> kl = {k, l};

            for(uint j = 0; j < results[l].size(); j ++){
                auto key = results[l][j].first;
                auto val = results[l][j].second;
                // printf("key = %d, val = %d\n", key, val);
                if(val >= k){
                    dataMap[kl].push_back(key+1);
                }
            }

        }

    }

    // std::cout << "Hello" << std::endl;
    // std::pair<int, int> kl_key = {39, 1};
    // printf("%d\n", dataMap[kl_key].size());
    // for(uint i = 0; i < dataMap[kl_key].size(); i ++){
    //     printf("%d\n", dataMap[kl_key][i]);
    // }



}

#endif