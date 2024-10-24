#include "CoreIndex.h"

CoreIndex::CoreIndex(/* args */){
}

CoreIndex::~CoreIndex(){
}

bool reverseSort(uint a, uint b) {
    return a > b; 
}

void CoreIndex::Execute(MultilayerGraph &mg){
    cout << "I am the CoreIndex" << endl;
    uint n_layer = mg.getLayerNumber();
    uint n_vertex = mg.GetN();

    cout << "n_layer = " << n_layer << endl;
    cout << "n_vertex = " << n_vertex << endl;

    uint **degs, **degs_sort;
    degs = new uint*[n_vertex];
    degs_sort = new uint*[n_vertex];
    
    // Parallel init the degree information
    #pragma omp parallel
    {
        #pragma omp for schedule(static)
        for(int v = 0; v < n_vertex; v ++){
             degs[v] = new uint[n_layer];
             degs_sort[v] = new uint[n_layer];
        } 

        #pragma omp for schedule(static) collapse(2)
        for(int v = 0; v < n_vertex; v ++){
            // degs[v] = new uint[n_layers];
            for(int l = 0; l < n_layer; l ++){
                degs[v][l] = mg.GetGraph(l).GetAdjLst()[v][0];
                degs_sort[v][l] = mg.GetGraph(l).GetAdjLst()[v][0];
            }
        }

        #pragma omp for schedule(static)
        for(int v = 0; v < n_vertex; v ++){
            std::sort(degs_sort[v], degs_sort[v] + n_layer, reverseSort);
        }
    }

    // cout << "==========Normal Order==========\n" << endl;

    // for(int v = 0; v < n_vertex; v ++){
    //     cout << "vertex " <<  v << " = ";
    //     for(int l = 0; l < n_layer; l ++){
    //         cout << degs[v][l] << " ";
    //     }
    //     cout << endl;
    // }


    // cout << "==========Reverse Sort Order==========\n" << endl;

    // for(int v = 0; v < n_vertex; v ++){
    //     cout << "vertex " <<  v << " = ";
    //     for(int l = 0; l < n_layer; l ++){
    //         cout << degs_sort[v][l] << " ";
    //     }
    //     cout << endl;
    // }

    for(int i = 1; i <= n_layer; i ++){
        get_index();
    }

}
