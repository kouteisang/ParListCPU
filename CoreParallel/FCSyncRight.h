#ifndef FCSyncRight_H
#define FCSyncRight_H

#include "Graph/MultilayerGraph.h"
#include "FCCoreTree.h"


class FCSyncRight
{
private:
    /* data */
public:
    FCSyncRight(/* args */);
    ~FCSyncRight();


    static void Execute(MultilayerGraph &mg, FCCoreTree &tree);

    static void constructCoreSync(coreNodeP *node, uint k, uint lmd, uint n_vertex, uint n_layer, bool* valid, uint** degs, int total, bool serial);

    static void PeelSync(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, bool* valid, bool serial, int &total);

    static void PathSerialSync(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, bool* valid, bool serial, int &total);

    static void BuildSubFCTreeSync(FCCoreTree &tree, MultilayerGraph &mg, uint **degs, uint *klmd, coreNodeP* node, bool* valid, int &total);
// PathByK(mg, root->degs, root->k+1, root->lmd, leftChild, root->valid, root->total);
    static void PathByK(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, bool* valid, int &total);


    // The following are mix solution
    static void PathByKMix(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, bool* valid, int &total);
    static void PeelSyncMix(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, bool* valid, bool serial, int &total);
    static void ExecuteMix(MultilayerGraph &mg, FCCoreTree &tree); 
    static void BuildSubFCTreeSyncMix(FCCoreTree &tree, MultilayerGraph &mg, uint **degs, uint *klmd, coreNodeP* node, bool* valid, int &total);
    // static void BuildSubcFCTreeSync(FCCoreTree &tree, MultilayerGraph &mg, uint **degs, uint *klmd, coreNodeP* node, bool* valid, uint* invalid, uint* cnts);
};




#endif