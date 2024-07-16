#ifndef FCTreeBuilderCoreParallel_H
#define FCTreeBuilderCoreParallel_H

#include "Graph/MultilayerGraph.h"
#include "FCTree.h"

class FCTreeBuilderCoreParallel
{
private:
    /* data */
public:
    FCTreeBuilderCoreParallel(/* args */);
    ~FCTreeBuilderCoreParallel();

    static void Execute(MultilayerGraph &mg, FCTree &tree, ll_uint *id2vtx);
    
    static void BuildSubFCTree(MultilayerGraph &mg, uint **degs, uint *klmd, coreNode* node, uint* valid, uint* invalid, uint* cnts, ll_uint *id2vtx);

    static bool PeelInvalidInParallelByCount(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNode* node, uint* valid, uint* invalid, uint* cnts, ll_uint *id2vtx);

    static bool PeelInvalidInParallelByCheck(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNode* node, uint* valid, uint* invalid, uint* cnts, ll_uint *id2vtx);

    static bool check(uint **degs, uint u, uint k, uint lmd, uint n_layers);
};



#endif