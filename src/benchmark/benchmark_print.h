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

#ifndef SRC_BENCHMARK_BENCHMARK_PRINT_H_
#define SRC_BENCHMARK_BENCHMARK_PRINT_H_

#include <sstream>
#include <iomanip>


struct comma_is_space : std::ctype<char> {
  comma_is_space() : std::ctype<char>(get_table()) {}
  static mask const* get_table()
  {
    static mask rc[table_size];             /// table_size is 256
    rc[' ']  = std::ctype_base::space;
    rc[',']  = std::ctype_base::space;
    rc['\n'] = std::ctype_base::space;
    return &rc[0];
  }
};


inline std::string dequote(const std::string& str) {
    std::stringstream input( str );
    input.imbue( std::locale(input.getloc(), new comma_is_space) );

    std::stringstream output;
    std::string buff;

    while (input >> std::quoted(buff)) {
//        std::cout << "buff: >" << buff << "<\n";
        output << buff << ",";
    }

    std::string ret = output.str();
    return ret.substr( 0, ret.size()-1 );       /// remove last comma
}

#define STRINGIZE( ... ) dequote( #__VA_ARGS__ )


#define STRINGIZE_STREAM( data )  ((std::stringstream&)(std::stringstream() << data)).str()


//
// beffered print prevents interleaving of messages
//
#define BUFFERED( output, data )                \
            {                                   \
                std::stringstream stream;       \
                stream << data;                 \
                output << stream.str();         \
            }


namespace benchmark {

	inline std::string humanMemSize(const std::size_t memSizeB) {
		std::stringstream stream;
        const double sizeKB = double(memSizeB) / 1024;
        if (sizeKB < 1024)
        	stream << std::fixed << sizeKB << " KB";
        else if (sizeKB < 1024*1024)
        	stream << std::fixed << (double)sizeKB / 1024 << " MB";
        else
        	stream << std::fixed << (double)sizeKB / (1024*1024) << " GB";
        return stream.str();
	}

}


#endif /* SRC_BENCHMARK_BENCHMARK_PRINT_H_ */
