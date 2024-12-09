#ifndef BENCH_HPP
#define BENCH_HPP

#include <chrono>
#include <iostream>
#include <limits>
#include <iomanip>
#include <string>
#include <cmath>

class Bench {
public:
    Bench(int warmupCount = 0);

    void mark();
    void measure();
    void reset();
    void report() const;

private:
    int warmupCount;
    int measurementCount;   // total measurements done
    long long sum;          // sum of all measurements post-warmup
    long long minTime;      // min measurement post-warmup
    long long maxTime;      // max measurement post-warmup
    std::chrono::steady_clock::time_point startTime;
    
    static std::string formatWithCommas(long long value);
    static std::pair<double, std::string> formatTime(double nanos);
};

#endif // BENCH_HPP
