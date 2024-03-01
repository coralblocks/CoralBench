#include <iostream>
#include "CoralBlocks/CoralBench/Util/ThreadPinning.cpp"

using namespace CoralBlocks::CoralBench::Util;
using namespace std;

int main() {

    int coreId = 1;

    ThreadPinning::pinCurrentThread(coreId);

    long total = 0;

    for(long i = 0; i < 10000000000; i++) {
        total += i % 2;
    }

    cout << total << endl;

    return 0;
}