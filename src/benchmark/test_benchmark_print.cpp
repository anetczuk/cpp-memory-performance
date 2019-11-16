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

#include "benchmark_print.h"


TEST(benchmark_print, STRINGIZE_token) {
    const std::string concatenated = STRINGIZE( 111, aaa );
    EXPECT_EQ( "111,aaa", concatenated );
}

TEST(benchmark_print, STRINGIZE_regular) {
    const std::string concatenated = STRINGIZE( 111, "aaa", "qqq,www" );
    EXPECT_EQ( "111,aaa,qqq,www", concatenated );
}

TEST(benchmark_print, STRINGIZE_nospaces) {
    const std::string concatenated = STRINGIZE( 111,"aaa","qqq,www" );
    EXPECT_EQ( "111,aaa,qqq,www", concatenated );
}

TEST(benchmark_print, STRINGIZE_spaces) {
    const std::string concatenated = STRINGIZE( 111  ,   "  aaa"  ,   "qqq,  www" );
    EXPECT_EQ( "111,  aaa,qqq,  www", concatenated );
}

TEST(benchmark_print, STRINGIZE_STREAM_regular) {
    const std::string concatenated = STRINGIZE_STREAM( 111 << "aaa" );
    EXPECT_EQ( "111aaa", concatenated );
}
