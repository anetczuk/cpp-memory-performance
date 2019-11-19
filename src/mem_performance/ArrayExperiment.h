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

#ifndef MEM_PERFORMANCE_ARRAYEXPERIMENT_H_
#define MEM_PERFORMANCE_ARRAYEXPERIMENT_H_

#include "MemoryExperiment.h"

#include "benchmark/benchmark_time.h"

#include "unroll.h"


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
class ArrayExperiment: public MemoryExperiment {
public:

    typedef typename BType::value_type InnerType;

	BType container;


	ArrayExperiment(): MemoryExperiment( sizeof(InnerType), sizeof(BType) ), container() {
	}

	~ArrayExperiment() override {
	}


protected:

	uint64_t executeIterations( const std::size_t containerSize, const std::size_t itersNum, const std::size_t repeats ) override {
        const InnerType* list = container.data();

        uint64_t bestDur = -1;
        for(std::size_t r=1; r<=repeats; ++r) {
//            std::cerr << "repetition " << r << "/" << repeats << "\n";
            const uint64_t dur = trait<InnerType, UNROLL>::iterate(list, containerSize, itersNum);
            if (dur < bestDur)
                bestDur = dur;
        }

        return bestDur;
	}

    std::size_t containerSize() const override {
        return container.size();
    }

    void allocateContainer( const std::size_t elementsNumber ) override {
        container = BType( elementsNumber, 0 );
    }

};


#endif /* MEM_PERFORMANCE_ARRAYEXPERIMENT_H_ */
