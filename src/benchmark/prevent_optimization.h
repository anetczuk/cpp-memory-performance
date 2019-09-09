/**
 * This code parts are taken form Google Benchmark library:
 *       https://github.com/google/benchmark
 */

#ifndef SRC_BENCHMARK_PREVENT_OPTIMIZATION_H_
#define SRC_BENCHMARK_PREVENT_OPTIMIZATION_H_


#if defined(__GNUC__)
    #define BENCHMARK_ALWAYS_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER) && !defined(__clang__)
    #define BENCHMARK_ALWAYS_INLINE __forceinline
#else
    #define BENCHMARK_ALWAYS_INLINE
#endif


// The DoNotOptimize(...) function can be used to prevent a value or
// expression from being optimized away by the compiler. This function is
// intended to add little to no overhead.
// See: https://youtu.be/nXaxk27zwlk?t=2441
template <class Tp>
inline BENCHMARK_ALWAYS_INLINE void DoNotOptimize(Tp const& value) {
  asm volatile("" : : "r,m"(value) : "memory");
}

template <class Tp>
inline BENCHMARK_ALWAYS_INLINE void DoNotOptimize(Tp& value) {
#if defined(__clang__)
  asm volatile("" : "+r,m"(value) : : "memory");
#else
  asm volatile("" : "+m,r"(value) : : "memory");
#endif
}


#endif /* SRC_BENCHMARK_PREVENT_OPTIMIZATION_H_ */
