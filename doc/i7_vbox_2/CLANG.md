## <a name="top"></a>single-threaded raw array

Performance of access time to raw array is presented in following image.

![Performance of raw array](clang/plot_array_st.png "Performance of raw array")

There are three plateau regions corresponding to levels of cache:
- *L1* with fastest access time around 0.07 ns per item up to 32KB of array size
- *L2* with access time around 0.14 ns per item in memory range between 32KB and 256KB
- *L3* with access time around 0.17 ns per item in memory range between 256KB and 4MB
Starting from 9MB access to items involves use of *RAM*, so it can be seen on chart in form of worse performance. *L1* cache performance for very small contaiers up to 1KB is significantly worse than for 32KB containers.

In addition, performance gain between *L2* - *L3* cache is minimal, but between *L1* - *L2* and *L3* - *RAM* is significant.



## single-threaded std::vector

![Performance of std::vector](clang/plot_vector_st.png "Performance of std::vector")

Performance chart and observations are similar to *single-threaded raw array* experiment.



## multi-threaded std::vector

![Performance of std::vector](clang/plot_vector_mt.png "Performance of std::vector")

The chart resembles curve from chart of single-threaded std::vector experiment. The difference is in slightly worse performance of *L2* and *L3* cache levels, and significant performance drop of *RAM* witch starts earlier.

When container reaches 2MB of size there is dramatic perfrmance loss due to increased cache coruption because of thread switching. Performance difference between *L1*, *L2* and *L3* cache levels is marginal in comparison of inefficient timing of *RAM*. 



## multi-processed std::vector

![Performance of std::vector](clang/plot_vector_mp.png "Performance of std::vector")

Performance chart and observations are similar to *multi-threaded std::vector* experiment. Timing difference is hard to notice.



## single-threaded linked list

![Performance of linked list](clang/plot_cllist_st.png "Performance of linked list")

Performance chart demonstrates three regions:
1. linearly degrading timing of cache up to 2MB
2. constant timing of *RAM* up to 64MB
3. constant timing of *RAM* beyond 128MB

Region no 2. and 3. in comparison with performance of *single-threaded std::vector* means existence of optimisations in access to *RAM* in case of sizes slightly greater than *L3* cache. It could be sequential access to chunks of memory or influence of dual-channel configuration.



## multi-threaded linked list

![Performance of std::vector](clang/plot_cllist_mt.png "Performance of linked list")

Performance chart and observations are similar to *single-threaded linked list* experiment. It proofs no influence of *CPU* cache structures. In comparison with *single-threaded* experiment region no 2. ends around container size of 16MB. 



## multi-processed linked list

![Performance of std::vector](clang/plot_cllist_mp.png "Performance of linked list")

Performance chart and observations are similar to *multi-threaded linked list* experiment. Timing difference is hard to notice.



## Conclusions

1. std::vector performs the same as raw array
2. *L1* cache performs significantly better than *L2*
3. *L1* cache performs the same as *L2* for very small containers
4. *L3* performs almost the same as *L2*
5. there is no difference between results of multi-threaded and multi-processed benchmarks
6. there is no difference between results of single-threaded, multi-threaded and multi-processed benchmarks of randomized linked list
