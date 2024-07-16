#ifndef FCTreeBuilderCoreParallel_H
#define FCTreeBuilderCoreParallel_H

#include "Graph/MultilayerGraph.h"
#include "FCCoreTree.h"

class FCTreeBuilderCoreParallel
{
private:
    /* data */
public:
    FCTreeBuilderCoreParallel(/* args */);
    ~FCTreeBuilderCoreParallel();

    static void Execute(MultilayerGraph &mg, FCCoreTree &tree, ll_uint *id2vtx);
    
    static void BuildSubFCTree(MultilayerGraph &mg, uint **degs, uint *klmd, coreNodeP* node, uint* valid, uint* invalid, uint* cnts, ll_uint *id2vtx);

    static bool check(uint **degs, uint u, uint k, uint lmd, uint n_layers);

    static void PathSerial(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, uint* valid, uint* invalid, uint* cnts, ll_uint *id2vtx);

    // Core Parallel to Peel invalid vertex
    static uint PeelInvalidInParallelByCount(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, uint* valid, uint* invalid, uint* cnts, ll_uint *id2vtx, uint e);

    static bool PeelInvalidInParallelByCheck(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, uint* valid, uint* invalid, uint* cnts, ll_uint *id2vtx);

};



#endif