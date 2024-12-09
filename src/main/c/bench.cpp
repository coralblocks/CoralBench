#include "bench.hpp"
using namespace std;

Bench::Bench(int warmupCount)
    : warmupCount(warmupCount),
      measurementCount(0),
      sum(0),
      minTime(numeric_limits<long long>::max()),
      maxTime(numeric_limits<long long>::min()),
      size(0) {

        results = new std::map<long long, long long>();

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
    measurementCount++;

    if (measurementCount > warmupCount) {
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
}

void Bench::reset() {
    measurementCount = 0;
    sum = 0;
    minTime = numeric_limits<long long>::max();
    maxTime = numeric_limits<long long>::min();
    results->clear();
    size = 0;
}

void Bench::printResults() const {
    int effectiveCount = measurementCount - warmupCount;
    if (effectiveCount <= 0) {
        cout << "Not enough measurements after warmup to report statistics." << endl;
        return;
    }

    double avg = static_cast<double>(sum) / effectiveCount;

    // Print counts with commas
    string effCountStr = formatWithCommas(effectiveCount);
    string warmupStr = formatWithCommas(warmupCount);
    string totalStr = formatWithCommas(measurementCount);

    cout << "Measurements: " << effCountStr
         << " | Warm-Up: " << warmupStr
         << " | Iterations: " << totalStr << endl;

    // Format times
    auto [avgVal, avgUnit] = formatTime(avg);
    auto [minVal, minUnit] = formatTime(static_cast<double>(minTime));
    auto [maxVal, maxUnit] = formatTime(static_cast<double>(maxTime));

    cout << fixed << setprecision(3);
    cout << "Avg Time: " << avgVal << " " << avgUnit << " | "
         << "Min Time: " << minVal << " " << minUnit << " | "
         << "Max Time: " << maxVal << " " << maxUnit << endl;

    printPercentiles();
}

string Bench::formatWithCommas(long long value) {
    // Convert number to string
    std::string numStr = std::to_string(value);
    int insertPosition = static_cast<int>(numStr.length()) - 3;
    while (insertPosition > 0) {
        numStr.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return numStr;
}

pair<double, string> Bench::formatTime(double nanos) {
    // Convert nanos into the largest possible unit
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
    double pow10 = std::pow(10.0, decimals);
    return std::round(d * pow10) / pow10;
}

void Bench::printPercentiles() const {

    if (size == 0) return;

    // Percentiles to print
    double percentiles[] = {0.75, 0.90, 0.99, 0.999, 0.9999, 0.99999};

    for (double p : percentiles) {
        addPercentile(p);
    }
}

std::string Bench::formatPercentage(double perc) {
    double p = perc * 100.0;

    // We'll use a string stream to format up to 6 decimal places initially
    // since some of your percentiles can have many decimal places (like 99.999%).
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6) << p;

    std::string s = oss.str();
    // Now strip trailing zeros:
    // 1. Remove trailing zeros
    while (s.back() == '0') {
        s.pop_back();
    }

    // 2. If the last character is now a '.', remove it
    if (s.back() == '.') {
        s.pop_back();
    }

    // Append the '%' sign
    s += "%";
    return s;
}

void Bench::addPercentile(double perc) const {

    if (results->empty()) return;

    long long target = static_cast<long long>(std::llround(perc * size));
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

    // Print percentile line
    // example: "75% = [avg: 1.016 micros, max: 1.042 micros]"
    cout << fixed << setprecision(3);
    cout << formatPercentage(perc) << " = [avg: " << avgVal << " " << avgUnit
         << ", max: " << maxVal << " " << maxUnit << "]\n";
}
