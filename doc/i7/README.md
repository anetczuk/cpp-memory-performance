## <a name="top"></a>Results per compiler

- [gcc](GCC.md#top)
- [clang](CLANG.md#top)



## <a name="comparison"></a>Comparison between compilers


### single-threaded array and std::vector 

![Intel Core i7 compilers comparison](vector_st_comparison.png "Intel Core i7 compilers comparison")

Performance differs in two aspects:
- shape of performance of *L1* cache
- performance of rest of chart

In first case performance of *GCC* is constant and of *CLANG* is parabolic which performs faster when container size grows. In second case curve shape is the same, but overal performance of *GCC* is better than performance of *CLANG*. For container 64KB timing is 0.11 ns to 0.14 ns. For container of 128MB *GCC* timing is 0.24 ns to 0.37 ns of *CLANG* timing.


### multi-threaded and multi-processed std::vector

Code performance of both compilers up to 1MB containers is similar. Beyond this size *GCC* performs significantly faster (1.6 ns for 8MB) than *CLANG* (3.0 ns for 8MB).


### linked list for single-threaded, multi-threaded and multi-processed version

Code of both compilers perform the same for all continer sizes.



## Conclusions

Comparison of compilers show that *GCC* performs significantly faster than *CLANG*. It can be seen in case of cache usage. In case of *RAM* access (linked list) there is no difference in timing between compilers.



