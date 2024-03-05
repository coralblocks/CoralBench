#include "../Bench.cpp"

using namespace CoralBlocks::CoralBench;
using namespace std;

long doSomething(int load, int i) {
    long x = 0;

    for (int j = 0; j < load; j++) {
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

    const int warmupIterations = atoi(argv[1]);
    const int measurementIterations = atoi(argv[2]);
    const int load = atoi(argv[3]);

    Bench& bench = *(new Bench(warmupIterations)); // don't want to use -> to dereference

    long x = 0;

    for (int i = 0; i < measurementIterations + warmupIterations; i++) {
        bench.mark();
        x += doSomething(load, i);
        bench.measure();
    }

    cout << "Value computed: " << x << endl;

    bench.printResults();

    bench.reset(false); // false because we don't want to repeat warmup

    x = 0;

    for (int i = 0; i < measurementIterations; i++) {
        bench.mark();
        x += doSomething(load, i);
        bench.measure();
    }

    cout << "Value computed: " << x << endl;

    bench.printResults();

    delete &bench;

    return 0;
}
