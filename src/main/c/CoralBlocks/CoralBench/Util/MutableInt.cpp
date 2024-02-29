#include "MutableInt.h"

namespace CoralBlocks::CoralBench::Util {

    MutableInt::MutableInt(int value) {
        set(value);
    }

    MutableInt::MutableInt() : MutableInt(DEFAULT_VALUE) {}

    MutableInt::MutableInt(const MutableInt& other) { // Copy constructor
        this->value = other.value;
        isNull = other.isNull;
    }

    MutableInt::MutableInt(MutableInt&& other) noexcept { // Move constructor
        value = other.value;
        isNull = other.isNull;
        other.setNull();  // Ensure the source object is set to null after moving
    }

    MutableInt& MutableInt::operator=(const MutableInt& other) { // Copy assignment
        if (this != &other) {
            value = other.value;
            isNull = other.isNull;
        }
        return *this;
    }

    MutableInt& MutableInt::operator=(MutableInt&& other) noexcept { // Move assignment
        if (this != &other) {
            value = other.value;
            isNull = other.isNull;
            other.setNull();  // Ensure the source object is set to null after moving
        }
        return *this;
    }

    void MutableInt::set(int value) {
        this->value = value;
        this->isNull = false;
    }

    bool MutableInt::isNullValue() const {
        return isNull;
    }

    void MutableInt::setNull() {
        isNull = true;
    }

    int MutableInt::get() const {
        if (isNull) throw std::runtime_error("NullPointerException");
        return value;
    }

    bool MutableInt::operator==(const MutableInt& other) const {
        return (isNull && other.isNull) || (value == other.value && isNull == other.isNull);
    }
}
