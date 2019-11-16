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

#ifndef MEM_PERFORMANCE_BENCH_ARRAY_H_
#define MEM_PERFORMANCE_BENCH_ARRAY_H_

#include <iostream>
#include <iomanip>
#include <limits>

#include "benchmark/benchmark.h"
#include "benchmark/benchmark_log.h"
#include "benchmark/benchmark_time.h"
#include "benchmark/benchmark_params.h"

#include "unroll.h"
#include "Array.h"


template<typename Type>
inline uint64_t iterate_1(const Type* container, const std::size_t listSize, const std::size_t itersnum) {
    Type sum = 0;

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
        std::cerr << "internal error -- sum error\n";
        exit(1);
    }

    return DURATION(startTime, endTime);
}

template<typename Type>
inline uint64_t iterate_16(const Type* container, const std::size_t listSize, const std::size_t itersnum) {
    const std::size_t listLimit = listSize-16;          /// prevents array out of bound
    if (listLimit > listSize) {
        /// invalid state -- unsigned overflow
        return -1;
    }
    Type sum = 0;

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
        std::cerr << "internal error -- sum error\n";
        exit(1);
    }

    return DURATION(startTime, endTime);
}


template<typename Type, int S>
struct trait {
    static uint64_t iterate(const Type* container, const std::size_t listSize, const std::size_t itersnum);
};

template<typename Type>
struct trait<Type, 1> {
    static uint64_t iterate(const Type* container, const std::size_t listSize, const std::size_t itersnum) {
        return iterate_1(container, listSize, itersnum);
    }
};

template<typename Type>
struct trait<Type, 16> {
    static uint64_t iterate(const Type* container, const std::size_t listSize, const std::size_t itersnum) {
        return iterate_16(container, listSize, itersnum);
    }
};


// ================================================================================


template <typename BType>
class VectorExperiment: public benchmark::ContainerExperiment {
public:

    typedef typename BType::value_type InnerType;

	benchmark::LogExperimentFunctor2 logFunctor;
	std::size_t expsNumber;
	BType container;

	std::size_t DATA_SIZE;
	std::size_t CONTAINER_SIZE;


	VectorExperiment(): benchmark::ContainerExperiment(),
			logFunctor(),
			expsNumber(0), container(),
			DATA_SIZE( sizeof(InnerType) ), CONTAINER_SIZE( sizeof(BType) )
	{
	}

	~VectorExperiment() override {
	}

	void initialize() {
	    const uint64_t memSize = logFunctor.getMemorySize();

		const std::size_t listSize = calcContainerSize( memSize );

	    std::cerr << STRINGIZE_STREAM( "initializing memory: " << memSize << " (" << std::fixed << std::setw( 6 ) << ( double(memSize) / (1024*1024*1024)) << " GB), container size: " << listSize << "\n" );

		expsNumber = benchmark::LogExperimentFunctor::calcLog(listSize, BASE, DIV) + 1;
		container = BType( listSize, 0 );
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

	    const InnerType* list = container.data();

	    uint64_t bestDur = -1;
	    for(std::size_t r=0; r<repeats; ++r) {
//	        std::cerr << "repetition " << r << "/" << repeats << "\n";
	        const uint64_t dur = trait<InnerType, UNROLL>::iterate(list, containerSize, itersNum);
	        if (dur < bestDur)
	            bestDur = dur;
	    }

	    const std::size_t memSizeB = containerSize * DATA_SIZE + CONTAINER_SIZE;
	    return benchmark::BenchResult(itersNum, repeats, containerSize, memSizeB, bestDur);
	}

	std::size_t calcContainerSize( const std::size_t memSizeB ) const {
		if (memSizeB <= CONTAINER_SIZE)
			return 0;
		return ( memSizeB - CONTAINER_SIZE ) / DATA_SIZE;
	}

};


#endif /* MEM_PERFORMANCE_BENCH_ARRAY_H_ */
