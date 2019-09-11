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

#ifndef SRC_BENCH_H_
#define SRC_BENCH_H_

#include <iostream>

#include "benchmark_print.h"


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

    	virtual BenchResult experiment(const std::size_t experimentNo) = 0;


    	void run(const std::size_t experimentsNumber, std::ostream& outStream = std::cout) {
			/// warm up
    		std::cerr << "warming up" << std::endl;
			experiment(experimentsNumber);

            for(std::size_t i=0; i<=experimentsNumber; ++i) {
                const BenchResult data = experiment(i);

                if (data.valid == false)
                    continue;

                const std::size_t iters = data.iterations;
                const std::size_t listSize = data.containerSize;
                const std::size_t memSize = data.memSize;
                const uint64_t duration = data.duration;
                const double timePerIter = double(duration) / iters;
                const double timePerElem = timePerIter / listSize;

                const std::string memHuman = humanMemSize(memSize);

                std::cerr << std::fixed << i << "/" << experimentsNumber << " iters: " << iters << " repeats: " << data.repeats << " memory: " << memSize << " B " << memHuman << std::endl;

                outStream << std::fixed << memSize << " B " << memHuman << " ";
                outStream << std::fixed << "time/iter: " << timePerIter << " ns time/item: " << timePerElem << " ns iters: " << iters << " items: " << listSize << std::endl;
            }
    	}

    };

}


#endif /* SRC_BENCH_H_ */
