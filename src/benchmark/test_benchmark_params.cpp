//
//
//

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
