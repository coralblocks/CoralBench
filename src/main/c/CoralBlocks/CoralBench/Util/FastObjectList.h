#pragma once

#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <string>

namespace CoralBlocks::CoralBench::Util {

    template <typename E>
    class FastObjectList {

    private:
        E** values;
        int pointer;
        int removed;
        int capacity;

    public:
        FastObjectList(int initialCapacity);
        ~FastObjectList();

        int size() const;
        int getCapacity() const;
        bool isEmpty() const;
        bool isAtCapacity() const;
        bool add(E& value);
        int grow();
        E* removeLast();
        E* removeFirst();
        bool remove(E& value);
        bool remove(E& value, bool fromTail);
        E* remove(int index);
        E* remove(int index, bool fromTail);
        E* first();
        E* last();
        void clear();
        
        class ReusableIterator {
        private:
            FastObjectList<E>* outer;
            int pointer;
            int size;
            int returned;

        public:

            ReusableIterator(FastObjectList<E>* outerPtr) : outer(outerPtr) { }
            ~ReusableIterator() = default;

            ReusableIterator(const ReusableIterator&) = delete;
            ReusableIterator& operator=(const ReusableIterator&) = delete;
            ReusableIterator(ReusableIterator&&) = delete;
            ReusableIterator& operator=(ReusableIterator&&) = delete;

            void reset();
            bool hasNext();
            E& next();
            void remove() = delete; // Remove not supported
        };

        private:
            ReusableIterator* reusableIter;

        public:
            ReusableIterator& iterator();
    };
}

#include "FastObjectList.tpp"

