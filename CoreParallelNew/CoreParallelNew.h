#ifndef CoreParallelNew_H
#define CoreParallelNew_H

#include "Graph/MultilayerGraph.h"
#include "CoreParallel/FCCoreTree.h"

class CoreParallelNew{

private:
/* data */
public:
    CoreParallelNew();
    ~CoreParallelNew();

    static void Execute(MultilayerGraph &mg, FCCoreTree &tree); 

    static void BuildSubFCTree(FCCoreTree &tree, MultilayerGraph &mg, uint **degs, uint *klmd, uint* valid, uint* cnts);

    static void PathSerial(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, uint* valid, uint* cnts, int invalidIndex);

    static bool PeelInvalidInParallel(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, uint* valid, uint* cnts, int invalidIndex, bool serial);

    static void constructCore(coreNodeP *node, uint k, uint lmd,  uint n_vertex, uint n_layer, uint* valid, uint** degs, uint* cnts, int invalidIndex, bool serial);

    static void PathLeft(MultilayerGraph &mg, uint **degs, uint k, uint lmd, coreNodeP* node, uint* valid, uint* cnts, int invalidIndex);

};


#endif