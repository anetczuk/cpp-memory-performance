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

#ifndef SRC_BENCHMARK_CUSTOM_OUTPUTTER_H_
#define SRC_BENCHMARK_CUSTOM_OUTPUTTER_H_

#include "hayai/hayai_main.hpp"
#include "hayai/hayai_outputter.hpp"
#include "hayai/hayai_console.hpp"


/// Prints the result to standard output.
class CustomOutputter: public ::hayai::Outputter
{
public:
    /// Initialize console outputter.

    /// @param stream Output stream. Must exist for the entire duration of
    /// the outputter's use.
    CustomOutputter(std::ostream& stream = std::cout)
        :   _stream(stream)
    {

    }


    virtual void Begin(const std::size_t& enabledCount,
                       const std::size_t& disabledCount)
    {
        _stream << std::fixed;
        _stream << ::hayai::Console::TextGreen << "[==========]"
                << ::hayai::Console::TextDefault << " Running "
                << enabledCount
                << (enabledCount == 1 ? " benchmark." : " benchmarks");

        if (disabledCount)
            _stream << ", skipping "
                    << disabledCount
                    << (disabledCount == 1 ?
                        " benchmark." :
                        " benchmarks");
        else
            _stream << ".";

        _stream << std::endl;
    }


    virtual void End(const std::size_t& executedCount,
                     const std::size_t& disabledCount)
    {
        _stream << ::hayai::Console::TextGreen << "[==========]"
                << ::hayai::Console::TextDefault << " Ran " << executedCount
                << (executedCount == 1 ?
                    " benchmark." :
                    " benchmarks");

        if (disabledCount)
            _stream << ", skipped "
                    << disabledCount
                    << (disabledCount == 1 ?
                        " benchmark." :
                        " benchmarks");
        else
            _stream << ".";

        _stream << std::endl;
    }


    inline void BeginOrSkipTest(const std::string& fixtureName,
                                const std::string& testName,
                                const ::hayai::TestParametersDescriptor& parameters,
                                const std::size_t& runsCount,
                                const std::size_t& iterationsCount,
                                const bool skip)
    {
        if (skip)
            _stream << ::hayai::Console::TextCyan << "[ DISABLED ]";
        else
            _stream << ::hayai::Console::TextGreen << "[ RUN      ]";

        _stream << ::hayai::Console::TextYellow << " ";
        WriteTestNameToStream(_stream, fixtureName, testName, parameters);
        _stream << ::hayai::Console::TextDefault
                << " (" << runsCount
                << (runsCount == 1 ? " run, " : " runs, ")
                << iterationsCount
                << (iterationsCount == 1 ?
                    " iteration per run)" :
                    " iterations per run)")
                << std::endl;
    }


    virtual void BeginTest(const std::string& fixtureName,
                           const std::string& testName,
                           const ::hayai::TestParametersDescriptor& parameters,
                           const std::size_t& runsCount,
                           const std::size_t& iterationsCount)
    {
        BeginOrSkipTest(fixtureName,
                        testName,
                        parameters,
                        runsCount,
                        iterationsCount,
                        false);
    }


    virtual void SkipDisabledTest(
        const std::string& fixtureName,
        const std::string& testName,
        const ::hayai::TestParametersDescriptor& parameters,
        const std::size_t& runsCount,
        const std::size_t& iterationsCount
    )
    {
        BeginOrSkipTest(fixtureName,
                        testName,
                        parameters,
                        runsCount,
                        iterationsCount,
                        true);
    }


    virtual void EndTest(const std::string& fixtureName,
                         const std::string& testName,
                         const ::hayai::TestParametersDescriptor& parameters,
                         const ::hayai::TestResult& result)
    {
#define PAD(x) _stream << std::setw(34) << x << std::endl;
#define PAD_DEVIATION(description,                                      \
                  deviated,                                         \
                  average,                                          \
                  unit)                                             \
        {                                                           \
            double _d_ =                                            \
                double(deviated) - double(average);                 \
                                                                    \
            PAD(description <<                                      \
                deviated << " " << unit << " (" <<                  \
                (deviated < average ?                               \
                        ::hayai::Console::TextRed :                                 \
                         ::hayai::Console::TextGreen) <<                             \
                (deviated > average ? "+" : "") <<                  \
                _d_ << " " << unit << " / " <<                      \
                (deviated > average ? "+" : "") <<                  \
                (_d_ * 100.0 / average) << " %" <<                  \
                ::hayai::Console::TextDefault << ")");                       \
        }
#define PAD_DEVIATION_INVERSE(description,                              \
                          deviated,                                 \
                          average,                                  \
                          unit)                                     \
        {                                                           \
            double _d_ =                                            \
                double(deviated) - double(average);                 \
                                                                    \
            PAD(description <<                                      \
                deviated << " " << unit << " (" <<                  \
                (deviated > average ?                               \
                        ::hayai::Console::TextRed :                                 \
                         ::hayai::Console::TextGreen) <<                             \
                (deviated > average ? "+" : "") <<                  \
                _d_ << " " << unit << " / " <<                      \
                (deviated > average ? "+" : "") <<                  \
                (_d_ * 100.0 / average) << " %" <<                  \
                ::hayai::Console::TextDefault << ")");                       \
        }

        _stream << ::hayai::Console::TextGreen << "[     DONE ]"
                << ::hayai::Console::TextYellow << " ";
        WriteTestNameToStream(_stream, fixtureName, testName, parameters);
        _stream << ::hayai::Console::TextDefault << " ("
                << std::setprecision(6)
                << (result.TimeTotal() / 1000000.0) << " ms)"
                << std::endl;

        _stream << ::hayai::Console::TextBlue << "[   RUNS   ] "
                << ::hayai::Console::TextDefault
                << "       Average time: "
                << std::setprecision(3)
                << result.RunTimeAverage() / 1000.0 << " us "
                << "(" << ::hayai::Console::TextBlue << "~"
                << result.RunTimeStdDev() / 1000.0 << " us"
                << ::hayai::Console::TextDefault << ")"
                << std::endl;

        PAD_DEVIATION_INVERSE("Fastest time: ",
                              (result.RunTimeMinimum() / 1000.0),
                              (result.RunTimeAverage() / 1000.0),
                              "us");
        PAD_DEVIATION_INVERSE("Slowest time: ",
                              (result.RunTimeMaximum() / 1000.0),
                              (result.RunTimeAverage() / 1000.0),
                              "us");
        PAD("Median time: " <<
            result.RunTimeMedian() / 1000.0 << " us (" <<
            ::hayai::Console::TextCyan << "1st quartile: " <<
            result.RunTimeQuartile1() / 1000.0 << " us | 3rd quartile: " <<
            result.RunTimeQuartile3() / 1000.0 << " us" <<
            ::hayai::Console::TextDefault << ")");

        _stream << std::setprecision(5);

        PAD("");
        PAD("Average performance: " <<
            result.RunsPerSecondAverage() << " runs/s");
        PAD_DEVIATION("Best performance: ",
                      result.RunsPerSecondMaximum(),
                      result.RunsPerSecondAverage(),
                      "runs/s");
        PAD_DEVIATION("Worst performance: ",
                      result.RunsPerSecondMinimum(),
                      result.RunsPerSecondAverage(),
                      "runs/s");
        PAD("Median performance: " <<
            result.RunsPerSecondMedian() << " runs/s (" <<
            ::hayai::Console::TextCyan << "1st quartile: " <<
            result.RunsPerSecondQuartile1() << " | 3rd quartile: " <<
            result.RunsPerSecondQuartile3() <<
            ::hayai::Console::TextDefault << ")");

        PAD("");
        _stream << ::hayai::Console::TextBlue << "[ITERATIONS] "
                << ::hayai::Console::TextDefault
                << std::setprecision(3)
                << "       Average time: "
                << result.IterationTimeAverage() << " ns "
                << "(" << ::hayai::Console::TextBlue << "~"
                << result.IterationTimeStdDev() << " ns"
                << ::hayai::Console::TextDefault << ")"
                << std::endl;

        PAD_DEVIATION_INVERSE("Fastest time: ",
                              (result.IterationTimeMinimum()),
                              (result.IterationTimeAverage()),
                              "ns");
        PAD_DEVIATION_INVERSE("Slowest time: ",
                              (result.IterationTimeMaximum()),
                              (result.IterationTimeAverage()),
                              "ns");
        PAD("Median time: " <<
            result.IterationTimeMedian() << " ns (" <<
            ::hayai::Console::TextCyan << "1st quartile: " <<
            result.IterationTimeQuartile1() <<
            " ns | 3rd quartile: " <<
            result.IterationTimeQuartile3() << " ns" <<
            ::hayai::Console::TextDefault << ")");

        _stream << std::setprecision(5);

        PAD("");
        PAD("Average performance: " <<
            result.IterationsPerSecondAverage() <<
            " iterations/s");
        PAD_DEVIATION("Best performance: ",
                      (result.IterationsPerSecondMaximum()),
                      (result.IterationsPerSecondAverage()),
                      "iterations/s");
        PAD_DEVIATION("Worst performance: ",
                      (result.IterationsPerSecondMinimum()),
                      (result.IterationsPerSecondAverage()),
                      "iterations/s");
        PAD("Median performance: " <<
            result.IterationsPerSecondMedian() << " iterations/s (" <<
            ::hayai::Console::TextCyan << "1st quartile: " <<
            result.IterationsPerSecondQuartile1() <<
            " | 3rd quartile: " <<
            result.IterationsPerSecondQuartile3() <<
            ::hayai::Console::TextDefault << ")");

#undef PAD_DEVIATION_INVERSE
#undef PAD_DEVIATION
#undef PAD
    }


    std::ostream& _stream;
};



class CustomFileOutputter: public ::hayai::FileOutputter
{
public:

    CustomFileOutputter(const char* path): ::hayai::FileOutputter(path) {}


protected:

    virtual ::hayai::Outputter* CreateOutputter(std::ostream& stream)
    {
        return new CustomOutputter(stream);
    }
};


#endif /* SRC_BENCHMARK_CUSTOM_OUTPUTTER_H_ */
