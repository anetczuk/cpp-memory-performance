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

#include <gtest/gtest.h>

#include "benchmark_params.h"


TEST(benchmark, get_param_maxmem_2K) {
	std::vector<std::string> data{"--maxmem", "2K"};
	std::vector<char*> args;
    for(std::size_t i = 0; i < data.size(); ++i) {
        args.push_back(const_cast<char*>(data[i].c_str()));
    }

	const long long maxmem = benchmark::get_param_maxmem(args.size(), args.data() );
    EXPECT_EQ(2*1024, maxmem);
}

TEST(benchmark, get_param_maxmem_2M) {
	std::vector<std::string> data{"--maxmem", "3M"};
	std::vector<char*> args;
    for(std::size_t i = 0; i < data.size(); ++i) {
        args.push_back(const_cast<char*>(data[i].c_str()));
    }

	const long long maxmem = benchmark::get_param_maxmem(args.size(), args.data() );
    EXPECT_EQ(3*1024*1024, maxmem);
}

TEST(benchmark, get_param_maxmem_4_5G) {
	std::vector<std::string> data{"--maxmem", "4.5G"};
	std::vector<char*> args;
    for(std::size_t i = 0; i < data.size(); ++i) {
        args.push_back(const_cast<char*>(data[i].c_str()));
    }

	const long long maxmem = benchmark::get_param_maxmem(args.size(), args.data() );
    EXPECT_EQ(4.5*1024*1024*1024, maxmem);
}
