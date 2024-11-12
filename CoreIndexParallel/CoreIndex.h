#ifndef CoreIndex_H
#define CoreIndex_H

#include "Graph/MultilayerGraph.h"


class CoreIndex
{
private:
    /* data */
public:
    CoreIndex(/* args */);
    ~CoreIndex();

    static void Execute(MultilayerGraph &mg, ll_uint *id2vtx);

    static void WdsCoreIndex(MultilayerGraph &mg, ll_uint *id2vtx, float beta);

};



#endif