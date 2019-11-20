## <a name="top"></a>Results per device

- [Intel Core i7](i7/README.md#top)
- [VirtualBox on Intel Core i7](i7_vbox_1/README.md#top)
- [Raspberry Pi 3](rpi3/README.md#top)



## <a name="experiments_exp"></a>Experimets explanation

Each experiment consists of following benchmarks:
- measuring access times of raw array (C-styled) on single thread,
- single threaded access to std::vector,
- multi-threaded access to std::vector,
- multi-processed access to std::vector,
- single threaded access to linked list,
- multi-threaded access to linked list,
- multi-processed access to linked list,


All experiments share the same base pattern. It consists of:
1. allocating container of maximum given memory space
2. executing warm-up
3. executing experiments range starting from minimal container size to container of maximum given memory space
    1. each experiment consists of variable number of iterations depending on elements number to access
    2. single iteration executes access to all container's elements in sequential order in given elements number
    3. each experiment is repeated some number of times
    4. access time is calculated by dividing fastest iteration time per number of elements

Maximum given memory space is passed to experiment by command-line argument.

Multi-threaded experiments are base experiments executed in maximum allowed *hardware* threads. Multi-processed experiments are base experiments executed in parallel processes (the same number as for multi-threaded version).

Number of threads is defined by C++ call ```std::thread::hardware_concurrency()```.

Number of processes is defined by bash command ```grep -c ^processor /proc/cpuinfo```.

Time measurements are done using *std::chrono* high resolution clock.



## <a name="devices"></a>Devices under benchmark

Experiments were executed on following units: 
- *Intel Core i7*
- virtual machine on *i7*
- *Raspbery Pi 3*
and for two compilers: gcc and clang.

To receive detailed info about CPU run: ```lscpu```

Information about RAM: ```sudo dmidecode --type 17```

Information about OS: ```cat /etc/os-release```



### Intel Core i7

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



### VirtualBox on Intel Core i7

Hardware, compilers and operating system is the same as for host *Intel Core i7*. In addition, following VirtualBox settings has been made:
- 4 CPUs (4 threads)
- execution cap: 100%
- enabled PAE/NX
- RAM: 4096MB
- Guest Additions installed
- VirtualBox version 5



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

