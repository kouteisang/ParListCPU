#include "CoreIndex.h"

CoreIndex::CoreIndex(/* args */){
}

CoreIndex::~CoreIndex(){
}

bool reverseSort(uint a, uint b) {
    return a > b; 
}

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

uint* get_index(MultilayerGraph &mg, uint **degs, uint lmd, ll_uint *id2vtx){
     

    uint n_layer = mg.getLayerNumber();
    uint n_vertex = mg.GetN(); 
    uint *i_v = new uint[n_vertex];
    uint *core = new uint[n_vertex];

    std::vector<std::set<uint>> B(n_vertex+1);

    bool visit[n_vertex];
    memset(visit, false, sizeof(bool) * n_vertex);


    for(uint v = 0; v < n_vertex; v ++){
        i_v[v] = findLmdthLargest(degs[v], lmd, n_layer); // This is where the problem happens
        B[i_v[v]].insert(v);
    }

    // std::ofstream outFile("/home/cheng/fctree/Output/homo-degree-"+std::to_string(lmd)+".txt");
    // for(uint v = 0; v < n_vertex; v ++){
    //     outFile << id2vtx[v] << ": " << i_v[v] << endl;
    //     // cout << v << ": " << core[v] << endl;
    // }
    // outFile.close();

    
    for(uint k = 0; k <= n_vertex; k ++){
        while(!B[k].empty()){ 
            auto v = B[k].begin();
            uint vv = *v;
            B[k].erase(vv);
            core[vv] = k;
            visit[vv] = true;
            // cout << "vv = " << vv << endl;
            std::set<uint> N;
            for(uint l = 0; l < n_layer; l ++){
                uint **adj_lst = mg.GetGraph(l).GetAdjLst();
                for(uint u = 1; u <= adj_lst[vv][0]; u ++){
                    uint nb_u = adj_lst[vv][u]; // node v's neighbourhood in layer l
                    if(i_v[nb_u] <= k) continue;
                    if(visit[nb_u]) continue;
                    degs[nb_u][l] = degs[nb_u][l] - 1;
                    if(degs[nb_u][l] + 1 == i_v[nb_u]){
                        N.insert(nb_u);
                    }
                }
            }
            
            for(const uint& u : N){
                B[i_v[u]].erase(u);
                i_v[u] = findLmdthLargest(degs[u], lmd, n_layer); 
                B[std::max(i_v[u], k)].insert(u);
            }

        }

    }

    return core;
}

void CoreIndex::Execute(MultilayerGraph &mg, ll_uint *id2vtx){
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




    for(uint lmd = 1; lmd <= n_layer; lmd ++){

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

        uint *core = get_index(mg, degs_copy, lmd, id2vtx);

        
        cout << "lmd = " << lmd << endl;
        cout << "==========" << endl;
        std::ofstream outFile("/home/cheng/fctree/OutputSo/so-"+std::to_string(lmd)+".txt");
        for(uint v = 0; v < n_vertex; v ++){
            outFile << id2vtx[v] << ": " << core[v] << endl;
            // cout << v << ": " << core[v] << endl;
        }
        outFile.close();
    }

}
