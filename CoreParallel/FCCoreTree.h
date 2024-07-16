#ifndef FCTreeCore_H
#define FCTreeCore_H

#include "Graph/MultilayerGraph.h"

// parallel core node
struct coreNodeP{

    unsigned int k{0};
    unsigned int lmd{0};
    unsigned int length{0};
    uint** degs{nullptr};
    uint *core{nullptr}; // result core
    bool *valid{nullptr}; // valid vertex
    uint* invalid{nullptr};

    uint e{0}; // e is the start point

    coreNodeP* left;
    coreNodeP* right;

    coreNodeP(){};

    coreNodeP(unsigned int _k, unsigned int _lmd, unsigned int _length){
        k = _k;
        lmd = _lmd;
        length = _length;
    };
     
};



class FCCoreTree
{
private:
    coreNodeP *node;
public:
    FCCoreTree(/* args */);
    ~FCCoreTree();


    FCCoreTree(unsigned int k, unsigned int lmd, unsigned int length){
       node = new coreNodeP(k, lmd, length);
    }

    // get the root Node
    inline coreNodeP* getNode() const {
        return node;
    }
};






#endif

