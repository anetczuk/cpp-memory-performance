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

#ifndef MEM_PERFORMANCE_ARRAY_H_
#define MEM_PERFORMANCE_ARRAY_H_

#include <vector>


typedef std::vector<uint64_t> StdVector;


struct Array {
	typedef uint64_t value_type;
	value_type* raw_data;

	Array(): raw_data(nullptr) {
		init(0, 1);
	}

	Array(const std::size_t size, const value_type initVal): raw_data(nullptr) {
		init(size, initVal);
	}

	virtual ~Array() {
		release();
	}

	Array& operator =( const Array& object ) {
	    if( &object == this ) {
	    	/// do nothing
	    	return *this;
	    }
	    release();
	    Array* obj = const_cast<Array*>(&object);
	    raw_data = obj->raw_data;
	    obj->raw_data = nullptr;
	    return * this;
	}

	void init(const std::size_t size, const value_type initVal) {
		release();
		raw_data = new value_type[size];
		for(std::size_t i=0; i<size; ++i) {
			raw_data[i] = initVal;
		}
	}

	void release() {
		if (raw_data == nullptr)
			return ;
		delete[] raw_data;
		raw_data = nullptr;
	}

	const value_type* data() const {
		return raw_data;
	}

};


#endif /* MEM_PERFORMANCE_ARRAY_H_ */
