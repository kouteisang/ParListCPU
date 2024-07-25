# fctree

## To run the code

```
cd Build
make
./firmcore_baseline -d dataset -m method
```

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

method(-m):

- serial (FirmCore Tree based solution, including buiding the tree structure, i.e, FCTree)
- dfs (dfs version, go through all the nodes)
- pathk (path level parallization by k)
- pathlmd (path level parallization by lmd)
- core (core level parallization)

num_thread(-t): 

Number of thread going to use 

k(-k), only in the serial, parallel version can use:

- k: specify the output k

m(-m), only in the serial, parallel version can use:

- lmd: specify the output lambda

- core: each firmcore decomposition parallel
