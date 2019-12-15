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

#ifndef MEM_PERFORMANCE_MEMORYEXPERIMENT_H_
#define MEM_PERFORMANCE_MEMORYEXPERIMENT_H_

#include "benchmark/ContainerExperiment.h"
#include "benchmark/benchmark_log.h"
#include "benchmark/benchmark_params.h"


class MemoryExperiment: public benchmark::ContainerExperiment {
public:

    benchmark::LogExperimentFunctor2 logFunctor;
    std::size_t expsNumber;

    std::size_t DATA_SIZE;
    std::size_t CONTAINER_SIZE;


    MemoryExperiment( const std::size_t elementSize, const std::size_t containerSize ): benchmark::ContainerExperiment(),
            logFunctor(),
            expsNumber(0),
            DATA_SIZE( elementSize ), CONTAINER_SIZE( containerSize )
    {
    }

    ~MemoryExperiment() override {
    }

    void initialize() {
        const uint64_t memSize = logFunctor.getMemorySize();

        const std::size_t listSize = calcContainerSize( memSize );

        std::cerr << STRINGIZE_STREAM( "initializing memory: " << memSize << " (" << std::fixed << std::setw( 6 ) << ( double(memSize) / (1024*1024*1024)) << " GB), container size: " << listSize << "\n" );

        expsNumber = benchmark::LogExperimentFunctor::calcLog(listSize, BASE, DIV) + 1;

        allocateContainer( listSize );
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
        if (containerSize() < 1) {
            initialize();
        }
        if (logFunctor.isStrictMem()) {
            benchmark::ContainerExperiment::runSingle(expsNumber, outStream);
        } else {
            benchmark::ContainerExperiment::runRange(expsNumber, outStream);
        }
    }

    std::size_t calcContainerSize( const std::size_t memSizeB ) const {
        if (memSizeB <= CONTAINER_SIZE)
            return 0;
        return ( memSizeB - CONTAINER_SIZE ) / DATA_SIZE;
    }


protected:

    benchmark::BenchResult executeRawExperiment(const std::size_t experimentNo, const std::size_t listSize) override {
        const std::size_t maxListSize = containerSize();
        const std::size_t currListSize = std::min( maxListSize, listSize );

        const double expFactor = ((double) experimentNo ) / expsNumber;

        const double iterFactor = expFactor + 1.0 / expsNumber;
        const std::size_t itersNum = iterFactor * maxListSize / currListSize + 1;
        const std::size_t repeats = (1.0 - expFactor) * REPEATS_MAX + expFactor * REPEATS_MIN;

        const uint64_t bestDur = executeIterations( currListSize, itersNum, repeats );

        const std::size_t memSizeB = currListSize * DATA_SIZE + CONTAINER_SIZE;
        return benchmark::BenchResult(itersNum, repeats, currListSize, memSizeB, bestDur);
    }

    virtual uint64_t executeIterations( const std::size_t containerSize, const std::size_t itersNum, const std::size_t repeats ) = 0;

    virtual std::size_t containerSize() const = 0;

    virtual void allocateContainer( const std::size_t elementsNumber ) = 0;

};


#endif /* MEM_PERFORMANCE_MEMORYEXPERIMENT_H_ */
