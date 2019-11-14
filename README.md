# Measuring memory performance

Project presents results of various memory benchmarks.
Benchmarks are done for processor cache (L1, L2, L3) and for RAM memory.


## Results

Detailed results can be found [here](doc/README.md)


## Compilation and execution

To compile all exercises execute following sequence:
1. create build directory
2. go to build directory
3. configure: ```cmake -D CMAKE_BUILD_TYPE=Release /abs/path/to/src```
4. compile: ```make```
5. (optional) run tests by: ```ctest```
6. execute benchmarks: ```./mem_performance/plot_all.sh --maxmem 1.6G```
7. examine results stored in directory ```./mem_performance/data```

Configuration can be done also by one of preconfigured scripts located in *configure* direcotry.


## References:

<a name="ref01">[1]</a>: *Hayai* benchmarking library [repository](https://github.com/nickbruun/hayai)
