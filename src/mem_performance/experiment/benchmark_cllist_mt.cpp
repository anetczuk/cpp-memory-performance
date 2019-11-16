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

#include <fstream>
#include <string>
#include <iomanip>

#include "benchmark/benchmark_thread.h"
#include "bench_list.h"

#include "CustomLinkedList.h"


typedef ListExperiment<CustomLinkedList> Experiment;
typedef benchmark::ThreadedExperiment<Experiment> Worker;


int main(int argc, char** argv) {
#ifdef DISABLE_MT_BENCHMARKS
    std::cerr << "multithreaded benchmark disabled\n";
    return 0;
#endif

	unsigned int nthreads = std::thread::hardware_concurrency();

	BUFFERED( std::cerr, "found threads: " << nthreads << std::endl );

	std::vector<Worker> workers;
	workers.reserve(nthreads);				/// make sure threads won't be copied

	/// initialize
	for(unsigned int i=0; i<nthreads; ++i) {
		const std::string filePath = "./data/cllist_mt_data_raw_core_" + std::to_string(i+1) + ".txt";
		workers.push_back( Worker(filePath) );
		Worker& currWorker = workers.back();
		currWorker.experiment.parseArguments(argc, argv);

		currWorker.experiment.logFunctor.divideMemory( nthreads );
	}

	Worker::runAll( workers );

    return 0;
}
