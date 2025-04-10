#ifndef FCNAIVE_H
#define FCNAIVE_H

#include "Graph/MultilayerGraph.h"
#include "Core/FCTree.h"

class FCNaive
{
private:
    /* data */
public:
    FCNaive(/* args */);
    ~FCNaive();

    static void Execute(MultilayerGraph &mg, int k, int lmd);

    static void BuildSubFCTree(MultilayerGraph &mg, uint **degs, uint *core, uint *pos, int k, int lmd, uint e, uint &count);

    static uint peel(MultilayerGraph &mg, uint **degs, int k, int lmd, uint *core, uint *pos, uint s, uint e);

    static void PrintCoreInfor(int k, int lmd, uint *core, uint new_e, uint n_vertex);

    static bool check(uint **degs, uint u, int k, int lmd, uint n_layers);
};




#endif