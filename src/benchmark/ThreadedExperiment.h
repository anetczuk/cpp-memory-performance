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

#ifndef SRC_BENCHMARK_THREADEDEXPERIMENT_H_
#define SRC_BENCHMARK_THREADEDEXPERIMENT_H_

#include "ContainerExperiment.h"

#include <fstream>
#include <thread>
#include <vector>


namespace benchmark {

	template <typename EType>
	struct ThreadedExperiment {
	public:

		std::thread worker;
		std::unique_ptr<std::ofstream> outFile;
		EType experiment;


		ThreadedExperiment(const std::string& outputFile): worker(), outFile(nullptr), experiment() {
			outFile.reset( new std::ofstream() );
			outFile->open(outputFile);
		}

		void run() {
			worker = std::thread( &ThreadedExperiment<EType>::execute, this );
		}

		void join() {
			worker.join();
		}

		static void runAll( std::vector< ThreadedExperiment<EType> >& container ) {
			const std::size_t cSize = container.size();

			/// execute
			for(std::size_t i=0; i<cSize; ++i) {
				ThreadedExperiment<EType>& experiment = container[i];
				experiment.run();
			}

			/// wait for finish
			for(std::size_t i=0; i<cSize; ++i) {
				container[i].join();
			}
		}


	private:

		void execute() {
			if (outFile != nullptr)
				experiment.run( *outFile );
			else
				experiment.run();
		}
	};

}


#endif /* SRC_BENCHMARK_THREADEDEXPERIMENT_H_ */
