#include <iostream>
#include <chrono>
#include <string>
#include <cstdlib>
#include "bench.hpp"

using namespace std;

void sleepFor(long nanos) {
    auto start = chrono::high_resolution_clock::now();
    while (true) {
        auto now = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::nanoseconds>(now - start).count();
        if (elapsed >= nanos) {
            break;
        }
    }
}

void doSleep(Bench* bench) {
    bench->mark(); // <===== timer starts
    sleepFor(1000);
    bench->measure(); // <===== timer stops
}

int main(int argc, char* argv[]) {
    const int warmupIterations = (argc > 1) ? stoi(argv[1]) : 1'000'000;
    const int measurementIterations = (argc > 2) ? stoi(argv[2]) : 2'000'000;
    const int totalIterations = measurementIterations + warmupIterations;

    // Specify the number of warmup iterations to ignore
    Bench* bench = new Bench(warmupIterations);

    // Perform warmup + measurement iterations
    while (bench->getIterations() < totalIterations) {
        doSleep(bench);
    }

    bench->printResults();

    delete bench;

    return 0;
}
