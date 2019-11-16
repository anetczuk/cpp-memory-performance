/// MIT License
///
/// Copyright (c) 2019 Arkadiusz Netczuk <dev.arnet@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <iostream>
#include <cmath>

#include "benchmark/benchmark_time.h"
#include "benchmark/benchmark_params.h"
#include "benchmark/benchmark_log.h"

#include "unroll.h"
#include "Array.h"


static const std::size_t BASE    = 16;
static const std::size_t STEPS   = 2;
static const std::size_t repeats = 25;
static const std::size_t DIV     = std::pow(2, STEPS-1);

// DIV = std::pow(2, exp)
// log2( DIV ) = i / DIV
// i = (STEPS-1) * std::pow(2, STEPS-1)
static const std::size_t START   = (STEPS-1) * std::pow(2, STEPS-1);


// ================================================================================


template <unsigned int S>
inline uint64_t iterate(const StdVector& container, const std::size_t listSize, const std::size_t itersnum);

template <>
inline uint64_t iterate<1>(const StdVector& container, const std::size_t listSize, const std::size_t itersnum) {
    StdVector::value_type sum = 0;

    auto startTime = MEASURE_TIME();

//    ///
//    const std::size_t acc_num = itersnum * listSize;
//    std::size_t x = 0;
//    for(std::size_t i=0; i<acc_num; ++i) {
//        if ( x == listSize)
//            x = 0;
//        sum += container[ x ];
//        ++x;
//    }

//    ///
//    const std::size_t acc_num = itersnum * listSize;
//    for(std::size_t i=0; i<acc_num; ++i) {
//        sum += container[ i % listSize ];
//    }

    ///
    for(std::size_t i=0; i<itersnum; ++i) {
        for(std::size_t x=0; x<listSize; ++x) {
            sum += container[x];
        }
    }

    auto endTime = MEASURE_TIME();
    if ( sum != 0 )  {
        std::cerr << "internal error" << std::endl;
        exit(1);
    }

    return DURATION(startTime, endTime);
    //std::chrono::nanoseconds elap = endTime - startTime;
    //return elap.count();
}

template <>
inline uint64_t iterate<2>(const StdVector& container, const std::size_t listSize, const std::size_t itersnum) {
    const std::size_t listLimit = listSize-2;               /// prevents array out of bound
    if (listLimit > listSize) {
        /// invalid state -- unsigned overflow
        return -1;
    }
    StdVector::value_type sum = 0;

    auto startTime = MEASURE_TIME();

    for(std::size_t i=0; i<itersnum; ++i) {
        for(std::size_t x=0; x<=listLimit; x+=2) {
            sum += container[x];
            sum += container[x+1];
        }
    }

    auto endTime = MEASURE_TIME();
    if ( sum != 0 )  {
        std::cerr << "internal error" << std::endl;
        exit(1);
    }

    return DURATION(startTime, endTime);
}

template <>
inline uint64_t iterate<4>(const StdVector& container, const std::size_t listSize, const std::size_t itersnum) {
    const std::size_t listLimit = listSize-4;               /// prevents array out of bound
    if (listLimit > listSize) {
        /// invalid state -- unsigned overflow
        return -1;
    }
    StdVector::value_type sum = 0;

    auto startTime = MEASURE_TIME();

    for(std::size_t i=0; i<itersnum; ++i) {
        for(std::size_t x=0; x<=listLimit; x+=4) {
            sum += container[x];
            sum += container[x+1];
            sum += container[x+2];
            sum += container[x+3];
        }
    }

    auto endTime = MEASURE_TIME();
    if ( sum != 0 )  {
        std::cerr << "internal error" << std::endl;
        exit(1);
    }

    return DURATION(startTime, endTime);
}

template <>
inline uint64_t iterate<8>(const StdVector& container, const std::size_t listSize, const std::size_t itersnum) {
    const std::size_t listLimit = listSize-8;               /// prevents array out of bound
    if (listLimit > listSize) {
        /// invalid state -- unsigned overflow
        return -1;
    }
    StdVector::value_type sum = 0;

    auto startTime = MEASURE_TIME();

    for(std::size_t i=0; i<itersnum; ++i) {
        for(std::size_t x=0; x<=listLimit; x+=8) {
            sum += container[x];
            sum += container[x+1];
            sum += container[x+2];
            sum += container[x+3];
            sum += container[x+4];
            sum += container[x+5];
            sum += container[x+6];
            sum += container[x+7];
        }
    }

    auto endTime = MEASURE_TIME();
    if ( sum != 0 )  {
        std::cerr << "internal error" << std::endl;
        exit(1);
    }

    return DURATION(startTime, endTime);
}

template <>
inline uint64_t iterate<16>(const StdVector& container, const std::size_t listSize, const std::size_t itersnum) {
    const std::size_t listLimit = listSize-16;          /// prevents array out of bound
    if (listLimit > listSize) {
        /// invalid state -- unsigned overflow
        return -1;
    }
    StdVector::value_type sum = 0;

    auto startTime = MEASURE_TIME();

    for(std::size_t i=0; i<itersnum; ++i) {
        for(std::size_t x=0; x<=listLimit; x+=16) {
            sum += container[x];
            sum += container[x+1];
            sum += container[x+2];
            sum += container[x+3];
            sum += container[x+4];
            sum += container[x+5];
            sum += container[x+6];
            sum += container[x+7];

            sum += container[x+ 9];
            sum += container[x+10];
            sum += container[x+11];
            sum += container[x+12];
            sum += container[x+13];
            sum += container[x+14];
            sum += container[x+15];
        }
    }

    auto endTime = MEASURE_TIME();
    if ( sum != 0 )  {
        std::cerr << "internal error" << std::endl;
        exit(1);
    }

    return DURATION(startTime, endTime);
}

void singleLoop(const StdVector& vector, const std::size_t listSize, const std::size_t i, const std::size_t maxiters) {
    const std::size_t maxListSize = vector.size();

    const double iterFactor = 3.0 * i / maxiters + 1;
    const std::size_t itersNum = iterFactor * maxListSize / listSize + 1;

    uint64_t bestDur = -1;
    for(std::size_t r=0; r<repeats; ++r) {
        const uint64_t dur = iterate<UNROLL>(vector, listSize, itersNum);
        if (dur < bestDur)
            bestDur = dur;
    }

    const double timePerIter = double(bestDur) / itersNum;
    const double timePerElem = timePerIter / listSize;

    const std::size_t acc_num = listSize * itersNum;
    std::cerr << std::fixed << i << " / " << maxiters << " iters: " << itersNum << " listSize: " << listSize << " repeats: " << repeats << " accnum: " << acc_num << std::endl;

    const std::size_t currMemSize = listSize * sizeof( StdVector::value_type ) + sizeof( StdVector );
    std::cout << std::fixed << currMemSize << " B " << "0 B" << " ";
    std::cout << std::fixed << "time/iter: " << timePerIter << " ns time/item: " << timePerElem << " ns iters: " << itersNum << " items: " << listSize << std::endl;
}


// ================================================================================


int main(int argc, char** argv) {
	std::size_t memSizeB = 128*1024*1024L;
	const long long mem = benchmark::get_param_maxmem(argc, argv);
	if (mem < 1) {
	    BUFFERED( std::cerr, "invalid maxmem argument\n" );
		exit(1);
	}
	memSizeB = mem;

	const std::size_t maxListSize = memSizeB / sizeof( StdVector::value_type );
	StdVector vector(maxListSize, 0);

	const std::size_t maxiters = benchmark::LogExperimentFunctor::calcLog(maxListSize, BASE, DIV) + 1;

	for(std::size_t i=START; i<maxiters; ++i) {
	    const std::size_t exp  = i / DIV;
	    const std::size_t mod  = i % DIV;
	    const std::size_t mul  = std::pow(2, exp);
	    const std::size_t currSize = BASE * mul / DIV * (DIV + mod);

	    singleLoop(vector, currSize, i, maxiters);
	}

	singleLoop(vector, maxListSize, maxiters, maxiters);

    return 0;
}
