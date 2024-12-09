#include "int_map.hpp"
#include "bench.hpp"
#include <iostream>

using namespace std;

struct Dummy {};

int main(int argc, char* argv[]) {

    int warmupCount = 1000000;    // default
    int measureCount = 1000000;   // default
    int capacity = 100000;        // default

    if (argc > 1) {
        warmupCount = atoi(argv[1]);
    }
    if (argc > 2) {
        measureCount = atoi(argv[2]);
    }
    if (argc > 3) {
        capacity = atoi(argv[3]);
    }

    cout << "\nArguments: warmup=" << warmupCount << " measurements=" << measureCount << " mapCapacity=" << capacity << endl << endl;

    int iterations = warmupCount + measureCount;

    IntMap<Dummy>* map = new IntMap<Dummy>(capacity);
    Bench bench(warmupCount);
    Dummy dummy;

    // Benchmark for put
    cout << "Benchmarking put..." << endl;
    for (int i = 0; i < iterations; i++) {
        bench.mark();
        map->put(i, dummy);
        bench.measure();
    }
    bench.report();
    cout << endl;

    // Benchmark for get
    cout << "Benchmarking get..." << endl;
    bench.reset(); // Reset the bench before next benchmark
    for (int i = 0; i < iterations; i++) {
        bench.mark();
        volatile auto val = map->get(i); // volatile to prevent optimization out
        bench.measure();
    }
    bench.report();
    cout << endl;

    // Benchmark for remove
    cout << "Benchmarking remove..." << endl;
    bench.reset(); // Reset before remove benchmark
    for (int i = 0; i < iterations; i++) {
        bench.mark();
        map->remove(i);
        bench.measure();
    }
    bench.report();
    cout << endl;

    delete map; 
    return 0;
}
