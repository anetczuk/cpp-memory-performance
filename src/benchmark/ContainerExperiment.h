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

#ifndef SRC_CONTAINEREXPERIMENT_H_
#define SRC_CONTAINEREXPERIMENT_H_

#include <iostream>
#include <cmath>

#include "benchmark_print.h"


// ================================================================================


static const std::size_t BASE        = 16;                          /// for list size
static const std::size_t STEPS       = 2;
static const std::size_t DIV         = std::pow(2, STEPS-1);        /// list size divider
static const std::size_t REPEATS_MAX = 25;                          /// number of repeats per experiment
static const std::size_t REPEATS_MIN =  4;                          /// number of repeats per experiment


inline std::size_t calculateContainerSize( const std::size_t experimentNo ) {
    const std::size_t exp = experimentNo / DIV;
    const std::size_t mod = experimentNo % DIV;
    const std::size_t mul = std::pow(2, exp);
    const std::size_t listSize = BASE * mul / DIV * (DIV + mod);
    return listSize;
}


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
    	    warmUp( experimentNumber-1 );
    	    executeExperiment(experimentNumber, experimentNumber, outStream);
    	}

    	void runRange(const std::size_t experimentsNumber, std::ostream& outStream = std::cout) {
    	    warmUp( 6 );
            for(std::size_t i=0; i<=experimentsNumber; ++i) {
                executeExperiment(i, experimentsNumber, outStream);
            }
    	}


    protected:

    	void warmUp( const std::size_t experimentNo ) {
//    	    std::cerr << STRINGIZE_STREAM( "warming up\n" );
            const std::size_t experimentSize = calculateContainerSize( experimentNo );
            executeRawExperiment( experimentNo, experimentSize );
    	}

    	void executeExperiment(const std::size_t currExperiment, const std::size_t experimentsNo, std::ostream& outStream = std::cout) {
            const std::size_t experimentSize = calculateContainerSize( currExperiment );
            const BenchResult data = executeRawExperiment(currExperiment, experimentSize);

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

            std::cerr << STRINGIZE_STREAM( std::fixed << currExperiment << "/" << experimentsNo << " listSize: " << listSize << " iters: " << iters << " repeats: " << data.repeats << " memory: " << memSize << " B " << memHuman << "\n" );

            outStream << STRINGIZE_STREAM( std::fixed << memSize << " B " << memHuman << " " );
            outStream << STRINGIZE_STREAM( std::fixed << "time/iter: " << timePerIter << " ns time/item: " << timePerElem << " ns iters: " << iters << " items: " << listSize << "\n" );
    	}

    	virtual BenchResult executeRawExperiment(const std::size_t experimentNo, const std::size_t listSize) = 0;

    };

}


#endif /* SRC_CONTAINEREXPERIMENT_H_ */
