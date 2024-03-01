#include "CoralBlocks/CoralBench/Bench.cpp"

using namespace CoralBlocks::CoralBench;

int main() {

    const int warmupIterations = 1000000;
    const int measurementIterations = 1000000;

    Bench& bench = *(new Bench(warmupIterations)); // don't want to use -> to dereference

    while (bench.getCount() < measurementIterations + warmupIterations) {
        Bench::doSleep(bench);
    }

    bench.printResults();

    bench.reset(false); // false because we don't want to repeat warmup

    while (bench.getCount() < measurementIterations) {
        Bench::doSleep(bench);
    }

    bench.printResults();

    delete &bench;

    return 0;
}
