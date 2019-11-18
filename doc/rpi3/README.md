## <a name="top"></a>Results per compiler

- [gcc](GCC.md)
- [clang](CLANG.md)



## <a name="comparison"></a>Comparison between compilers


### single-threaded array and std::vector 

Performance differs in shape of performance of *CPU* cache. Performance of *GCC* is constant and of *CLANG* is parabolic which performs faster when container size grows. In addition *GCC* code is faster. Beyond cache size peroformance is simillar.


### multi-processed std::vector

Code of both compilers perform in similar manner for all continer sizes.


### linked list for single-threaded and multi-processed version

Code of both compilers perform the same for all continer sizes.



## Conclusions

Comparison of compilers show that *GCC* performs significantly faster than *CLANG*. It can be seen in case of cache usage. In case of *RAM* access (linked list) there is no difference in timing between compilers.



