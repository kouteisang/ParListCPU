#include "FCTreeBuilderLeft.h"

FCTreeBuilderLeft::~FCTreeBuilderLeft(){
}

// ========== Common method ==========

bool FCTreeBuilderLeft::check(uint **degs, uint u, uint *klmd, uint n_layers){
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

uint FCTreeBuilderLeft::peel(MultilayerGraph &mg, uint **degs, uint *klmd, uint *core, uint *pos, uint s, uint e){

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

void FCTreeBuilderLeft::restore(MultilayerGraph &mg, uint **degs, uint *core, uint old_e, uint new_e){

    uint n_layers = mg.getLayerNumber();
    uint **adj_lst;
    uint u, v;

    // [old_e, new_e) is the elements that change from active -> inactive during this time.
    for(int i = 0; i < n_layers; i ++){
        adj_lst = mg.GetGraph(i).GetAdjLst();
        for(int j = old_e; j < new_e; j ++){   
            v = core[j];
            for(int l = 1; l <= adj_lst[v][0]; l ++){
                u = adj_lst[v][l];
                degs[u][i] ++;
            }
        }
    } 
}

void FCTreeBuilderLeft::PrintCoreInfor(uint *klmd, uint *core, uint new_e, uint n_vertex){

    cout << "( ";
    cout << "k=" << klmd[0] << ",";
    cout << "lmd=" << klmd[1];
    cout << ") len  = ";
    cout << n_vertex - new_e;
    // for(uint x = new_e; x < n_vertex; x ++){
    //     cout << core[x] << " "; 
    // }
    cout << endl;
    

    std::ofstream outFile("/home/cheng/fctree/klmd/s2_klmd.txt", std::ios::app);
    if (outFile.is_open()) {
        outFile << klmd[0] << " " << klmd[1] << endl;
    }
    outFile.close(); 
}

void FCTreeBuilderLeft::constructCore(uint *klmd, uint *core, uint new_e, uint n_vertex, coreNode *node){

    // The node infor
    node->k = klmd[0];
    node->lmd = klmd[1];
    node->length = n_vertex - new_e;
    node->core = new uint[node->length];
    memcpy(node->core, core + new_e, (n_vertex - new_e) * sizeof(uint));
}


// ========== The following build the tree structure to actually store the data ==========

void FCTreeBuilderLeft::Execute(MultilayerGraph &mg, FCTree &tree){
    coreNode* node = tree.getNode();

        
    uint count = 0;
    uint n_vertex = mg.GetN(); // number of vertex
    uint n_layers = mg.getLayerNumber();
    uint **degs, **adj_list;

    degs = new uint*[n_vertex];

    for(int v = 0; v < n_vertex; v ++){
        degs[v] = new uint[n_layers];
        for(int l = 0; l < n_layers; l ++){
            degs[v][l] = mg.GetGraph(l).GetAdjLst()[v][0];
        }
    }

    uint* core = new uint[n_vertex];
    uint* pos = new uint[n_vertex];

    for(int i = 0; i < n_vertex; i ++){
        core[i] = i;
        // vertex i is in the i-th position
        pos[i] = i;
    }

    uint klmd[2];
    klmd[0] = 1; // k
    klmd[1] = 1; // lmd

    uint e = 0;
    BuildSubFCTree(mg, degs, core, pos, klmd, e, count, node);

    // Free the memory
    for (uint i = 0; i < n_vertex; i++) delete[] degs[i];
    delete[] degs;

    cout << "count = " << count << endl;

   
}

void FCTreeBuilderLeft::BuildSubFCTree(MultilayerGraph &mg, uint **degs, uint *core, uint *pos, uint *klmd, uint e, uint &count, coreNode* node){

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
        PrintCoreInfor(klmd, core, new_e, n_vertex); 
        constructCore(klmd, core, new_e, n_vertex, node);
    }else{
        node->k = 0;
        node->lmd = 0;
        node->length = 0; 
    }

    if(n_vertex - new_e > 0){

        coreNode* leftChild = new coreNode();
        node->left = leftChild;

        klmd[0] += 1;
        BuildSubFCTree(mg, degs, core, pos, klmd, new_e, count, leftChild); 
        klmd[0] -= 1;
    }

    if(n_vertex - new_e > 0 && klmd[0] == 1){

        coreNode* rightChild = new coreNode();
        node->right = rightChild;

        klmd[1] += 1;
        BuildSubFCTree(mg, degs, core, pos, klmd, new_e, count, rightChild);
        klmd[1] -= 1; 

    }

    restore(mg, degs, core, old_e, new_e); 

    s = old_e;
    e = old_e;   
}

// ========== The following without building the tree structure to actually store the data ==========

void FCTreeBuilderLeft::Execute(MultilayerGraph &mg){
    
    uint count = 0;
    uint n_vertex = mg.GetN(); // number of vertex
    uint n_layers = mg.getLayerNumber();
    uint **degs, **adj_list;

    degs = new uint*[n_vertex];

    for(int v = 0; v < n_vertex; v ++){
        degs[v] = new uint[n_layers];
        for(int l = 0; l < n_layers; l ++){
            degs[v][l] = mg.GetGraph(l).GetAdjLst()[v][0];
        }
    }

    uint* core = new uint[n_vertex];
    uint* pos = new uint[n_vertex];

    for(int i = 0; i < n_vertex; i ++){
        core[i] = i;
        // vertex i is in the i-th position
        pos[i] = i;
    }

    uint klmd[2];
    klmd[0] = 1; // k
    klmd[1] = 1; // lmd

    uint e = 0;
    BuildSubFCTree(mg, degs, core, pos, klmd, e, count);

    // Free the memory
    for (uint i = 0; i < n_vertex; i++) delete[] degs[i];
    delete[] degs;

    cout << "count = " << count << endl;

}

void FCTreeBuilderLeft::BuildSubFCTree(MultilayerGraph &mg, uint **degs, uint *core, uint *pos, uint *klmd, uint e, uint &count){

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
        PrintCoreInfor(klmd, core, new_e, n_vertex); 
        klmd[0] += 1;
        BuildSubFCTree(mg, degs, core, pos, klmd, new_e, count); 
        klmd[0] -= 1;
    }

    if(n_vertex - new_e > 0 && klmd[0] == 1){
        klmd[1] += 1;
        BuildSubFCTree(mg, degs, core, pos, klmd, new_e, count);
        klmd[1] -= 1; 

    }

    restore(mg, degs, core, old_e, new_e); 

    s = old_e;
    e = old_e;   
}