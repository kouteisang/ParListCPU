#include "FCNaive.h"

FCNaive::FCNaive(/* args */)
{
}

FCNaive::~FCNaive()
{
}

bool FCNaive::check(uint **degs, uint u, int k, int lmd, uint n_layers){
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

void FCNaive::PrintCoreInfor(int k, int lmd, uint *core, uint new_e, uint n_vertex){

    cout << "( ";
    cout << "k=" << k << ",";
    cout << "lmd=" << lmd;
    cout << ") len  = ";
    cout << n_vertex - new_e;
    // for(uint x = new_e; x < n_vertex; x ++){
    //     cout << core[x] << " "; 
    // }
    cout << endl;
    

    // std::ofstream outFile("/home/cheng/fctree/klmd/homo-0-5.txt", std::ios::app);
    // if (outFile.is_open()) {
    //     outFile << klmd[0] << " " << klmd[1] << endl;
    // }
    // outFile.close(); 
}


uint FCNaive::peel(MultilayerGraph &mg, uint **degs, int k, int lmd, uint *core, uint *pos, uint s, uint e){

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

                    if(degs[u][i] < k && pos[u] >= e){
                        flag = check(degs, u, k, lmd, n_layers);
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

void FCNaive::BuildSubFCTree(MultilayerGraph &mg, uint **degs, uint *core, uint *pos, int k, int lmd, uint e, uint &count){

    uint s = e;
    uint old_e = e;
    uint new_e = 0;
    uint old_pos_v;
    uint n_vertex = mg.GetN();
    uint n_layer = mg.getLayerNumber();

    for(int i = s; i < n_vertex; i ++){
        uint v = core[i];
        uint cnt = 0;
        bool flag = false;
        for(int j = 0; j < n_layer; j ++){
            if(degs[v][j] >= k){
                cnt ++;
                if(cnt >= lmd){
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
        new_e = peel(mg, degs, k, lmd, core, pos, s, e);
    }else if(s == e){
        new_e = e;
    }

    if(n_vertex - new_e > 0){
        // count ++;
        // PrintCoreInfor(k, lmd, core, new_e, n_vertex); 
    }

}


void FCNaive::Execute(MultilayerGraph &mg, int k, int lmd){
    
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


    uint e = 0;
    BuildSubFCTree(mg, degs, core, pos, k, lmd, e, count);

    // Free the memory
    for (uint i = 0; i < n_vertex; i++) delete[] degs[i];
    delete[] degs;

    // cout << "count = " << count << endl;

}