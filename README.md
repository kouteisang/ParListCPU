## Compile the code

```
cd Build
cmake ..
make
```

## Parameters

dataset(-d):

- example
- fao
- homo
- obamainisrael
- sacchcere
- so
- wiki
- dblp-coauthor
- flickr-growth
- amazon

method(-m):

- naive
- OptimizedLeft
- OptimizedRight
- PathParallel (Our Path Parallel strategy)
- CoreParallel (Our Core Parallel strategy)
- CoreParallelSync (Our Core Parallel algorithm reduce synchronization issue)
- CoreIndex (Baseline)
- mix (Combine Core Parallel and Path Parallel)
- CoreMem (Core Parallel with memory saving)

num of thread (-num_thread)

Order of layer (-o)

0: random order
1: order by increasing the number of edge
2: order by decreasing the number of edge
3: order by increasing the graph density
4: order by decreasing the graph density



## example

To run the homo dataset with opimized left algorithm.

```
./firmcore_baseline -d homo -m OptimizedLeft
```

To run the homo dataset with pathparallel algorithm with 10 threads.

```
./firmcore_baseline -d homo -m PathParallel -num_thread 10
```

