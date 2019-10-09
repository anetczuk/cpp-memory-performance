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

#ifndef SRC_BENCHMARK_BENCHMARK_LOG_H_
#define SRC_BENCHMARK_BENCHMARK_LOG_H_

#include <cmath>


namespace benchmark {

    class LogExperimentFunctor {
    public:

        uint64_t strictSizeB;
        uint64_t maxSizeB;
        double factor;
        double decay;
        std::size_t itersmax;
        std::size_t minSize;
        std::size_t minIters;


        LogExperimentFunctor(const double factor = 4, const std::size_t itersmax = 10000000):
                strictSizeB(0), maxSizeB(0),
                factor(factor), decay(10.0), itersmax(itersmax), minSize(0), minIters(1)
        {
        }

        bool isStrictMem() const {
            return (strictSizeB > 0);
        }

        uint64_t getMemorySize() const {
            if (strictSizeB > 0)
                return strictSizeB;
            return maxSizeB;
        }

        uint64_t getMemoryOffset() const {
            if (strictSizeB > 0)
                return 0;
            return minSize;
        }

        void divideMemory(const std::size_t divider) {
            strictSizeB /= divider;
            maxSizeB /= divider;
        }

        void calcDecay() {
            // calcItersNumber( 0 ) = ITERSMAX
            // calcItersNumber( EMAX ) = MINITERS

            // ITERSMAX * decay / (calcMemSize( EMAX ) - 1 + decay) = MINITERS
            // decay / (calcMemSize( EMAX ) - 1 + decay) = MINITERS / ITERSMAX
            // (calcMemSize( EMAX ) - 1 + decay) / decay  = ITERSMAX / MINITERS
            // (calcMemSize( EMAX ) - 1) / decay + 1 = ITERSMAX / MINITERS
            // (calcMemSize( EMAX ) - 1) / decay = ITERSMAX / MINITERS - 1
            // calcMemSize( EMAX ) - 1 = (ITERSMAX / MINITERS - 1) * decay
            // decay = (calcMemSize( EMAX ) - 1) / (ITERSMAX / MINITERS - 1)

            // decay = MEMMAX - 1 / (ITERSMAX / MINITERS - 1)

        	const std::size_t EMAX = experimentsNumber();
        	const std::size_t MEMMAX = calcMemSize(EMAX) - 1;
        	const double itersFactor = double(itersmax) / minIters;
        	decay = double(MEMMAX) / ( itersFactor - 1 );
        }

        std::size_t experimentsNumber() const {
            // size = 2^(i/f)
            // log2(size) = i/f
            // i = f * log2(size)

            uint64_t mem = 0;
            if (strictSizeB > 0) {
                mem = strictSizeB;
            } else {
                if (maxSizeB < minSize)
                    return 0;
                mem = maxSizeB - minSize;
            }
            if (mem < 1)
                return 0;
            const std::size_t iternum = factor * std::log2( mem );
            return iternum;
        }

        std::size_t calcMemSize(const std::size_t experiment) {
            const uint64_t memoffset = getMemoryOffset();
            const std::size_t currSize = std::pow(2, double(experiment) / factor);   // in Bytes
            return currSize + memoffset;
        }

        std::size_t calcItersNumber(const std::size_t experiment) {
            const std::size_t currSize = calcMemSize(experiment) - 1;
            const double divider = decay / (decay + currSize);
            const std::size_t iters = itersmax * divider;
            return std::max( minIters, iters );
        }

        std::size_t calcItersNumber(const std::size_t listSize, const std::size_t itersNum) {
            /// accesses = listSize * repeats * iters
            /// iters = accesses / listSize * repeats
            ///static const std::size_t access_num = 500000000;
            std::size_t repeats = 500000000;
            repeats /= listSize;
            repeats /= itersNum;
            if (repeats < 5)
                return 5;
            return repeats;
        }

        static std::size_t calcLog(const std::size_t maxListSize, const std::size_t base, const double step) {
            /// base * step^x <= maxListSize
            /// log(base * step^x) <= log(maxListSize)
            /// log(base) + log(step^x) <= log(maxListSize)
            /// x*log(step) <= log(maxListSize) - log(base)
            /// x <= [log(maxListSize) - log(base)] / log(step)
            const double mamLog = std::log2( maxListSize );
            const double baseLog = std::log2( base );
            const double stepLog = std::log2( step );
            return (mamLog - baseLog) / stepLog;
        }

    };

}


#endif /* SRC_BENCHMARK_BENCHMARK_LOG_H_ */
