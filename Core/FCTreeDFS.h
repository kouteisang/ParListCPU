#ifndef FCTreeDFS_H
#define FCTreeDFS_H

#include "Graph/MultilayerGraph.h"

class FCTreeDFS{
private:
    /* data */
public:
    FCTreeDFS(/* args */);
    ~FCTreeDFS();

    static void Execute(MultilayerGraph &mg);

    static void dfs(MultilayerGraph &mg, uint **dges, uint *klmd, uint *core, uint *pos, uint e, uint &count);

    static uint peel(MultilayerGraph &mg, uint **degs, uint *klmd, uint *core, uint *pos, uint s, uint e);

    static bool check(uint **degs, uint u, uint *klmd, uint n_layers);

    static void restore(MultilayerGraph &mg, uint **degs, uint *core, uint old_e, uint new_e);

    static void PrintCoreInfor(uint *klmd, uint *core, uint new_e, uint n_vertex);

};






#endif



