#include <iostream>
#include <cassert>
#include "CoralBlocks/CoralBench/Util/FastObjectList.h"

using namespace CoralBlocks::CoralBench::Util;
using namespace std;

int main() {
    // Create an empty FastObjectList
    FastObjectList<int> list(5);
    assert(list.size() == 0);
    assert(list.getCapacity() == 5);
    assert(list.isEmpty());
    assert(!list.isAtCapacity());

    // Add elements to FastObjectList
    int one = 1;
    int two = 2;
    int three = 3;

    assert(list.add(one));
    assert(list.add(two));
    assert(list.size() == 2);
    assert(!list.isEmpty());

    // Remove elements from FastObjectList
    assert(*(list.removeLast()) == 2);
    assert(list.size() == 1);

    assert(*(list.removeFirst()) == 1);
    assert(list.size() == 0);
    assert(list.isEmpty());

    // Iterator functionality
    list.add(one);
    list.add(two);
    list.add(three);

    FastObjectList<int>::ReusableIterator& iter = list.iterator();
    assert(iter.hasNext());
    assert(*iter.next() == 1);

    iter.reset();
    assert(iter.hasNext());
    assert(*iter.next() == 1);
    assert(*iter.next() == 2);
    assert(*iter.next() == 3);
    assert(!iter.hasNext());

    cout << "All tests passed!" << endl;

    // If any assert fails, the program will terminate with an error message.
    // If everything passes, no output will be generated.
    return 0;
}
