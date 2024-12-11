#include <iostream>
#include "bench.hpp"

using namespace std;

long doSomething(int load, int i) {
    long x = 0;
    for (int j = 0; j < load; ++j) {
        long pow = (i % 8) * (i % 16);
        if (i % 2 == 0) {
            x += pow;
        } else {
            x -= pow;
        }
    }
    return x;
}

int main(int argc, char* argv[]) {

    int warmupIterations = (argc > 1) ? atoi(argv[1]) : 1'000'000;
    int measurementIterations = (argc > 2) ? atoi(argv[2]) : 9'000'000;
    int totalIterations = measurementIterations + warmupIterations;
    int load = 10000;

    Bench* bench = new Bench(warmupIterations);

    long x = 0;

    for (int i = 0; i < totalIterations; ++i) {
        bench->mark();
        x += doSomething(load, i);
        bench->measure();
    }

    cout << "Value computed: " << x << endl;
    cout << "warmup=" << warmupIterations << " measurements=" << measurementIterations << endl;

    bench->printResults();
    
    cout << endl;

    delete bench;
    
    return 0;
}
