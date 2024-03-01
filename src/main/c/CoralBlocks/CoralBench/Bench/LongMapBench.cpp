#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <unordered_map>

#include "../Bench.cpp"
#include "../Util/ThreadPinning.cpp"
#include "../Util/LongMap.h"
#include "../Util/Random.cpp"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::atoi;
using namespace CoralBlocks::CoralBench;
using namespace CoralBlocks::CoralBench::Util;

long get_nano_ts(struct timespec &ts) {
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

string formatWithCommas(long value) {
    stringstream ss;
    ss.imbue(std::locale("en_US")); // for #,###
    ss << std::fixed << value;
    return ss.str();
}

int main(int argc, char* argv[]) {

    const long THE_SEED = 123456789L;

    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <iterations> <passes> <coreId>" << endl;
        return 1;
    }

    int iterations = atoi(argv[1]);
    int passes = atoi(argv[2]);
    int coreId = atoi(argv[3]);

    if (coreId >= 0) ThreadPinning::pinCurrentThread(coreId);

    LongMap<string>* mapPointer = new LongMap<string>(8388608);  // 2 ^ 23
    Random* randPointer = new Random(THE_SEED);
    Bench* benchPointer = new Bench();

    LongMap<string>& map = *mapPointer;
    Random& rand = *randPointer;
    Bench& bench = *benchPointer;

    struct timespec ts;
    string filler("FILLER");

    cout << "\nBenchmarking put operation...\n";

    for (int pass = 0; pass < passes; pass++) {

        map.clear();  // we will re-insert everything...
        bench.reset();
        rand.reset();

        long time = get_nano_ts(ts);

        for (int i = 0; i < iterations; i++) {
            long key = rand.nextLong();  // this is deterministic (pseudo-random)

            bench.mark();
            map.put(key, filler);
            bench.measure();
        }

        time = get_nano_ts(ts) - time;

        cout << "Total time in nanoseconds: " << formatWithCommas(time) << endl;
        cout << "Final size of map: " << map.size() << endl << endl;
        bench.printResults();
    }

    cout << "Benchmarking get operation...\n";

    for (int pass = 0; pass < passes; pass++) {
        
        bench.reset();
        rand.reset();

        string* gotten;

        long time = get_nano_ts(ts);

        for (int i = 0; i < iterations; i++) {

            long key = rand.nextLong();  // this is deterministic (pseudo-random)

            bench.mark();
            gotten = map.get(key);
            bench.measure();
        }

        time = get_nano_ts(ts) - time;

        cout << "Total time in nanoseconds: " << formatWithCommas(time) << endl;
        cout << "Last object gotten: " << *gotten << endl << endl;
        bench.printResults();
    }

    cout << "Benchmarking remove operation...\n";

    map.clear();

    for (int pass = 0; pass < passes; pass++) {

        rand.reset();
        for (int i = 0; i < iterations; i++) map.put(rand.nextLong(), filler);

        bench.reset();
        rand.reset();

        string* removed;

        long time = get_nano_ts(ts);

        for (int i = 0; i < iterations; i++) {

            long key = rand.nextLong();  // this is deterministic (pseudo-random)

            bench.mark();
            removed = map.remove(key);
            bench.measure();
        }

        time = get_nano_ts(ts) - time;

        cout << "Total time in nanoseconds: " << formatWithCommas(time) << endl;
        cout << "Last object gotten: " << *removed << endl;
        cout << "Final size of map: " << map.size() << endl << endl;
        bench.printResults();
    }

    delete mapPointer;
    delete benchPointer;
    delete randPointer;

    return 0;
}
