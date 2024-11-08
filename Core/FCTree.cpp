#include "FCTree.h"



FCTree::~FCTree() {
    // deleteNode(node);
}


struct LayerDegree{
    uint id;
    uint degree;
};

bool cmp(const LayerDegree &a, const LayerDegree &b){
    return a.degree > b.degree;
}

void calculateWeightDenestSubgraph(coreNode *node, MultilayerGraph &mg, float beta, float &maximum_density){
    uint n_layers = mg.getLayerNumber();
    uint n_vertex = mg.GetN();
    uint *core = node->core;
    uint n_node = node->length;

    // uint *degree = new uint[n_layers];

    std::unordered_set<uint> nodes_set;

    for(uint i = 0; i < n_node; i ++){
        nodes_set.insert(core[i]);
    }

    LayerDegree *layer_degree = new LayerDegree[n_layers];

    for(uint l = 0; l < n_layers; l ++){
        uint **adj = mg.GetGraph(l).GetAdjLst();
        int n_edge = 0;
        for(uint v = 0; v < n_node; v ++){
            auto vv = core[v]; // This is the vertex
            for(uint u = 1; u <= adj[vv][0]; u ++){
                auto uu = adj[vv][u];
                if(nodes_set.find(uu) != nodes_set.end()){
                    n_edge ++;
                }
            }
        }
        layer_degree[l].id = l;
        layer_degree[l].degree = n_edge/2;
    }

    float *maximum_average_degree = new float[n_layers];
    sort(layer_degree, layer_degree+n_layers, cmp);

    for(uint l = 0; l < n_layers; l ++){
        auto lid = layer_degree[l].id;
        maximum_average_degree[lid] = layer_degree[l].degree / n_node;
        float layer_density = maximum_average_degree[lid] * pow((l+1), beta); 
        if(layer_density > maximum_density){
            maximum_density = layer_density;
            cout << "Maximum density = " << maximum_density << endl;
        }
    }
    
    delete[] layer_degree;

    // cout << "len = " << n_node << endl;
} 

void FCTree::WeightDenestSubgraph(coreNode *node){
    
    if(node != nullptr && node->k != 0 && node->lmd != 0){
        cout << "(k = " << node->k << " lmd = " << node->lmd << ") len = " << node->length << endl;
        WeightDenestSubgraph(node->left);
        WeightDenestSubgraph(node->right);
    }
}

void FCTree::WeightDenestSubgraph(coreNode *node, MultilayerGraph &mg, float beta, float &maximum_density){
    
    if(node != nullptr && node->k != 0 && node->lmd != 0){
        cout << "(k = " << node->k << " lmd = " << node->lmd << ") len = " << node->length << endl;
        calculateWeightDenestSubgraph(node, mg, beta, maximum_density); 
        WeightDenestSubgraph(node->left, mg, beta, maximum_density);
        WeightDenestSubgraph(node->right, mg, beta, maximum_density);
    }
}

// Test
void FCTree::traversal(coreNode *node, uint &count){
    if(node != nullptr && node->k != 0 && node->lmd != 0){
        // cout << "(k = " << node->k << " lmd = " << node->lmd << ") len = " << node->length << endl;
        count ++;
        traversal(node->left, count);
        traversal(node->right, count);
    }
}
// Test
void FCTree::getNumValidRight(coreNode *node, uint lmd, uint &num_valid){
    if(node == nullptr){
        return ;
    }
    if(lmd == node->lmd){
        num_valid ++;
        return getNumValidRight(node->left, lmd, num_valid); 
    }else if(lmd > node->lmd){
        return getNumValidRight(node->right, lmd, num_valid);
    }
}

coreNode* FCTree::getCoreByKAndLmdByLeft(coreNode *node, int k, unsigned int lmd){
    if(node == nullptr){
        return nullptr;
    }
    if(node->k == 0 && node->lmd == 0){
        return nullptr;
    }
    if(k == node->k && lmd == node->lmd){
        return node;
    }else if(k > node->k){
        return getCoreByKAndLmdByLeft(node->left, k, lmd); 
    }else if(k == node->k && lmd > node->lmd){
        return getCoreByKAndLmdByLeft(node->right, k, lmd);
    }
}

coreNode* FCTree::getCoreByKAndLmdByRight(coreNode *node, int k, unsigned int lmd){
    if(node == nullptr){
        return nullptr;
    }
    if(node->k == 0 && node->lmd == 0){
        return nullptr;
    }
    if(k == node->k && lmd == node->lmd){
        return node;
    }else if(lmd > node->lmd){
        return getCoreByKAndLmdByRight(node->right, k, lmd); 
    }else if(k > node->k && lmd == node->lmd){
        return getCoreByKAndLmdByRight(node->left, k, lmd);
    }
}

void FCTree::saveCoreToLocal(string dataset, ll_uint *id2vtx, coreNode *coreNode, string method){

    cout << coreNode << endl;
    string res_file = "../output/" + dataset + "_k_" + std::to_string(coreNode->k) + "_lmd_" + std::to_string(coreNode->lmd) +"_"+ method +".txt"; 
    cout << "res_file = " << res_file << endl;
    ofstream f_write(res_file);
    f_write << "########## k_" + std::to_string(coreNode->k) + "_lmd_" + std::to_string(coreNode->lmd) + "_length_" + std::to_string(coreNode->length) << endl;
    
    uint *core  = coreNode->core;

    for(uint i = 0; i < coreNode->length; i ++){
        f_write << id2vtx[core[i]] << endl;
    }


}