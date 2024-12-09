#include "int_map.hpp"
#include "bench.hpp"
#include <iostream>

using namespace std;

struct Dummy {};

int main(int argc, char* argv[]) {

    int warmupCount = 1000000;
    int measureCount = 1000000;
    int capacity = 100000;

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

    cout << "Benchmarking put..." << endl;
    bench.reset();
    for (int i = 0; i < iterations; i++) {
        bench.mark();
        map->put(i, dummy);
        bench.measure();
    }
    bench.printResults();
    cout << endl;

    cout << "Benchmarking get..." << endl;
    bench.reset();
    for (int i = 0; i < iterations; i++) {
        bench.mark();
        volatile auto val = map->get(i); // volatile to prevent optimization out
        bench.measure();
    }
    bench.printResults();
    cout << endl;

    cout << "Benchmarking remove..." << endl;
    bench.reset();
    for (int i = 0; i < iterations; i++) {
        bench.mark();
        map->remove(i);
        bench.measure();
    }
    bench.printResults();
    cout << endl;

    delete map; 
    return 0;
}
