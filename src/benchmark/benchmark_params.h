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

#include "benchmark_print.h"


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
		    BUFFERED( std::cerr, "'maxmem' not given\n" );
			return -1;
		}
		const std::string data(maxMem);
		const std::size_t chNum = data.length();
		if (chNum < 1) {
		    BUFFERED( std::cerr, "'maxmem' empty\n" );
			return -1;
		}
		long long unitMuliplier = 1;
		double value = -1.0;
		const char unit = maxMem[ chNum-1 ];
		switch(unit) {
		case 'B': {
		    const std::string strnum = data.substr(0, chNum-1);
			value = std::stod( strnum );
			break;
		}
		case 'K': {
			unitMuliplier = 1024;
            const std::string strnum = data.substr(0, chNum-1);
            value = std::stod( strnum );
			break;
		}
		case 'M': {
			unitMuliplier = 1024*1024;
            const std::string strnum = data.substr(0, chNum-1);
            value = std::stod( strnum );
			break;
		}
		case 'G': {
			unitMuliplier = 1024*1024*1024;
            const std::string strnum = data.substr(0, chNum-1);
            value = std::stod( strnum );
			break;
		}
		default: {
			/// no unit
			value = std::stod( data );
			break;
		}
		}
		if (value < 0.0) {
		    BUFFERED( std::cerr, "could not parse 'maxmem' argument: '" << data << "'\n" );
			return -1;
		}
		long long memsize = value * unitMuliplier;
		if (memsize < 1) {
			return -1;
		}
		return memsize;
	}

	long long get_param_long(int argc, char** argv, const std::string& param) {
		const char* rawParam = get_param(param, argc, argv);
		if (rawParam == nullptr) {
			return -1;
		}
		const std::string data(rawParam);
		const std::size_t chNum = data.length();
		if (chNum < 1) {
			return -1;
		}
		return std::atoll( data.c_str() );
	}

}


#endif /* SRC_BENCHMARK_BENCHMARK_PARAMS_H_ */
