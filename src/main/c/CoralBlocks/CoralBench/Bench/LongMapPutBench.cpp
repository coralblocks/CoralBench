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

    string filler("FILLER");

    for (int pass = 0; pass < passes; pass++) {

        map.clear();
        bench.reset();
        rand.reset();

        for (int i = 0; i < iterations; i++) {

            long key = rand.nextLong();  // this is deterministic (pseudo-random)

            bench.mark();
            map.put(key, filler);
            bench.measure();
        }

        bench.printResults();
    }

    delete mapPointer;
    delete benchPointer;
    delete randPointer;

    return 0;
}
