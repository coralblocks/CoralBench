#pragma once

#include <iterator>
#include <stdexcept>

template <typename E>
class LongMap {

    private:
        static const int DEFAULT_INITIAL_CAPACITY = 128;
        static constexpr float DEFAULT_LOAD_FACTOR = 0.80f;

        struct Entry {
            long key;
            E value;
            Entry* next;
        };

        Entry** data;
        int lengthMinusOne;
        int count;
        int threshold;
        float loadFactor;
        Entry* poolHead;
        long currIteratorKey;

        void rehash();

        Entry* getEntryFromPool(long key, E value, Entry* next);

        void releaseEntryBackToPool(Entry* e);

        int toArrayIndex(long key) const;

    public:
        LongMap() : LongMap(DEFAULT_INITIAL_CAPACITY, DEFAULT_LOAD_FACTOR) {}
        LongMap(int initialCapacity) : LongMap(initialCapacity, DEFAULT_LOAD_FACTOR) {}
        LongMap(int initialCapacity, float loadFactor);
        ~LongMap();

        // Copy constructor and assignment operator are deleted to avoid unintended behavior
        LongMap(const LongMap&) = delete;
        LongMap& operator=(const LongMap&) = delete;

        int size() const;
        bool isEmpty() const;
        bool contains(const E& value) const;
        bool containsKey(long key) const;
        E get(long key) const;
        E put(long key, const E& value);
        E remove(long key);
        void clear();

        class ReusableIterator : public std::iterator<std::input_iterator_tag, E> {

            private:
                LongMap<E>* outer;
                int size;
                int index;
                int dataIndex;
                Entry* prev;
                Entry* next;
                Entry* entry;
                bool wasRemoved;

            public:
                ReusableIterator(LongMap<E>& outerPtr) : outer(outerPtr) { }
                ~ReusableIterator() = default;

                void reset();
                bool hasNext() const;
                E nextValue();
                void remove();
        };

        ReusableIterator iterator();

        private:

            ReusableIterator* reusableIter; 

};