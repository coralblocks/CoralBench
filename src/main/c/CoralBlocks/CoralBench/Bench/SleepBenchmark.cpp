#include "../Bench.cpp"

using namespace CoralBlocks::CoralBench;

struct timespec ts;

long get_nano_ts() {
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

void sleepFor(long nanos) {
    long total = get_nano_ts();
    while(get_nano_ts() - total < nanos);
}

void doSleep(Bench& bench) {
    bench.mark();
    sleepFor(1000);
    bench.measure();
}

int main() {

    const int warmupIterations = 1000000;
    const int measurementIterations = 1000000;

    Bench& bench = *(new Bench(warmupIterations)); // don't want to use -> to dereference

    while (bench.getCount() < measurementIterations + warmupIterations) {
        doSleep(bench);
    }

    bench.printResults();

    delete &bench;

    return 0;
}
