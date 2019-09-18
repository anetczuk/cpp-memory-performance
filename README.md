# Measuring memory performance

Project presents results of various memory benchmarks.


## Device

To receive detailed info about CPU run: ```lscpu```

To receive detailed info about CPU run: ```sudo dmidecode --type 17```

### Intel i7
All measurements have been done on following unit: 

CPU:
- Intel(R) Core(TM) i7-8750H CPU @ 2.20GHz
- 6 cores, 2 threads per core
- L1 cache: 32KB per core (separated for data and for instructions)
- L2 cache: 256KB per core
- L3 cache: 9MB SmartCache (Smart Cache refers to the architecture that allows all cores to dynamically share access to the last level cache.)

RAM:
- type: DDR4
- speed: 2667 MT/s
- size: 2x8192MB

Compilers:
- g++ 7.4.0
- clang++ 6.0.0


### Raspberry Pi 3 Model B Rev 1.2

CPU:
- Broadcom BCM2837 SoC with ARMv7 Processor rev 4 (v7l) - ARM Cortex-A53 @ 1200 MHz
- 4 cores, 1 thread per core
- L1 cache: 32kB
- L2 cache: 512kB
- L3 cache: none

RAM:
- type: SDRAM
- speed: 900 MHz
- size: 1 GB (shared by the GPU and CPU)

Compilers:
- g++ 4.9.2
- clang++ 3.5.0


## Benchmarking

Time measurements is done using *Hayai* library. It can be found [here [1]](#ref01).

In addition, one function has been taken from [*Google Benchmark*](#ref02) library (preventing compiler to opt out unnecessary code). Code is placed in *src/benchmark/prevent_optimization.h*.


## Compilation and execution

To compile all exercises execute following sequence:
1. create build directory
2. go to build directory
3. configure: ```cmake -D CMAKE_BUILD_TYPE=Release /abs/path/to/src```
4. compile: ```make```
5. (optional) run tests by: ```ctest```
6. execute benchmarks: ```./mem_performance/plot_all.sh```

Configuration can be done also by one of preconfigured scripts located in *configure* direcotry.


## References:

<a name="ref01">[1]</a>: *Hayai* benchmarking library [repository](https://github.com/nickbruun/hayai)

<a name="ref01">[2]</a>:: *Google Benchmark* library [repository](https://github.com/google/benchmark)


