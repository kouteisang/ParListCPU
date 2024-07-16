#include "FCTree.h"



FCTree::~FCTree() {
    // deleteNode(node);
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