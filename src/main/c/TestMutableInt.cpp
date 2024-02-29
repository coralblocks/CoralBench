#include "CoralBlocks/CoralBench/Util/MutableInt.h"
#include <cassert>

using namespace CoralBlocks::CoralBench::Util;

int main() {
    // Test default constructor and set method
    MutableInt mutableInt1;
    assert(mutableInt1.get() == 0);
    assert(!mutableInt1.isNullValue());
    mutableInt1.set(10);
    assert(!mutableInt1.isNullValue());
    assert(mutableInt1.get() == 10);

    // Test parameterized constructor
    MutableInt mutableInt2(5);
    assert(!mutableInt2.isNullValue());
    assert(mutableInt2.get() == 5);

    // Test copy constructor
    MutableInt mutableInt3 = mutableInt1;
    assert(!mutableInt3.isNullValue());
    assert(mutableInt3.get() == 10);

    // Test move constructor
    MutableInt mutableInt4 = std::move(mutableInt2);
    assert(!mutableInt4.isNullValue());
    assert(mutableInt4.get() == 5);
    assert(mutableInt2.isNullValue());  // After move, the source object should be null

    // Test copy assignment
    MutableInt mutableInt5;
    mutableInt5 = mutableInt1;
    assert(!mutableInt5.isNullValue());
    assert(mutableInt5.get() == 10);

    // Test move assignment
    MutableInt mutableInt6;
    mutableInt6 = std::move(mutableInt4);
    assert(!mutableInt6.isNullValue());
    assert(mutableInt6.get() == 5);
    assert(mutableInt4.isNullValue());  // After move, the source object should be null

    // Test equality operator
    assert(mutableInt1 == mutableInt5);
    assert((mutableInt4 == mutableInt6) == false);
    assert(!(mutableInt1 == mutableInt2));

    // Test setNull method
    mutableInt1.setNull();
    assert(mutableInt1.isNullValue());

    std::cout << "All tests passed successfully!" << std::endl;

    return 0;
}
