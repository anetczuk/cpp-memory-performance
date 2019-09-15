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

#include "benchmark/benchmark_time.h"
#include "benchmark/benchmark_params.h"

#include "array.h"


inline void do_benchmark(const StdVector& container, const std::size_t memSizeB, const std::size_t itersnum) {
	const std::size_t listSize = memSizeB / sizeof( StdVector::value_type );
	if (container.size() < listSize) {
		return ;
	}
	benchmark::Clock::TimePoint startTime, endTime;

	static const std::size_t repeats = 5;
	uint64_t bestDur = -1;
	for (std::size_t r=0; r<repeats; ++r) {
		StdVector::value_type sum = 0;
		startTime = benchmark::Clock::Now();

		for(std::size_t i=0; i<itersnum; ++i) {
			for(std::size_t x=0; x<listSize; ++x) {
				sum += container[x];
			}
		}

		endTime = benchmark::Clock::Now();
		if ( sum != listSize*itersnum )  {
			std::cerr << "internal error" << std::endl;
			exit(1);
		}
		const uint64_t duration = benchmark::Clock::Duration(startTime, endTime);
		bestDur = std::min(bestDur, duration);
	}


    const double timePerIter = double(bestDur) / itersnum;
    const double timePerElem = timePerIter / listSize;

    std::cerr << std::fixed << " iters: " << itersnum << " repeats: " << repeats << " memory: " << memSizeB << " B " << std::endl;

    std::cout << std::fixed << memSizeB << " B " << "0 B" << " ";
    std::cout << std::fixed << "time/iter: " << timePerIter << " ns time/item: " << timePerElem << " ns iters: " << itersnum << " items: " << listSize << std::endl;
}


int main(int argc, char** argv) {
	std::size_t memSizeB = 128*1024*1024L;
	const long long mem = benchmark::get_param_maxmem(argc, argv);
	if (mem > 0) {
		memSizeB = mem;
	}

	const std::size_t vSize = memSizeB / sizeof( StdVector::value_type );
	StdVector vector(vSize, 1);

	do_benchmark( vector, 16*1024*1024,   100 );
	do_benchmark( vector,  8*1024*1024,   100 );
	do_benchmark( vector,  4*1024*1024,  1000 );
	do_benchmark( vector,  2*1024*1024,  1000 );

	do_benchmark( vector,    1024*1024,  1000 );
	do_benchmark( vector,     512*1024,  1000 );
	do_benchmark( vector,     256*1024,  1000 );
	do_benchmark( vector,     128*1024, 10000 );

	do_benchmark( vector,      64*1024, 10000 );
	do_benchmark( vector,      32*1024, 10000 );
	do_benchmark( vector,      16*1024, 10000 );
	do_benchmark( vector,       8*1024, 10000 );

	do_benchmark( vector,       4*1024, 10000 );
	do_benchmark( vector,       2*1024, 10000 );
	do_benchmark( vector,         1024, 10000 );
	do_benchmark( vector,          512, 10000 );

	do_benchmark( vector,          256, 10000 );
	do_benchmark( vector,          128, 10000 );
	do_benchmark( vector,           64, 10000 );
	do_benchmark( vector,           32, 10000 );

    return 0;
}
