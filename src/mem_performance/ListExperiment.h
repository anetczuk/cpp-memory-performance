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

#ifndef MEM_PERFORMANCE_LISTEXPERIMENT_H_
#define MEM_PERFORMANCE_LISTEXPERIMENT_H_

#include "MemoryExperiment.h"

#include "benchmark/benchmark_time.h"


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


// =====================================================================


template <typename BType>
class ListExperiment: public MemoryExperiment {
public:

    typedef typename BType::value_type InnerType;

	BType container;


	ListExperiment(): MemoryExperiment( sizeof(InnerType), sizeof(BType) ), container() {
	}

	~ListExperiment() override {
	}


protected:

    uint64_t executeIterations( const std::size_t containerSize, const std::size_t itersNum, const std::size_t repeats ) override {
        const InnerType* list = container.data();

        uint64_t bestDur = -1;
        for(std::size_t r=1; r<=repeats; ++r) {
//          std::cerr << "repetition " << r << "/" << repeats << "\n";
            const uint64_t dur = bench_iteration<BType>(list, containerSize, itersNum);
            if (dur < bestDur)
                bestDur = dur;
        }

        return bestDur;
    }

    std::size_t containerSize() const override {
        return container.size();
    }

    void allocateContainer( const std::size_t elementsNumber ) override {
        container = BType( elementsNumber );
        container.randomize();
    }

};


#endif /* MEM_PERFORMANCE_LISTEXPERIMENT_H_ */
