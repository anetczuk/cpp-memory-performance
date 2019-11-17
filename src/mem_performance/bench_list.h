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

#ifndef MEM_PERFORMANCE_BENCH_LIST_H_
#define MEM_PERFORMANCE_BENCH_LIST_H_

#include <iomanip>

#include "benchmark/benchmark.h"
#include "benchmark/benchmark_log.h"
#include "benchmark/benchmark_time.h"
#include "benchmark/benchmark_params.h"


template <typename BType>
inline uint64_t bench_iteration(const typename BType::value_type* list, const std::size_t listSize, const std::size_t itersnum) {
	auto startTime = MEASURE_TIME();

	for(std::size_t i = 0; i<itersnum; ++i) {
		const typename BType::value_type* elem = list;
		uint64_t sum = 0;
		while( elem != nullptr && sum < listSize ) {
			elem = elem->next;
			++sum;
		}
		if ( sum != listSize )  {
		    std::cerr << STRINGIZE_STREAM( "internal error: " << sum << " " << listSize << "\n" );
			exit(1);
		}
	}

	auto endTime = MEASURE_TIME();
	return DURATION(startTime, endTime);
}


template <typename BType>
class ListExperiment: public benchmark::ContainerExperiment {
public:

	benchmark::LogExperimentFunctor2 logFunctor;
	std::size_t expsNumber;
	BType container;

	std::size_t DATA_SIZE;
	std::size_t CONTAINER_SIZE;


	ListExperiment(): benchmark::ContainerExperiment(),
			logFunctor(),
			expsNumber(0), container(),
			DATA_SIZE( sizeof(typename BType::value_type) ), CONTAINER_SIZE( sizeof(BType) )
	{
	}

	~ListExperiment() override {
	}

	void initialize() {
	    const uint64_t memSize = logFunctor.getMemorySize();

		const std::size_t listSize = calcContainerSize( memSize );

	    std::cerr << STRINGIZE_STREAM( "initializing memory: " << memSize << " (" << std::fixed << std::setw( 6 ) << ( double(memSize) / (1024*1024*1024)) << " GB), container size: " << listSize << "\n" );

		expsNumber = benchmark::LogExperimentFunctor::calcLog(listSize, BASE, DIV) + 1;
		container = BType( listSize );
		container.randomize();
	}

	void parseArguments(int argc, char** argv) {
        const long long strictmem = benchmark::get_param_mem(argc, argv);
        if (strictmem > 0) {
            logFunctor.strictSizeB = strictmem;
        } else {
            const long long maxmem = benchmark::get_param_maxmem(argc, argv);
            if (maxmem < 1) {
                std::cerr << STRINGIZE_STREAM( "'mem' or 'maxmem' argument not given\n" );
                exit(1);
            }
            logFunctor.maxSizeB = maxmem;
        }

        const long long memdiv = benchmark::get_param_long(argc, argv, "memdiv");
        if (memdiv > 0) {
            logFunctor.divideMemory( memdiv );
        }
	}

	void run(std::ostream& outStream = std::cout) {
		if (container.size() < 1) {
			initialize();
		}
        if (logFunctor.isStrictMem()) {
            benchmark::ContainerExperiment::runSingle(expsNumber, outStream);
        } else {
            benchmark::ContainerExperiment::runRange(expsNumber, outStream);
        }
	}

    benchmark::BenchResult executeExperiment(const std::size_t experimentNo, const std::size_t listSize) override {
        const std::size_t maxListSize = container.size();
        const std::size_t containerSize = std::min( maxListSize, listSize );

	    const double iterFactor = 3.0 * experimentNo / expsNumber + 1;
	    const std::size_t itersNum = iterFactor * maxListSize / containerSize + 1;

	    const typename BType::value_type* list = container.data();

	    uint64_t bestDur = -1;
	    for(std::size_t r=0; r<REPEATS; ++r) {
	        const uint64_t dur = bench_iteration<BType>(list, containerSize, itersNum);
	        if (dur < bestDur)
	            bestDur = dur;
	    }

	    const std::size_t memSizeB = containerSize * DATA_SIZE + CONTAINER_SIZE;
	    return benchmark::BenchResult(itersNum, REPEATS, containerSize, memSizeB, bestDur);
	}

	std::size_t calcContainerSize( const std::size_t memSizeB ) const {
		if (memSizeB <= CONTAINER_SIZE)
			return 0;
		return ( memSizeB - CONTAINER_SIZE ) / DATA_SIZE;
	}

};


#endif /* MEM_PERFORMANCE_BENCH_LIST_H_ */
