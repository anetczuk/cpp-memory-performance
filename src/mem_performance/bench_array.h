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

#include "benchmark/benchmark.h"
#include "benchmark/benchmark_log.h"
#include "benchmark/benchmark_time.h"

#include "array.h"


template <typename BType>
inline uint64_t bench_iteration(const typename BType::value_type* list, const std::size_t listSize, const std::size_t itersnum) {
	benchmark::Clock::TimePoint startTime, endTime;
	typename BType::value_type sum = 0;
	startTime = benchmark::Clock::Now();

	for(std::size_t i = 0; i<itersnum; ++i) {
		std::size_t x = 0;
		for(; x<listSize; ++x) {
			sum += list[x];
		}
	}

	endTime = benchmark::Clock::Now();
	if ( sum != listSize*itersnum )  {
		std::cerr << "internal error" << std::endl;
		exit(1);
	}
	const uint64_t duration = benchmark::Clock::Duration(startTime, endTime);
	return duration;
}


template <typename BType>
class VectorExperiment: public benchmark::ContainerExperiment {
public:

	benchmark::LogExperimentFunctor logFunctor;
	double avgProbesFactor;
	std::size_t avgProbesMin;
	std::size_t prevMemSize;
	std::size_t expsNumber;
	BType container;

	std::size_t DATA_SIZE;
	std::size_t CONTAINER_SIZE;


	VectorExperiment(): benchmark::ContainerExperiment(),
			logFunctor(), avgProbesFactor(0.0), avgProbesMin(1),
			prevMemSize(0), expsNumber(0), container(),
			DATA_SIZE( sizeof(typename BType::value_type) ), CONTAINER_SIZE( sizeof(BType) )
	{
		logFunctor.maxSizeB = (1024 + 256)*1024*1024L;
		logFunctor.factor 	= 3.0;
		logFunctor.itersmax = 10000;
		logFunctor.minIters = 3;
		logFunctor.decay 	= 10000.0;

		avgProbesFactor = 50;
		avgProbesMin = 5;

		expsNumber = logFunctor.experimentsNumber();

		//TODO: adding memory declaration here helps timing

		std::cerr << "initializing memory" << std::endl;
		const std::size_t listSize = calcContainerSize( logFunctor.maxSizeB );
		container = BType( listSize, 1 );
	}

	virtual ~VectorExperiment() {
	}

	void run() {
		benchmark::ContainerExperiment::run(expsNumber);
	}

	benchmark::BenchResult experiment(const std::size_t experimentNo) override {
        const std::size_t memSizeB = logFunctor.calcMemSize(experimentNo);		// in Bytes

	    if (memSizeB < CONTAINER_SIZE)
	        return benchmark::BenchResult();
        if (memSizeB == prevMemSize) {
        	return benchmark::BenchResult();
        }
        prevMemSize = memSizeB;

	    const std::size_t listSize = calcContainerSize( memSizeB );
	    if (listSize < 8)
	        return benchmark::BenchResult();

        const std::size_t itersnum = logFunctor.calcItersNumber(experimentNo);

	    const double avgFactor = 1.0 - double(experimentNo) / expsNumber;		// [1..0]
	    const std::size_t avgProbes = avgFactor * avgProbesFactor + avgProbesMin;
	    uint64_t minVal = -1;

	    const typename BType::value_type* list = container.data();

	    for(std::size_t x = 0; x<avgProbes; ++x) {
	    	const uint64_t duration = bench_iteration<BType>(list, listSize, itersnum);
	    	minVal = std::min(minVal, duration);
	    }

	    return benchmark::BenchResult(itersnum, avgProbes, listSize, memSizeB, minVal);
	}

	std::size_t calcContainerSize( const std::size_t memSizeB ) const {
		return ( memSizeB - CONTAINER_SIZE ) / DATA_SIZE;
	}

};


#endif /* MEM_PERFORMANCE_BENCH_ARRAY_H_ */