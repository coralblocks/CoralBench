/* 
 * Copyright 2024 (c) CoralBlocks - http://www.coralblocks.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 */
#include "int_map.hpp"
#include "bench.hpp"

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

    IntMap<Dummy*>* map = new IntMap<Dummy*>(capacity);
    Dummy* dummy = new Dummy();
    Bench bench(warmupCount);

    cout << "Benchmarking put..." << endl;
    for (int i = 0; i < iterations; i++) {
        bench.mark();
        map->put(i, dummy);
        bench.measure();
    }
    bench.printResults();
    
    cout << "Benchmarking get..." << endl;
    bench.reset(true);
    for (int i = 0; i < iterations; i++) {
        bench.mark();
        volatile auto val = map->get(i); // volatile to avoid optimizing away...
        bench.measure();
    }
    bench.printResults();

    cout << "Benchmarking remove..." << endl;
    bench.reset(true);
    for (int i = 0; i < iterations; i++) {
        bench.mark();
        map->remove(i);
        bench.measure();
    }
    bench.printResults();

    delete dummy;
    delete map; 
    return 0;
}
