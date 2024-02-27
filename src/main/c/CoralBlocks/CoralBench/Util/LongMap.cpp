#include "LongMap.h"
#include "MathUtils.h"
#include <stdexcept>
#include <string>

using namespace CoralBlocks::CoralBench::Util;
using std::invalid_argument;
using std::to_string;

template <typename E>
LongMap<E>::LongMap() : LongMap(DEFAULT_INITIAL_CAPACITY, DEFAULT_LOAD_FACTOR) {}

template <typename E>
LongMap<E>::LongMap(int initialCapacity) : LongMap(initialCapacity, DEFAULT_LOAD_FACTOR) {}

template <typename E>
LongMap<E>::LongMap(int initialCapacity, float loadFactor) {
    if (!MathUtils::isPowerOfTwo(initialCapacity)) {
        throw invalid_argument("Size must be power of two: " + to_string(initialCapacity));
    }

    data = new Entry*[initialCapacity]();
    lengthMinusOne = initialCapacity - 1;
    this->loadFactor = loadFactor;
    threshold = static_cast<int>(initialCapacity * loadFactor);
}

template <typename E>
LongMap<E>::~LongMap() {
    clear();
    delete[] data;
}

template <typename E>
int LongMap<E>::size() const {
    return count;
}

template <typename E>
bool LongMap<E>::isEmpty() const {
    return size() == 0;
}

template <typename E>
bool LongMap<E>::contains(const E& value) const {
    for (int i = lengthMinusOne; i >= 0; i--) {
        Entry* e = data[i];
        while (e != nullptr) {
            if (e->value == value) {
                return true;
            }
            e = e->next;
        }
    }
    return false;
}

template <typename E>
bool LongMap<E>::containsKey(long key) const {
    Entry* e = data[toArrayIndex(key)];
    while (e != nullptr) {
        if (e->key == key) {
            return true;
        }
        e = e->next;
    }
    return false;
}

template <typename E>
E LongMap<E>::get(long key) const {
    Entry* e = data[toArrayIndex(key)];
    while (e != nullptr) {
        if (e->key == key) {
            return e->value;
        }
        e = e->next;
    }
    return nullptr;
}

template <typename E>
E LongMap<E>::put(long key, const E& value) {

    if (value == nullptr) {
        throw invalid_argument("Cannot put null value!");
    }

    int index = toArrayIndex(key);

    Entry* e = data[index];

    while (e != nullptr) {
        if (e->key == key) {
            E old = e->value;
            e->value = value;
            return old;
        }
        e = e->next;
    }

    if (count >= threshold) {
        rehash();
        index = toArrayIndex(key); // lengthMinusOne has changed!
        data[index] = getEntryFromPool(key, value, data[index]);
    } else {
        data[index] = getEntryFromPool(key, value, data[index]);
    }

    count++;

    return nullptr;
}

template <typename E>
E LongMap<E>::remove(long key) {

    int index = toArrayIndex(key);

    Entry* e = data[index];
    Entry* prev = nullptr;

    while (e != nullptr) {
        if (e->key == key) {
            if (prev != nullptr) {
                prev->next = e->next;
            } else {
                data[index] = e->next;
            }

            E oldValue = e->value;
            releaseEntryBackToPool(e);
            count--;

            return oldValue;
        }

        prev = e;
        e = e->next;
    }

    return nullptr;
}

template <typename E>
void LongMap<E>::clear() {
    for (int index = lengthMinusOne; index >= 0; index--) {
        while (data[index] != nullptr) {
            Entry* next = data[index]->next;
            releaseEntryBackToPool(data[index]);
            data[index] = next;
        }
    }

    count = 0;
}

template <typename E>
typename LongMap<E>::ReusableIterator LongMap<E>::iterator() {
    ReusableIterator iter;
    iter.reset();
    return iter;
}

template <typename E>
void LongMap<E>::rehash() {

    int oldCapacity = lengthMinusOne + 1;

    Entry** oldData = data;

    int newCapacity = oldCapacity * 2; // power of two, always!

    data = new Entry*[newCapacity]();
    lengthMinusOne = newCapacity - 1;

    threshold = static_cast<int>(newCapacity * loadFactor);

    for (int i = oldCapacity - 1; i >= 0; i--) {
        Entry* old = oldData[i];

        while (old != nullptr) {
            Entry* e = old;
            old = old->next;

            int index = toArrayIndex(e->key);
            e->next = data[index];
            data[index] = e;
        }
    }

    delete[] oldData;
}

template <typename E>
typename LongMap<E>::Entry* LongMap<E>::getEntryFromPool(long key, E value, Entry* next) {

    Entry* newEntry = poolHead;

    if (newEntry != nullptr) {
        poolHead = newEntry->next;
    } else {
        newEntry = new Entry();
    }

    newEntry->key = key;
    newEntry->value = value;
    newEntry->next = next;

    return newEntry;
}

template <typename E>
void LongMap<E>::releaseEntryBackToPool(Entry* e) {
    e->value = E(); // Set value to default-constructed value
    e->next = poolHead;
    poolHead = e;
}

template <typename E>
int LongMap<E>::toArrayIndex(long key) const {
    return static_cast<int>((static_cast<unsigned int>(key) & 0x7FFFFFFF) & lengthMinusOne);
}