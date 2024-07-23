#include "FCTreeBuilderPathParallelBylmd.h"
// #include "FCTree.h"

FCTreeBuilderPathParallelBylmd::~FCTreeBuilderPathParallelBylmd(){
}

// ==========common method==========

void FCTreeBuilderPathParallelBylmd::constructCore(uint** degs, uint *klmd, uint *pos, uint *core, uint n_vertex, uint n_layer, coreNode *node, uint new_e, bool serial){

    // The node infor
    node->e = new_e; 
    node->k = klmd[0];
    node->lmd = klmd[1];
    node->length = n_vertex - new_e;
    node->core = new uint[node->length];
    
    memcpy(node->core, core + new_e, (n_vertex - new_e) * sizeof(uint));

    //Every node store the its own degs, o_core, o_pos copy for the serial version
    if(serial){
        node->degs = new uint*[n_vertex];
        for(uint v = 0; v < n_vertex; v ++){
            node->degs[v] = new uint[n_layer];
            memcpy(node->degs[v], degs[v], n_layer * sizeof(uint));
        }

        node->o_core = new uint[n_vertex];
        node->o_pos = new uint[n_vertex];
        memcpy(node->o_core, core, n_vertex * sizeof(uint));
        memcpy(node->o_pos, pos, n_vertex * sizeof(uint));

        // for(uint i = 0; i < n_vertex; i ++){
        //     cout << node->o_pos[i] << " ";
        // }
        // cout << endl;
    }
}

void FCTreeBuilderPathParallelBylmd::PrintCoreInfor(uint *klmd, uint *core, uint new_e, uint n_vertex){
    cout << "( ";
    cout << " k = " << klmd[0] << " ";
    cout << " lmd = " << klmd[1] << " ";
    cout << ") len  = ";
    cout << n_vertex - new_e;
    // for(uint x = new_e; x < n_vertex; x ++){
    //     cout << core[x] << " "; 
    // }
    cout << endl;
}

bool FCTreeBuilderPathParallelBylmd::check(uint **degs, uint u, uint* klmd, uint n_layers){
    uint k = klmd[0];
    uint lmd = klmd[1];
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

uint FCTreeBuilderPathParallelBylmd::peel(MultilayerGraph &mg, uint **degs, uint* klmd, uint *core, uint *pos, uint s, uint e){

    uint n_layers = mg.getLayerNumber();
    uint old_s = s;
    uint old_e = e;
    uint **adj_lst;
    uint v, u;
    uint pos_u;
    bool flag = true;

    while(s < e){
        old_e = e;

        for(uint i = 0; i < n_layers; i ++){
            adj_lst = mg.GetGraph(i).GetAdjLst(); 
            for(uint j = s; j < old_e; j ++){
                v = core[j];
                for(uint l = 1; l <= adj_lst[v][0]; l ++){
                    u = adj_lst[v][l];
                    degs[u][i] --;

                    if(degs[u][i] < klmd[0] && pos[u] >= e){
                        flag = check(degs, u, klmd, n_layers);
                        if(!flag){ // node u does not satisfy the condition
                            pos_u = pos[u];
                            core[pos_u] = core[e];
                            core[e] = u;

                            pos[core[pos_u]] = pos_u;
                            pos[u] = e;
                            e ++;
                        }
                    }
                }
            }
 
        }
        s = old_e;
    }
    return e;
}


// ==========Path Serial==========

void FCTreeBuilderPathParallelBylmd::PathSerial(MultilayerGraph& mg, uint *klmd, uint** degs, coreNode *node, uint *core, uint *pos, uint e, uint &count){
    
    uint s = e;
    uint old_e = e;
    uint new_e = 0;

    uint n_vertex = mg.GetN();
    uint n_layer = mg.getLayerNumber();
    uint old_pos_v;

    for(int i = s; i < n_vertex; i ++){
        uint v = core[i];
        uint cnt = 0;
        bool flag = false;
        for(int j = 0; j < n_layer; j ++){
            if(degs[v][j] >= klmd[0]){
                cnt ++;
                if(cnt >= klmd[1]){
                    flag = true;
                    break;
                }
            }
        }
        if(!flag){
            old_pos_v = pos[v];
            core[old_pos_v] = core[e];
            core[e] = v;

            pos[core[old_pos_v]] = old_pos_v;
            pos[v] = e ++;

        }
    }

    if(s != e){
        new_e = peel(mg, degs, klmd, core, pos, s, e);
    }else if(s == e){
        new_e = e;
    }

    if(n_vertex - new_e > 0){
        count ++;
        // PrintCoreInfor(klmd, core, new_e, n_vertex);
        constructCore(degs, klmd, pos, core, n_vertex, n_layer, node, new_e, true);
    }else{
        node->k = 0;
        node->lmd = 0;
        node->length = 0; 
    } 

    if(n_vertex - new_e > 0 && klmd[0] == 1){

        coreNode* rightChild = new coreNode();
        node->right = rightChild;

        klmd[1] += 1;
        PathSerial(mg, klmd, degs, rightChild, core, pos, new_e, count);

    }

}

// ==========Path Parallel==========

void FCTreeBuilderPathParallelBylmd::PathParallel(MultilayerGraph &mg, coreNode *node, uint** degs, uint *pos, uint *core, uint e){
    
    uint old_pos_v;
    uint s = e;
    uint old_e = e;
    uint new_e = 0;

    uint n_layer = mg.getLayerNumber();
    uint n_vertex = mg.GetN();
    
    uint k = node->k;
    uint lmd = node->lmd;
    uint klmd[2];
    klmd[0] = k+1;
    klmd[1] = lmd;


    // cout << "n_vertex = " << n_vertex << endl;

    for(int i = s; i < n_vertex; i ++){
        uint v = core[i];
        uint cnt = 0;
        bool flag = false;
        for(int j = 0; j < n_layer; j ++){
            if(degs[v][j] >= klmd[0]){
                cnt ++;
                if(cnt >= klmd[1]){
                    flag = true;
                    break;
                }
            }
        }
        if(!flag){
            old_pos_v = pos[v];
            core[old_pos_v] = core[e];
            core[e] = v;

            pos[core[old_pos_v]] = old_pos_v;
            pos[v] = e ++;

        }
    }

    if(s != e){
        new_e = peel(mg, degs, klmd, core, pos, s, e);
    }else if(s == e){
        new_e = e;
    }

   if(n_vertex - new_e > 0){
        coreNode* leftChild = new coreNode();
        node->left = leftChild;
        // PrintCoreInfor(klmd, core, new_e, n_vertex); 
        constructCore(degs, klmd, pos, core, n_vertex, n_layer, leftChild, new_e, false);
        PathParallel(mg, leftChild, degs, pos, core, new_e);
    }else{
        node->left = nullptr;
    }   

}

// ==========Execute==========

void FCTreeBuilderPathParallelBylmd::Execute(MultilayerGraph &mg, FCTree &tree){

    uint count = 0;
    uint n_vertex = mg.GetN(); // number of vertex
    uint n_layers = mg.getLayerNumber();
    uint **degs, **adj_list;

    degs = new uint*[n_vertex];

    // Parallel the degs part
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        // cout << "tid? = " << tid << endl;
        #pragma omp for schedule(static)
        for(int v = 0; v < n_vertex; v ++){
             degs[v] = new uint[n_layers];
        } 

        #pragma omp for schedule(static) collapse(2)
        for(int v = 0; v < n_vertex; v ++){
            // degs[v] = new uint[n_layers];
            for(int l = 0; l < n_layers; l ++){
                degs[v][l] = mg.GetGraph(l).GetAdjLst()[v][0];
            }
        }
    }


    coreNode* node = tree.getNode();

    uint *core = new uint[n_vertex];
    uint *pos = new uint[n_vertex];

    for(uint i = 0; i < n_vertex; i ++){
        core[i] = i;
        pos[i] = i;
    }

    uint e = 0;
    uint klmd[2];
    klmd[0] = 1;
    klmd[1] = 1;

    // Path Serail
    //       (1, 1)
    //    (2, 1)(1, 2)
    // (3, 1)  (2, 2)  (1, 3)
    // (1, 1), (2, 1), (3, 1)  Path serial
    auto start_time = omp_get_wtime();
    PathSerial(mg, klmd, degs, node, core, pos, e, count); 
    auto end_time = omp_get_wtime(); 

    double elapsed_time_serial = end_time - start_time;
    std::cout << "Pathlmd elapsed_time_serial Elapsed time: " << elapsed_time_serial << " seconds\n";

    uint lmdCount = 0;
// Path parallel
#pragma omp parallel
{
    #pragma omp single
    {
        coreNode* root = tree.getNode();
        while(root != nullptr && root->k != 0){
            lmdCount ++;
            #pragma omp task
            {
                PathParallel(mg, root, root->degs, root->o_pos, root->o_core, root->e);
            }
            root = root->right;
        }
        #pragma omp taskwait

    }
}

cout << "lmdCount = " << lmdCount << endl;
//// Path parallel serial version
    // coreNode* root = tree.getNode();
    // while(root != nullptr && root->k != 0){
    //     PathParallel(mg, root, root->degs, root->o_pos, root->o_core, root->e);
    //     root = root->left;
    // }

    //
    //Test to traversal 
    // uint totalCount = 0;
    // // tree.traversal(tree.getNode(), totalCount);
    // cout << "totalCount = " << totalCount << endl;

   

    // Free the memory
    for (uint i = 0; i < n_vertex; i++) delete[] degs[i];
    delete[] degs;


}