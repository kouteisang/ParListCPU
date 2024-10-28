#include "CoreIndex.h"

CoreIndex::CoreIndex(/* args */){
}

CoreIndex::~CoreIndex(){
}

bool reverseSort(uint a, uint b) {
    return a > b; 
}

// uint findLmdthLargest(uint *degs, int lmd, int n_layer){
//     std::nth_element(degs, degs + lmd - 1, degs+n_layer, std::greater<int>());
//     return degs[lmd-1];
// }

void print(std::vector<std::vector<uint>> B, uint n_vertex){
    cout << "==========Round==========" << endl;
   for(uint k = 1; k <= n_vertex; k ++){
        cout << "k = " << k << ": ";
        for(uint j = 0; j < B[k].size(); j ++){
            cout << B[k][j] << " ";
        }
        cout << endl;
    }
}

uint findLmdthLargest(uint *deg, uint lmd, uint n_layer){

    uint *deg_sort = new uint[n_layer];
    memcpy(deg_sort, deg, sizeof(uint) * n_layer);


    sort(deg_sort, deg_sort+n_layer, reverseSort);

    return deg_sort[lmd-1];
}

uint* get_index(MultilayerGraph &mg, uint **degs, uint lmd){
     

    uint n_layer = mg.getLayerNumber();
    uint n_vertex = mg.GetN(); 
    uint *i_v = new uint[n_vertex];
    uint *core = new uint[n_vertex];

    std::vector<std::vector<uint>> B(n_vertex+1);

    
    // cout << "==========Befor Order==========\n" << endl;

    // for(int v = 0; v < n_vertex; v ++){
    //     cout << "vertex " <<  v << " = ";
    //     for(int l = 0; l < n_layer; l ++){
    //         cout << degs[v][l] << " ";
    //     }
    //     cout << endl;
    // }

    for(uint v = 0; v < n_vertex; v ++){
        i_v[v] = findLmdthLargest(degs[v], lmd, n_layer); // This is where the problem happens
        B[i_v[v]].push_back(v);
    }

    // cout << "==========After Order==========\n" << endl;

    // for(int v = 0; v < n_vertex; v ++){
    //     cout << "vertex " <<  v << " = ";
    //     for(int l = 0; l < n_layer; l ++){
    //         cout << degs[v][l] << " ";
    //     }
    //     cout << endl;
    // }

    // cout << "=====i_v information=====" << endl;
    for(uint v = 0; v < n_vertex; v ++){
        cout << "v = " << v << " i_v = " << i_v[v] << endl;
    }


    // cout << "=====B information=====" << endl;
    for(uint k = 1; k <= n_vertex; k ++){
        cout << "k = " << k << ": ";
        for(uint j = 0; j < B[k].size(); j ++){
            cout << B[k][j] << " ";
        }
        cout << endl;
    }

    //     cout << "=====5 degree information=====" << endl;
    // cout << "deg 5 0 = " << degs[5][0] << endl;
    // cout << "deg 5 1 = " << degs[5][1] << endl;
    
    for(uint k = 1; k <= n_vertex; k ++){
        while(!B[k].empty()){
            std::set<uint> N;
            int v = B[k].back();
            B[k].pop_back();
            core[v] = k;
            for(uint l = 0; l < n_layer; l ++){
                uint **adj_lst = mg.GetGraph(l).GetAdjLst();
                for(uint u = 1; u <= adj_lst[v][0]; u ++){
                    uint nb_u = adj_lst[v][u]; // node v's neighbourhood in layer l
                    if(i_v[nb_u] <= k) continue;
                    // cout << "I am here before" << " l = " << ll << " nb_u = "  << nb_u << " i_v[nb_u] = " <<  i_v[nb_u]  << " deg = " << degs[nb_u][ll] << endl;
                    degs[nb_u][l] = degs[nb_u][l] - 1;
                    // cout << "I am here after" << " l = " << ll << " nb_u = " << nb_u << " degree [nb_u] = " <<  degs[nb_u][ll] << endl;
                    if(degs[nb_u][l] == i_v[nb_u] - 1){
                        N.insert(nb_u);
                    }
                }
            }
            // cout << "k = " << k << " v = " << v << endl;
            // for(const uint &u : N){
            //     cout << "u = " << u << endl;
            // }
            for(const uint& u : N){
                // cout << "k ist = " << k  <<  " u here ist = " << u << endl;
                B[i_v[u]].erase(std::remove(B[i_v[u]].begin(), B[i_v[u]].end(), u), B[i_v[u]].end());
                i_v[u] = findLmdthLargest(degs[u], lmd, n_layer); 
                bool flag = std::find(B[std::max(i_v[u],k)].begin(), B[std::max(i_v[u],k)].end(), u) != B[std::max(i_v[u],k)].end();
                if(!flag){
                    B[std::max(i_v[u],k)].push_back(u);
                }
            }

            // print(B, n_vertex);
            break;
        }

    }

    return core;
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




    for(uint lmd = 2; lmd <= n_layer; lmd ++){

        // Copy the degs degree
        uint **degs_copy = new uint*[n_vertex];
        for(uint v = 0; v < n_vertex; v ++){
            degs_copy[v] = new uint[n_layer];
            memcpy(degs_copy[v], degs[v], n_layer * sizeof(uint));
        }
        // for(int v = 0; v < n_vertex; v ++){
        //     cout << "vertex " <<  v << " = ";
        //     for(int l = 0; l < n_layer; l ++){
        //         cout << degs_copy[v][l] << " ";
        //     }
        //     cout << endl;
        // }

        uint *core = get_index(mg, degs_copy, lmd);

        cout << "lmd = " << lmd << endl;
        cout << "==========" << endl;
        for(uint v = 0; v < n_vertex; v ++){
            cout << "v = " << v << " k = " << core[v] << "\n";
        }
    }

}
