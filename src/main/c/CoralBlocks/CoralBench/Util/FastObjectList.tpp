
namespace CoralBlocks::CoralBench::Util {

    template <typename E>
    FastObjectList<E>::FastObjectList(int initialCapacity) {
        values = new E*[initialCapacity]();
        pointer = -1;
        removed = 0;
        capacity = initialCapacity;
        reusableIter = new ReusableIterator(this);
    }

    template <typename E>
    FastObjectList<E>::~FastObjectList() {
        delete[] values;
    }

    template <typename E>
    int FastObjectList<E>::size() const {
        return pointer + 1 - removed;
    }

    template <typename E>
    int FastObjectList<E>::getCapacity() const {
        return capacity;
    }

    template <typename E>
    bool FastObjectList<E>::isEmpty() const {
        return size() == 0;
    }

    template <typename E>
    bool FastObjectList<E>::isAtCapacity() const {
        return pointer + 1 == capacity;
    }

    template <typename E>
    bool FastObjectList<E>::add(E& value) {
        if (pointer + 1 == capacity) return false;
        values[++pointer] = &value;
        return true;
    }

    template <typename E>
    int FastObjectList<E>::grow() {
        int newCapacity = capacity + capacity / 2;
        E** newValues = new E*[newCapacity]();
        std::copy(values, values + capacity, newValues);
        delete[] values;
        values = newValues;
        capacity = newCapacity;
        return newCapacity;
    }

    template <typename E>
    E* FastObjectList<E>::removeLast() {
        if (isEmpty()) throw std::out_of_range("Cannot remove from an empty list");
        for (int i = pointer; i >= 0; i--) {
            if (values[i] != nullptr) {
                E* old = values[i];
                values[i] = nullptr;
                removed++;
                return old;
            }
        }
        throw std::logic_error("Should never happen!");
    }

    template <typename E>
    E* FastObjectList<E>::removeFirst() {
        if (isEmpty()) throw std::out_of_range("Cannot remove from an empty list");
        for (int i = 0; i <= pointer; i++) {
            if (values[i] != nullptr) {
                E* save = values[i];
                values[i] = nullptr;
                removed++;
                return save;
            }
        }
        throw std::logic_error("Should never happen!");
    }

    template <typename E>
    bool FastObjectList<E>::remove(E& value) {
        return remove(value, false);
    }

    template <typename E>
    bool FastObjectList<E>::remove(E& value, bool fromTail) {
        if (!fromTail) {
            for (int i = 0; i <= pointer; i++) {
                if (*values[i] == value) {
                    values[i] = nullptr;
                    removed++;
                    return true;
                }
            }
        } else {
            for (int i = pointer; i >= 0; i--) {
                if (values[i] == value) {
                    values[i] = nullptr;
                    removed++;
                    return true;
                }
            }
        }
        return false;
    }

    template <typename E>
    E* FastObjectList<E>::remove(int index) {
        return remove(index, false);
    }

    template <typename E>
    E* FastObjectList<E>::remove(int index, bool fromTail) {
        if (index >= size()) throw std::out_of_range("Cannot remove index " + std::to_string(index) + " from a list of size " + std::to_string(size()) + " (fromTail=" + std::to_string(fromTail) + ")");
        if (!fromTail) {
            int count = -1;
            for (int i = 0; i <= pointer; i++) {
                if (values[i] != nullptr && ++count == index) {
                    E save = values[i];
                    values[i] = nullptr;
                    removed++;
                    return save;
                }
            }
        } else {
            int count = -1;
            for (int i = pointer; i >= 0; i--) {
                if (values[i] != nullptr && ++count == index) {
                    E save = values[i];
                    values[i] = nullptr;
                    removed++;
                    return save;
                }
            }
        }
        return nullptr;
    }

    template <typename E>
    E* FastObjectList<E>::first() {
        if (isEmpty()) throw std::out_of_range("Cannot get first element from an empty list");
        for (int i = 0; i <= pointer; i++) {
            if (values[i] != nullptr) {
                return values[i];
            }
        }
        throw std::logic_error("Should never happen!");
    }

    template <typename E>
    E* FastObjectList<E>::last() {
        if (isEmpty()) throw std::out_of_range("Cannot get last element from an empty list");
        for (int i = pointer; i >= 0; i--) {
            if (values[i] != nullptr) {
                return values[i];
            }
        }
        throw std::logic_error("Should never happen!");
    }

    template <typename E>
    void FastObjectList<E>::clear() {
        pointer = -1;
        removed = 0;
    }

    template <typename E>
    typename FastObjectList<E>::ReusableIterator& FastObjectList<E>::iterator() {
        reusableIter->reset();
        return *reusableIter;
    }

        template <typename E>
    void FastObjectList<E>::ReusableIterator::reset() {
        size = outer->size();
        pointer = 0;
        returned = 0;
    }

    template <typename E>
    bool FastObjectList<E>::ReusableIterator::hasNext() {
        return returned < size;
    }

    template <typename E>
    E* FastObjectList<E>::ReusableIterator::next() {
        E* value;
        while ((value = outer->values[pointer++]) == nullptr);
        returned++;
        return value;
    }
}
