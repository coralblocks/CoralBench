#include "bench.hpp"
using namespace std;

Bench::Bench(int warmupCount)
    : warmupCount(warmupCount),
      measurementCount(0),
      sum(0),
      minTime(numeric_limits<long long>::max()),
      maxTime(numeric_limits<long long>::min()) {}

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
    }
}

void Bench::reset() {
    measurementCount = 0;
    sum = 0;
    minTime = numeric_limits<long long>::max();
    maxTime = numeric_limits<long long>::min();
}

void Bench::report() const {
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
    // Convert nanos to the largest possible unit
    // nanos < 1,000 -> nanos
    // nanos < 1,000,000 -> micros
    // nanos < 1,000,000,000 -> millis
    // else seconds

    if (nanos < 1000.0) {
        return {nanos, "nanos"};
    } else if (nanos < 1'000'000.0) {
        double micros = nanos / 1000.0;
        return {micros, "micros"};
    } else if (nanos < 1'000'000'000.0) {
        double millis = nanos / 1'000'000.0;
        return {millis, "millis"};
    } else {
        double seconds = nanos / 1'000'000'000.0;
        return {seconds, "seconds"};
    }
}
