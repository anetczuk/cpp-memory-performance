## Experimets

Each experiment consists of following benchmarks:
- measuring access times of raw array (C-styled) on single thread,
- single threaded access to std::vector,
- multi-threaded access to std::vector,
- multi-processed access to std::vector,
- single threaded access to linked list,
- multi-threaded access to linked list,
- multi-processed access to linked list,

Number of threads is defined by c++ call ```std::thread::hardware_concurrency()```.

Number of processes is defined by bash command ```grep -c ^processor /proc/cpuinfo```.

Each benchmark consists of time measurements of access time to allocated structures. Measurments are taken structures of size in range from almost empty container to container of maximum given size. 


Time measurements are done using *std::chrono* high resolution clock.


## Devices

Experiments were executed on two machines: *Intel i7* and *Raspbery Pi 3*, and for two compilers: gcc and clang.

To receive detailed info about CPU run: ```lscpu```

Information about RAM: ```sudo dmidecode --type 17```

Information about OS: ```cat /etc/os-release```


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
- size: 2 x 8192MB

Compilers:
- g++ 7.4.0
- clang++ 6.0.0

OS: XUbuntu 18.04.3 LTS (Bionic Beaver)


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

OS: Raspbian 8 (jessie)


## Results per experiment

- [Intel i7](i7/README.md)
- [RPi3](rpi3/README.md)


## References:

<a name="ref01">[1]</a>: *Hayai* benchmarking library [repository](https://github.com/nickbruun/hayai)
