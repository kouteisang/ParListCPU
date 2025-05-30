# Swift FirmCore Decomposition on Billion-scale Multilayer Graphs

## Abstract

Multilayer (ML) graphs offer a convenient paradigm for modeling complex node-to-node interactions, such as social or semantic connections, as layers of a graph. FirmCore decomposition aims to identify groups of nodes with strong ties across layers to support clustering and community detection in ML graphs. Yet, the best FirmCore decomposition algorithm requires hours of computation on large graphs with millions of nodes and billions of edges, hindering its applicability in practical scenarios. To overcome this, we propose a novel list-based structure, FC-List, to boost the efficiency of FirmCore decomposition by exploiting redundancies in the computation of progressively smaller cores. Since the list-based structure is naturally amenable to parallelism, we introduce three new parallel algorithms on multi-core CPU, ParList, ParCore, and ParHybrid. We further leverage GPU architecture and propose a novel GPU solution ParCore-GPU to further boost the efficiency. In extensive experiments on 12 datasets, FC-List achieves 9x speedup on average in the serial version and more than two orders of magnitude improvement in the parallel version and memory footprint reduction up to 28.5x compared to existing methods. Furthermore, our GPU algorithm gains over three orders of magnitude speedup. Applied to the challenging NP-hard densest subgraph mining in ML graphs, our algorithms achieve up to 436x speedup.


## Compile the code

```
cd Build
cmake ..
make
```

## Parameters

dataset(-d):


- homo
- sacchcere
- fao
- internet
- wiki
- obamainisrael
- amazon
- dblp-coauthor
- flickr-growth
- so
- hw1
- hw2


method(-m):

- lmdlist ($\lambda$-list)
- klist ($k$-list)
- fclist (FC-List)
- ParCore (ParCore algorithm)
- ParList (ParList)
- CoreIndex (Baseline)
- hybrid (Combine ParCore and ParList)
- CoreMem (ParCore with memory saving strategy)
- WdsList (densest subgraph mining)

num of thread (-num_thread)

Order of layer (-o)

- 0: random order
- 1: order by increasing the number of edge
- 2: order by decreasing the number of edge
- 3: order by increasing the graph density
- 4: order by decreasing the graph density



## example

To run the homo dataset with ParCore algorithm with 2 threads.

```
./firmcore_baseline -d homo -m ParCore -num_thread 2
```
