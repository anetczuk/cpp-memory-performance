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

#ifndef SRC_BENCHMARK_BENCHMARK_AVG_H_
#define SRC_BENCHMARK_BENCHMARK_AVG_H_

#include <vector>
#include <algorithm>


namespace benchmark {

	class ResultsVector {
	public:

		std::vector<uint64_t> times;


		ResultsVector(const std::size_t timesSize = 1): times(timesSize, 0) {
		}

		void push_back(const uint64_t duration) {
			times.push_back(duration);
		}

		void put(const std::size_t index, const uint64_t duration) {
			times[index] = duration;
		}

		uint64_t min() {
			if (times.size() < 1)
				return 0;
			std::sort( times.begin(), times.end() );
			return times[0];
		}

		uint64_t max() {
			if (times.size() < 1)
				return 0;
			std::sort( times.begin(), times.end() );
			return times[ times.size()-1 ];
		}

		uint64_t result(const std::size_t averageSize) {
			std::sort( times.begin(), times.end() );
			uint64_t sum = 0;
			const std::size_t sumSize = std::min(averageSize, times.size());
			for(std::size_t i=0; i<sumSize; ++i) {
				sum += times[i];
			}
			if (sumSize == 0)
				return 0;
			if (sumSize == 1)
				return sum;
			return sum / sumSize;
		}

	};

}


#endif /* SRC_BENCHMARK_BENCHMARK_AVG_H_ */
