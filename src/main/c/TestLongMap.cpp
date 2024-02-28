#include <iostream>
#include <cassert>
#include <string>
#include "CoralBlocks/CoralBench/Util/LongMap.h"

using namespace CoralBlocks::CoralBench::Util;
using std::string;
using std::cout;
using std::endl;
using std::to_string;

void runLongMapTests() {

    // Create a LongMap with default constructor
    LongMap<string> map;

    assert(map.size() == 0);
    assert(map.isEmpty());

    string one = "One";
    string two = "Two";
    string three = "Three";
    string four = "Four";
    string ten = "Ten";
    string twenty = "Twenty";
    string thirty = "Thirty";

    // Put some entries
    map.put(1, one);
    map.put(2, two);
    map.put(3, three);

    // Check size and isEmpty
    assert(map.size() == 3);
    assert(!map.isEmpty());

    // Check contains
    assert(map.contains(two));
    assert(!map.contains(four));

    // Check containsKey
    assert(map.containsKey(2));
    assert(!map.containsKey(4));

    // Get values
    assert(*map.get(1) == one);
    assert(*map.get(3) == three);

    // Remove an entry
    map.remove(2);
    assert(map.size() == 2);
    assert(!map.containsKey(2));

    // Clear the map
    map.clear();
    assert(map.size() == 0);
    assert(map.isEmpty());

    // Test ReusableIterator
    map.put(10, ten);
    map.put(20, twenty);
    map.put(30, thirty);

    LongMap<string>::ReusableIterator* iterator = map.iterator();

    assert(iterator->hasNext());
    assert(*iterator->nextValue() == ten);

    iterator->remove();
    assert(map.size() == 2);
    assert(!map.containsKey(10));

    assert(iterator->hasNext());
    assert(*iterator->nextValue() == twenty);

    iterator->remove();
    assert(map.size() == 1);
    assert(!map.containsKey(20));

    assert(iterator->hasNext());
    assert(*iterator->nextValue() == thirty);

    iterator->remove();
    assert(map.size() == 0);
    assert(!map.containsKey(30));
    assert(!iterator->hasNext());

    cout << "All tests passed!" << endl;
}

int main() {
    runLongMapTests();
    return 0;
}
