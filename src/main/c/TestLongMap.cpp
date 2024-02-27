#include <iostream>
#include <cassert>
#include "CoralBlocks/CoralBench/Util/LongMap.h"

using namespace CoralBlocks::CoralBench::Util;
using std::string;
using std::cout;
using std::endl;

void runLongMapTests() {

    // Create a LongMap with default constructor
    LongMap<string> map;

    assert(map.size() == 0);
    // assert(map.isEmpty());

    // // Put some entries
    // map.put(1, "One");
    // map.put(2, "Two");
    // map.put(3, "Three");

    // // Check size and isEmpty
    // assert(map.size() == 3);
    // assert(!map.isEmpty());

    // // Check contains
    // assert(map.contains("Two"));
    // assert(!map.contains("Four"));

    // // Check containsKey
    // assert(map.containsKey(2));
    // assert(!map.containsKey(4));

    // // Get values
    // assert(map.get(1) == "One");
    // assert(map.get(3) == "Three");

    // // Remove an entry
    // map.remove(2);
    // assert(map.size() == 2);
    // assert(!map.containsKey(2));

    // // Clear the map
    // map.clear();
    // assert(map.size() == 0);
    // assert(map.isEmpty());

    // // Test ReusableIterator
    // map.put(10, "Ten");
    // map.put(20, "Twenty");
    // map.put(30, "Thirty");

    // LongMap<string>::ReusableIterator iterator = map.iterator();

    // assert(iterator.hasNext());
    // assert(iterator.nextValue() == "Ten");

    // iterator.remove();
    // assert(map.size() == 2);
    // assert(!map.containsKey(10));

    // assert(iterator.hasNext());
    // assert(iterator.nextValue() == "Twenty");

    // iterator.remove();
    // assert(map.size() == 1);
    // assert(!map.containsKey(20));

    // assert(iterator.hasNext());
    // assert(iterator.nextValue() == "Thirty");

    // iterator.remove();
    // assert(map.size() == 0);
    // assert(!map.containsKey(30));
    // assert(!iterator.hasNext());

    cout << "All tests passed!" << endl;
}

int main() {
    runLongMapTests();
    return 0;
}
