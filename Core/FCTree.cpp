#include "FCTree.h"



FCTree::~FCTree() {
    // deleteNode(node);
}

coreNode* FCTree::getCoreByKAndLmd(coreNode *node, int k, unsigned int lmd){
    if(node->k == 0 && node->lmd == 0){
        return nullptr;
    }
    if(k == node->k && lmd == node->lmd){
        return node;
    }else if(k > node->k){
        return getCoreByKAndLmd(node->left, k, lmd); 
    }else if(k == node->k && lmd > node->lmd){
        return getCoreByKAndLmd(node->right, k, lmd);
    }
}

void FCTree::saveCoreToLocal(string dataset, ll_uint *id2vtx, coreNode *coreNode){

    cout << coreNode << endl;

    string res_file = "../output/" + dataset + "_k_" + std::to_string(coreNode->k) + "_lmd_" + std::to_string(coreNode->lmd) + ".txt"; 
    cout << "res_file = " << res_file << endl;
    ofstream f_write(res_file);
    f_write << "########## k_" + std::to_string(coreNode->k) + "_lmd_" + std::to_string(coreNode->lmd) + "_length_" + std::to_string(coreNode->length) << endl;
    
    uint *core  = coreNode->core;

    for(uint i = 0; i < coreNode->length; i ++){
        f_write << id2vtx[core[i]] << endl;
    }


}