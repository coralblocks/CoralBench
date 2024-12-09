#ifndef BENCH_HPP
#define BENCH_HPP

#include <chrono>
#include <iostream>
#include <limits>
#include <iomanip>
#include <string>
#include <cmath>
#include <map>
#include <sstream>

class Bench {
public:
    Bench(int warmupCount = 0);
    ~Bench();

    void mark();
    void measure();
    void reset();
    void printResults() const;

private:
    int warmupCount;
    int measurementCount;   // total measurements done
    long long sum;          // sum of all measurements post-warmup
    long long minTime;      // min measurement post-warmup
    long long maxTime;      // max measurement post-warmup
    int size;               // number of measurements post-warmup
    std::map<long long, long long>* results;
    std::chrono::steady_clock::time_point startTime;
    
    static std::string formatWithCommas(long long value);
    static std::pair<double, std::string> formatTime(double nanos);
    static std::string formatPercentage(double perc);
    static double roundToDecimals(double d, int decimals);
    void printPercentiles() const;
    void addPercentile(double perc) const;
};

#endif // BENCH_HPP
