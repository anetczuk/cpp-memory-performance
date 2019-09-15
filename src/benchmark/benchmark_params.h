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

#ifndef SRC_BENCHMARK_BENCHMARK_PARAMS_H_
#define SRC_BENCHMARK_BENCHMARK_PARAMS_H_

#include <stdlib.h>     /* atoi */


namespace benchmark {

	char* get_param_raw(const std::string& param, int argc, char** argv) {
		int i = 0;
		for(; i<argc; ++i) {
			if (param.compare( argv[i] ) == 0) {
				break;
			}
		}
		if (i < (argc-1)) {
			return argv[i+1];
		}
		return nullptr;
	}

	char* get_param(const std::string& param, int argc, char** argv) {
		char* value = nullptr;
		value = get_param_raw( "-" + param, argc, argv );
		if (value != nullptr) {
			return value;
		}
		value = get_param_raw( "--" + param, argc, argv );
		if (value != nullptr) {
			return value;
		}
		value = get_param_raw( param, argc, argv );
		if (value != nullptr) {
			return value;
		}
		return nullptr;
	}

	long long get_param_maxmem(int argc, char** argv) {
		const char* maxMem = get_param("maxmem", argc, argv);
		if (maxMem == nullptr) {
			return -1;
		}
		return atoll(maxMem) * 1024;		/// returns in KB unit
	}

}


#endif /* SRC_BENCHMARK_BENCHMARK_PARAMS_H_ */
