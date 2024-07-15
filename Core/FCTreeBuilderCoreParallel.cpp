
#include "FCTreeBuilderCoreParallel.h"


FCTreeBuilderCoreParallel::~FCTreeBuilderCoreParallel(){
}

bool FCTreeBuilderCoreParallel::check(uint **degs, uint u, uint k, uint lmd, uint n_layers){
    uint cnt = 0;
    for(int l = 0; l < n_layers; l ++){
        if(degs[u][l] >= k){
            cnt += 1;
            if(cnt >= lmd){
                return true;
            }
        }
    }
    return false;
}

// bool FCTreeBuilderCoreParallel::PeelInvalidInParallel(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNode* node, uint* valid, uint* invalid, uint* cnts, ll_uint *id2vtx){
//     uint n_vertex = mg.GetN(); // number of vertex
//     uint n_layers = mg.getLayerNumber();
//     uint cnt = 0;
//     uint s = 0, e = 0, new_e = 0, old_e = 0;
//     uint **adj_lst;

//     cout << "k = " << k << " lmd = " << lmd << endl;
//     // Debug for test
//     // omp_set_num_threads(1);

// // #pragma omp parallel private(cnt) shared(cnts)
// {
//     // uint tid = omp_get_thread_num();
//     // uint maxthreads = omp_get_num_threads();

//     for(int v = 0; v < n_vertex; v += 1){
//         cnt = 0;
//         if(valid[v] != 0){
//             continue; // only process the valid vertex
//         } 
//         for(int l = 0; l < n_layers; l ++){
//             if(degs[v][l] >= k){
//                 cnt += 1;
//             }
//         }
//         cnts[v] = cnt;


//         if(cnts[v] < lmd){
//             valid[v] = 1;
//             int index;
//             // #pragma omp atomic capture
//             index = e++;
//             invalid[index] = v;
//         }
//     }
// }

//     cout << "e = " << e << endl;

//     s = 0;
//     new_e = e;
    
//     uint vv;
//     while(s < e){
//         s = old_e;

//         // #pragma omp parallel private(vv) shared(s, e, new_e, valid, invalid, degs, cnts, mg)
//         {
//             // #pragma omp for
//             for(uint j = s; j < e; j ++){
//                 vv = invalid[j];
//                 for(uint l = 0; l < n_layers; l ++){
//                     adj_lst = mg.GetGraph(l).GetAdjLst();
//                     for(uint i = 1; i <= adj_lst[vv][0]; i ++){
//                         uint u = adj_lst[vv][i]; // the neighbourhood
//                         if(valid[u] != 0) continue; // only process if u is valid
//                         // minus one and return the old value
//                         // auto originDeg = __sync_fetch_and_sub(&degs[u][l], 1);
//                         degs[u][l] --;
//                         if(degs[u][l] == k-1){
//                             // auto originCnt = __sync_fetch_and_sub(&cnts[u], 1); 
//                             // if(originCnt == lmd){
//                             //     #pragma omp atomic
//                             //         new_e ++;
//                             //     invalid[new_e] = u;
//                             //     valid[u] = 1; // set u invalid
//                             // }
//                             if(!check(degs, u, k, lmd, n_layers)){
//                                 // #pragma omp atomic
//                                 new_e ++;
//                                 invalid[new_e] = u;
//                                 valid[u] = 1; 
//                             }
//                         }
//                     }
//                 }
//             }
//             // #pragma omp barrier
//         }

//         old_e = e;
//         e = new_e;
//     }

   
//     uint res_count = 0;
//     for(int i = 0; i < n_vertex; i ++){
//         if(valid[i] == 0){
//             cout << id2vtx[i] << " ";
//             res_count ++;
//         }
//     }
//     cout << endl;
//     cout << "res_count after = " << res_count << endl;

// }


bool FCTreeBuilderCoreParallel::PeelInvalidInParallel(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNode* node, uint* valid, uint* invalid, uint* cnts, ll_uint *id2vtx){
    uint n_vertex = mg.GetN(); // number of vertex
    uint n_layers = mg.getLayerNumber();
    uint cnt = 0;
    uint s = 0, e = 0, new_e = 0, old_e = 0;
    uint **adj_lst;

    cout << "k = " << k << " lmd = " << lmd << endl;
   
    for(int v = 0; v < n_vertex; v ++){
        cnt = 0;
        if(valid[v] != 0){
            cout << "valid[v] != 0" << endl;
            continue; 
        } 
        for(int l = 0; l < n_layers; l ++){
            if(degs[v][l] >= k){
                cnt ++;
            }
        }
        cnts[v] = cnt;

        if(cnts[v] < lmd){
            valid[v] = 1;
            int index;
            index = e++;
            invalid[index] = v;
        }
    }

    cout << "e = " << e << endl;

    s = 0;
    new_e = e;
    
    uint vv;
    while(s < e){
        s = old_e;

        for(uint j = s; j < e; j ++){
            vv = invalid[j];
            for(uint l = 0; l < n_layers; l ++){
                adj_lst = mg.GetGraph(l).GetAdjLst();
                for(uint i = 1; i <= adj_lst[vv][0]; i ++){
                    uint u = adj_lst[vv][i]; // the neighbourhood
                    if(valid[u] != 0) continue; // only process if u is valid
                    // minus one and return the old value
                    // auto originDeg = __sync_fetch_and_sub(&degs[u][l], 1);
                    degs[u][l] --;
                    if(degs[u][l] == k-1){
                        
                        if(!check(degs, u, k, lmd, n_layers)){
                            invalid[new_e] = u;
                            new_e ++;
                            valid[u] = 1; 
                        }
                    }
                }
            }
        }

        old_e = e;
        e = new_e;
    }

   
    uint res_count = 0;
    for(int i = 0; i < n_vertex; i ++){
        if(valid[i] == 0){
            cout << id2vtx[i] << " ";
            res_count ++;
        }
    }
    cout << endl;
    cout << "res_count after = " << res_count << endl;

}


void FCTreeBuilderCoreParallel::BuildSubFCTree(MultilayerGraph &mg, uint **degs, uint *klmd, coreNode* node, uint* valid, uint* invalid, uint* cnts, ll_uint *id2vtx){ 

    // uint k = klmd[0];
    // uint lmd = klmd[1];

    uint k = 39;
    uint lmd = 1;

    PeelInvalidInParallel(mg, degs, k, lmd, node, valid, invalid, cnts, id2vtx);

}

void FCTreeBuilderCoreParallel::Execute(MultilayerGraph &mg, FCTree &tree, ll_uint *id2vtx){

    coreNode* node = tree.getNode();
        
    uint count = 0;
    uint n_vertex = mg.GetN(); // number of vertex
    uint n_layers = mg.getLayerNumber();
    uint **degs, **adj_list;
    uint* valid = new uint[n_vertex]; // 0 is valid
    uint* cnts = new uint[n_vertex];
    uint* invalid = new uint[n_vertex];

    degs = new uint*[n_vertex];


    // Parallel init the degree and valid part
    #pragma omp parallel
    {
        #pragma omp for schedule(static)
        for(int v = 0; v < n_vertex; v ++){
             degs[v] = new uint[n_layers];
             valid[v] = 0;
        } 

        #pragma omp for schedule(static) collapse(2)
        for(int v = 0; v < n_vertex; v ++){
            // degs[v] = new uint[n_layers];
            for(int l = 0; l < n_layers; l ++){
                degs[v][l] = mg.GetGraph(l).GetAdjLst()[v][0];
            }
        }
    }



    uint klmd[2];
    klmd[0] = 1; // k
    klmd[1] = 1; // lmds

    BuildSubFCTree(mg, degs, klmd, node, valid, invalid, cnts, id2vtx);
    
    // Free the memory
    for (uint i = 0; i < n_vertex; i++) delete[] degs[i];
    delete[] degs;

    cout << "count = " << count << endl;
}