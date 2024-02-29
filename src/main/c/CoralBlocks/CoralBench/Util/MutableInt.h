#pragma once

#include <iostream>
#include <stdexcept>

namespace CoralBlocks::CoralBench::Util {

    class MutableInt {

    private:

        static const int DEFAULT_VALUE = 0;

        int value;
        bool isNull;

    public:
        MutableInt(int value);
        MutableInt();

        MutableInt(const MutableInt& other);  // Copy constructor
        MutableInt(MutableInt&& other) noexcept;  // Move constructor
        MutableInt& operator=(const MutableInt& other);  // Copy assignment
        MutableInt& operator=(MutableInt&& other) noexcept;  // Move assignment

        void set(int value);
        bool isNullValue() const;
        void setNull();
        int get() const;

        bool operator==(const MutableInt& other) const;  // Equality operator
    };
}
