#ifndef FCTreeBuilderLeft_H
#define FCTreeBuilderLeft_H

#include "Graph/MultilayerGraph.h"
#include "Core/FCTree.h"

class FCTreeBuilderLeft{
private:
    /* data */
public:
    FCTreeBuilderLeft(/* args */);
    ~FCTreeBuilderLeft();

    static void Execute(MultilayerGraph &mg);

    static void Execute(MultilayerGraph &mg, FCTree &tree);

    static void BuildSubFCTree(MultilayerGraph &mg, uint **degs, uint *core, uint *pos, uint *klmd, uint e, uint &count);

    static void BuildSubFCTree(MultilayerGraph &mg, uint **degs, uint *core, uint *pos, uint *klmd, uint e, uint &count, coreNode* node, coreNode* father);

    static uint peel(MultilayerGraph &mg, uint **degs, uint *klmd, uint *core, uint *pos, uint s, uint e);

    static bool check(uint **degs, uint u, uint *klmd, uint n_layers);

    static void restore(MultilayerGraph &mg, uint **degs, uint *core, uint old_e, uint new_e);

    static void PrintCoreInfor(uint *klmd, uint *core, uint new_e, uint n_vertex);

    static void constructCore(uint *klmd, uint *core, uint new_e, uint n_vertex, coreNode *node, coreNode* father);
};




#endif
