#include "CoreIndex.h"

CoreIndex::CoreIndex(/* args */){
}

CoreIndex::~CoreIndex(){
}

bool reverseSort(uint a, uint b) {
    return a > b; 
}

uint findLmdthLargest(uint *degs, int lmd, int n_layer){
    std::nth_element(degs, degs + lmd - 1, degs+n_layer, std::greater<int>());
    return degs[lmd-1];
}

void get_index(MultilayerGraph &mg, uint **degs, uint lmd){
    
    uint n_layer = mg.getLayerNumber();
    uint n_vertex = mg.GetN(); 
    uint *i_v = new uint[n_vertex];
    uint *core = new uint[n_vertex];

    std::vector<std::vector<uint>> B(n_vertex+1);
    std::vector<uint> N;
    
    for(uint v = 0; v < n_vertex; v ++){
        i_v[v] = findLmdthLargest(degs[v], lmd, n_layer);
        B[i_v[v]].push_back(v);
    }

    for(uint k = 1; k <= n_vertex; k ++){
        while(!B[k].empty()){
            uint v = B[k].back();
            B[k].pop_back();
            core[v] = k;
            for(uint l = 0; l < n_layer; l ++){
                uint **adj_lst = mg.GetGraph(l).GetAdjLst();
            }
        }
    }

    cout << endl;
}

void CoreIndex::Execute(MultilayerGraph &mg){
    cout << "I am the CoreIndex" << endl;
    uint n_layer = mg.getLayerNumber();
    uint n_vertex = mg.GetN();

    cout << "n_layer = " << n_layer << endl;
    cout << "n_vertex = " << n_vertex << endl;

    uint **degs;
    degs = new uint*[n_vertex];
    // degs_sort = new uint*[n_vertex];
    
    // Parallel init the degree information
    #pragma omp parallel
    {
        #pragma omp for schedule(static)
        for(int v = 0; v < n_vertex; v ++){
             degs[v] = new uint[n_layer];
            //  degs_sort[v] = new uint[n_layer];
        } 

        #pragma omp for schedule(static) collapse(2)
        for(int v = 0; v < n_vertex; v ++){
            // degs[v] = new uint[n_layers];
            for(int l = 0; l < n_layer; l ++){
                degs[v][l] = mg.GetGraph(l).GetAdjLst()[v][0];
                // degs_sort[v][l] = mg.GetGraph(l).GetAdjLst()[v][0];
            }
        }

        // #pragma omp for schedule(static)
        // for(int v = 0; v < n_vertex; v ++){
        //     std::sort(degs_sort[v], degs_sort[v] + n_layer, reverseSort);
        // }
    }

    // cout << "==========Normal Order==========\n" << endl;

    for(int v = 0; v < n_vertex; v ++){
        cout << "vertex " <<  v << " = ";
        for(int l = 0; l < n_layer; l ++){
            cout << degs[v][l] << " ";
        }
        cout << endl;
    }


    // cout << "==========Reverse Sort Order==========\n" << endl;

    // for(int v = 0; v < n_vertex; v ++){
    //     cout << "vertex " <<  v << " = ";
    //     for(int l = 0; l < n_layer; l ++){
    //         cout << degs_sort[v][l] << " ";
    //     }
    //     cout << endl;
    // }

    for(uint lmd = 1; lmd <= n_layer; lmd ++){

        // Copy the degs degree
        uint **degs_copy = new uint*[n_vertex];
        for(uint v = 0; v < n_vertex; v ++){
            degs_copy[v] = new uint[n_layer];
            memcpy(degs_copy[v], degs[v], n_layer * sizeof(uint));
        }

        get_index(mg, degs_copy, lmd);
    }

}
