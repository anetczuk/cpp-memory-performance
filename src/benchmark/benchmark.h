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

#ifndef SRC_BENCHMARK_H_
#define SRC_BENCHMARK_H_

#include <iostream>
#include <cmath>

#include "benchmark_print.h"


// ================================================================================


static const std::size_t BASE    = 16;                          /// for list size
static const std::size_t STEPS   = 2;
static const std::size_t DIV     = std::pow(2, STEPS-1);        /// list size divider
static const std::size_t repeats = 25;                          /// number of repeats per experiment


// ================================================================================


namespace benchmark {

	struct BenchResult {
		bool valid;
		std::size_t iterations;
		std::size_t repeats;
		std::size_t containerSize;
		std::size_t memSize;		/// in Bytes
		uint64_t duration;

		BenchResult(): valid(false), iterations(0), repeats(1), containerSize(0), memSize(0), duration(0) {
		}

		BenchResult(const std::size_t iters, const std::size_t reps, const std::size_t conSize, const std::size_t memSize, const std::uint64_t duration):
			valid(true), iterations(iters), repeats(reps),
			containerSize(conSize), memSize(memSize),
			duration(duration)
		{
		}
	};


    // =====================================================================


    class ContainerExperiment {
    public:

    	ContainerExperiment() {
    	}

    	virtual ~ContainerExperiment() {
    	}

    	void runSingle(const std::size_t experimentNumber, std::ostream& outStream = std::cout) {
            /// warm up
            BUFFERED( std::cerr, "warming up" << std::endl );
            executeExperiment(experimentNumber-1);

            const BenchResult data = executeExperiment(experimentNumber);

            if (data.valid == false) {
                return;
            }

            const std::size_t iters = data.iterations;
            const std::size_t listSize = data.containerSize;
            const std::size_t memSize = data.memSize;
            const uint64_t duration = data.duration;
            const double timePerIter = double(duration) / iters;
            const double timePerElem = timePerIter / listSize;

            const std::string memHuman = humanMemSize(memSize);

            BUFFERED( std::cerr, std::fixed << experimentNumber << " iters: " << iters << " repeats: " << data.repeats << " memory: " << memSize << " B " << memHuman << std::endl );

            BUFFERED( outStream, std::fixed << memSize << " B " << memHuman << " " );
            BUFFERED( outStream, std::fixed << "time/iter: " << timePerIter << " ns time/item: " << timePerElem << " ns iters: " << iters << " items: " << listSize << std::endl );
    	}

    	void runRange(const std::size_t experimentsNumber, std::ostream& outStream = std::cout) {
			/// warm up
//    	    BUFFERED( std::cerr, "warming up" << std::endl );
    	    executeExperiment(6);

            for(std::size_t i=0; i<=experimentsNumber; ++i) {
//            for(std::size_t i=experimentsNumber-1; i<experimentsNumber; --i) {
                const BenchResult data = executeExperiment(i);

                if (data.valid == false)
                    continue;

                const std::size_t iters = data.iterations;
                const std::size_t listSize = data.containerSize;
                const std::size_t memSize = data.memSize;
                const uint64_t duration = data.duration;
                const double timePerIter = double(duration) / iters;
                const double timePerElem = timePerIter / listSize;

                const std::string memHuman = humanMemSize(memSize);

                BUFFERED( std::cerr, std::fixed << i << "/" << experimentsNumber << " listSize: " << listSize << " iters: " << iters << " repeats: " << data.repeats << " memory: " << memSize << " B " << memHuman << std::endl );

                BUFFERED( outStream, std::fixed << memSize << " B " << memHuman << " " );
                BUFFERED( outStream, std::fixed << "time/iter: " << timePerIter << " ns time/item: " << timePerElem << " ns iters: " << iters << " items: " << listSize << std::endl );
            }
    	}


    protected:

    	virtual BenchResult executeExperiment(const std::size_t experimentNo) {
            const std::size_t exp = experimentNo / DIV;
            const std::size_t mod = experimentNo % DIV;
            const std::size_t mul = std::pow(2, exp);
            const std::size_t listSize = BASE * mul / DIV * (DIV + mod);

            return executeExperiment(experimentNo, listSize);
    	}

    	virtual BenchResult executeExperiment(const std::size_t experimentNo, const std::size_t listSize) = 0;

    };

}


#endif /* SRC_BENCHMARK_H_ */
