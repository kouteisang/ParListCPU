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
    
    static void BuildSubFCTree(MultilayerGraph &mg, uint **degs, uint *klmd, coreNodeP* node, bool* valid, uint* invalid, uint* cnts, ll_uint *id2vtx);

   
    static void PathSerial(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, bool* valid, uint* invalid, uint* cnts, ll_uint *id2vtx, uint e);

    // Core Parallel to Peel invalid vertex
    static uint PeelInvalidInParallelByCount(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, bool* valid, uint* invalid, uint* cnts, ll_uint *id2vtx, uint e, bool serial);

    static bool PeelInvalidInParallelByCheck(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, bool* valid, uint* invalid, uint* cnts, ll_uint *id2vtx);

    // useful common method
    static bool check(uint **degs, uint u, uint k, uint lmd, uint n_layers);

    static void constructCore(coreNodeP *node, uint k, uint lmd, uint new_e, uint n_vertex, uint n_layer, bool* valid, uint* invalid, uint** degs, bool serial);


};



#endif