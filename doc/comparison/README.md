## <a name="top"></a> Comparison


### Performance cost of Virtual Box

Performance const can be measured by comparing timing charts of experiments executed on host machine with results of executions experiments on virtualized environments.

![Comparison of single-thread std::vector experiments](i7-i7_vbox_1/clang/vector_st_data_plot.png "Comparison of single-thread std::vector experiments")

![Comparison of single-thread linked list experiments](i7-i7_vbox_1/clang/cllist_st_data_plot.png "Comparison of single-thread linked list experiments")

The graphs shown present performance of std::vector and linked list on single thread under clang. Results of std::vector no performance penalties on both: cache and *RAM*. Situation changes on linked list. Beyond cache size there is increased use of *RAM*, so performance on virtual environment decreases significantly. This observation leads to conclussion that on virtualized environment access to cache is costless, but access to *RAM* is burdened significantly.


### Cache influence on i7

![i7 performance of single-thread std::vector](i7/vector_st_data_plot_comparison.png "i7 performance of single-thread std::vector")

Above chart presents preformance of std::vector under various compilations. Under *gcc unroll* and *clang* compilations there can be seen clear difference between cache levels, it is:
- *L1* cache up to 32 KB
- *L2* cache up to 256 KB
- *L3* cache up to 9 MB

*L1* cache is twice times faster than other cache levels.

In case of *gcc* and *clang unroll* there is no difference between cache levels.


![i7 performance of single-thread linked list](i7/cllist_st_data_plot_comparison_log.png "i7 performance of single-thread linked list")

Results of linked list shows significant benefits of using cache. For small lists (up to 128 KB) performance is simillar to std::vector. Beyond point of 2 MB performance highly depends on *RAM*. 


#### Impact on multiple threads and processes

![i7 performance of multi-thread std::vector](i7/vector_mt_data_plot_average_comparison.png "i7 performance of multi-thread std::vector")

![i7 performance of multi-thread linked list](i7/cllist_mt_data_plot_average_comparison.png "i7 performance of multi-thread linked list")

Shape of presented curves is imillar to curves of single-threaded variants. The difference is in size-values of change points. For std::vector cache improves performance of containers of size up to 1 MB. For linked lists this container size limit is 16 KB.


### Cache influence on Raspberry Pi

![Raspberry Pi performance of single-thread std::vector](rpi3/vector_st_data_plot_comparison.png "Raspberry Pi performance of single-thread std::vector")

Above chart presents preformance of std::vector under various compilations. There can be seen clear difference between cache and *RAM*, but difference between *L1* and *L2* is hard to notice.


![Raspberry Pi performance of single-thread linked list](rpi3/cllist_st_data_plot_comparison_log.png "Raspberry Pi performance of single-thread linked list")

Results of linked list shows significant benefits of using cache. Beyond cache size performance highly depends on *RAM*. 


#### Impact on multiple threads and processes

![Raspberry Pi performance of multi-process std::vector](rpi3/vector_mp_data_plot_average_comparison.png "Raspberry Pi performance of multi-process std::vector")

![Raspberry Pi performance of multi-process linked list](rpi3/cllist_mp_data_plot_average_comparison.png "Raspberry Pi performance of multi-process linked list")

Shape of presented curves is imillar to curves of single-threaded variants. The difference is in size-values of change points. For std::vector cache improves performance of containers of size up to 256 KB. For linked lists this container size limit is 16 KB.

