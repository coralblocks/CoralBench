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
#include "bench.hpp"
using namespace std;

Bench::Bench(int warmupCount)
    : warmupCount(warmupCount),
      measurementCount(0),
      sum(0),
      minTime(numeric_limits<long long>::max()),
      maxTime(numeric_limits<long long>::min()),
      size(0) {

        results = new map<long long, long long>();

}

Bench::~Bench() {
    delete results;
}

void Bench::mark() {
    startTime = chrono::steady_clock::now();
}

void Bench::measure() {
    auto endTime = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count();
    measure(elapsed);
}

bool Bench::measure(long long elapsed) {

    bool isToMeasure = ++measurementCount > warmupCount;

    if (isToMeasure) {
        sum += elapsed;
        if (elapsed < minTime) minTime = elapsed;
        if (elapsed > maxTime) maxTime = elapsed;

        // Increment the frequency of this elapsed time
        auto it = results->find(elapsed);
        if (it == results->end()) {
            results->insert({elapsed, 1});
        } else {
            it->second++;
        }
        size++;
    }
    
    return isToMeasure;
}

int Bench::getIterations() const {
	return measurementCount;
}

int Bench::getMeasurements() const {
    return size;
}

void Bench::reset() {
	reset(false);
}

void Bench::reset(bool repeatWarmup) {
    measurementCount = 0;
    sum = 0;
    if (!repeatWarmup) warmupCount = 0;
    minTime = numeric_limits<long long>::max();
    maxTime = numeric_limits<long long>::min();
    results->clear();
    size = 0;
}

bool Bench::isWarmingUp() const {
	return warmupCount <= measurementCount;
}

double Bench::avg() const {
    const int effectiveCount = measurementCount - warmupCount;
    if (effectiveCount <= 0) {
        return 0;
    }
    const double avg = static_cast<double>(sum) / effectiveCount;
    const double rounded = round(avg * 100.0) / 100.0;
    return rounded;
}

double Bench::getAverage() const {
	return avg();
}	

void Bench::printResults() const {
	printResults(true);
}

void Bench::printResults(bool includePercentiles) const {

    int effectiveCount = measurementCount - warmupCount;

    string effCountStr = formatWithCommas(effectiveCount);
    string warmupStr = formatWithCommas(warmupCount);
    string totalStr = formatWithCommas(measurementCount);

    cout << "Measurements: " << effCountStr
         << " | Warm-Up: " << warmupStr
         << " | Iterations: " << totalStr << endl;
         
  	if (effectiveCount > 0) {

	    auto [avgVal, avgUnit] = formatTime(avg());
	    auto [minVal, minUnit] = formatTime(static_cast<double>(minTime));
	    auto [maxVal, maxUnit] = formatTime(static_cast<double>(maxTime));
	
	    cout << fixed << setprecision(3);
	    cout << "Avg Time: " << avgVal << " " << avgUnit << " | "
	         << "Min Time: " << minVal << " " << minUnit << " | "
	         << "Max Time: " << maxVal << " " << maxUnit << endl;
	
	    if (includePercentiles) printPercentiles();
	}
	
	cout << endl;
}

string Bench::formatWithCommas(long long value) {
    string numStr = to_string(value);
    int insertPosition = static_cast<int>(numStr.length()) - 3;
    while (insertPosition > 0) {
        numStr.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return numStr;
}

pair<double, string> Bench::formatTime(double nanos) {
    if (nanos >= 1'000'000'000.0) {
        double seconds = nanos / 1'000'000'000.0;
        return {roundToDecimals(seconds, 3), seconds > 1 ? "seconds" : "second"};
    } else if (nanos >= 1'000'000.0) {
        double millis = nanos / 1'000'000.0;
        return {roundToDecimals(millis, 3), millis > 1 ? "millis" : "milli"};
    } else if (nanos >= 1000.0) {
        double micros = nanos / 1000.0;
        return {roundToDecimals(micros, 3), micros > 1 ? "micros" : "micro"};
    } else {
        double ns = nanos;
        return {roundToDecimals(ns, 3), ns > 1 ? "nanos" : "nano"};
    }
}

double Bench::roundToDecimals(double d, int decimals) {
    double pow10 = pow(10.0, decimals);
    return round(d * pow10) / pow10;
}

void Bench::printPercentiles() const {

    if (size == 0) return;

    double percentiles[] = {0.75, 0.90, 0.99, 0.999, 0.9999, 0.99999};

    for (double p : percentiles) {
        addPercentile(p);
    }
}

string Bench::formatPercentage(double perc) {
    double p = perc * 100.0;

    ostringstream oss;
    oss << fixed << setprecision(6) << p;

    string s = oss.str();
    // remove trailing zeros
    while (s.back() == '0') {
        s.pop_back();
    }

    // if the last character is now a '.', remove it
    if (s.back() == '.') {
        s.pop_back();
    }

    // Append the '%' sign
    s += "%";

    return s;
}

void Bench::addPercentile(double perc) const {

    if (results->empty()) return;

    long long target = static_cast<long long>(llround(perc * size));
    if (target == 0) return;
    if (target > size) target = size;

    // Iterate through the map to find the top element at position target
    long long iTop = 0;
    long long sumTop = 0;
    long long maxTop = -1;

    for (auto &entry : *results) {
        long long time = entry.first;
        long long count = entry.second;

        for (int i = 0; i < count; i++) {
            iTop++;
            sumTop += time;
            if (iTop == target) {
                maxTop = time;
                goto FOUND;
            }
        }
    }

FOUND:;

    double avgTop = static_cast<double>(sumTop) / iTop;
    auto [avgVal, avgUnit] = formatTime(avgTop);
    auto [maxVal, maxUnit] = formatTime(static_cast<double>(maxTop));

    cout << fixed << setprecision(3);
    cout << formatPercentage(perc) << " = [avg: " << avgVal << " " << avgUnit
         << ", max: " << maxVal << " " << maxUnit << "]\n";
}
