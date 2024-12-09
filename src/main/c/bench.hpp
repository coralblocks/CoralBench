/* 
 * Copyright 2024 (c) CoralBlocks - http://www.coralblocks.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 */
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
    int measurementCount;
    long long sum;
    long long minTime;
    long long maxTime;
    int size;
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
