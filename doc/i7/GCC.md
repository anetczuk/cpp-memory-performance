## single-threaded raw array

Performance of access time to raw array is presented in following image.

![Performance of raw array](gcc/plot_array_st.png "Performance of raw array")

It can be clearly seen three plateaus regions corresponding to levels of cache:
- *L1* with fastest access time around 0.07 ns per item up to 32KB of array size
- *L2* with access time around 0.11 ns per item in memory range between 32KB and 256KB
- *L3* with access time around 0.12 ns per item in memory range between 256KB and 4MB
Starting from 9MB access to items involves use of RAM, so it can be seen on chart in form of worse performance.

In addition, performance gain between *L2* - *L3* cache is minimal, but between *L1* - *L2* and *L3* - *RAM* is significant.



## single-threaded std::vector

![Performance of std::vector](gcc/plot_vector_st.png "Performance of std::vector")

Performance chart and observations are similar to *single-threaded raw array* experiment. Timing difference is very hard to notice.



## multi-threaded std::vector

![Performance of std::vector](gcc/plot_vector_mt.png "Performance of std::vector")

In multi-threaded experiment chart of std::vector exists two regions:
- region up to 1MB of container size
- region starting from 1MB

When container reaches 1MB of size there is dramatic perfrmance loss due to increased cache coruption due to thread switching. Performance difference between *L1*, *L2* and *L3* cache levels is marginal in comparison of inefficient timing of *RAM*. 



## multi-processed std::vector

![Performance of std::vector](gcc/plot_vector_mp.png "Performance of std::vector")

Performance chart and observations are similar to *multi-threaded std::vector* experiment. Timing difference is very hard to notice.



## single-threaded linked list

![Performance of linked list](gcc/plot_cllist_st.png "Performance of linked list")

Performance chart demonstrates three regions:
1. linearly degrading timing of cache up to 2MB
2. constant timing of *RAM* up to 64MB
3. constant timing of *RAM* beyond 128MB

Region no 2. and 3. in comparison with performance of *single-threaded std::vector* means existence of optimisations in access to *RAM* in case of sizes slightly greater than *L3* cache. It could be sequential access to chunks of memory or influence of dual-channel configuration.



## multi-threaded linked list

![Performance of std::vector](gcc/plot_cllist_mt.png "Performance of linked list")

Performance chart and observations are similar to *single-threaded linked list* experiment. It proofs no influence of *CPU* cache structures. In comparison with *single-threaded* experiment region no 2. ends around container size of 16MB. 



## multi-processed linked list

![Performance of std::vector](gcc/plot_cllist_mp.png "Performance of linked list")

Performance chart and observations are similar to *multi-threaded linked list* experiment. Timing difference is very hard to notice.


