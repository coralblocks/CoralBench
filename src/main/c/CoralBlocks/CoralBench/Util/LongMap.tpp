
namespace CoralBlocks::CoralBench::Util {

    template <typename E>
    LongMap<E>::LongMap(int initialCapacity, float loadFactor) {

        if (!MathUtils::isPowerOfTwo(initialCapacity)) {
            throw invalid_argument("Size must be power of two: " + to_string(initialCapacity));
        }

        data = new Entry*[initialCapacity]();
        lengthMinusOne = initialCapacity - 1;
        this->loadFactor = loadFactor;
        threshold = static_cast<int>(round(initialCapacity * loadFactor));
        reusableIter = new ReusableIterator(this);
    }

    template <typename E>
    LongMap<E>::~LongMap() {

        clear();

        // don't forget to release all entry objects from the pool...
        Entry* entryToRelease = nullptr;
        while((entryToRelease = poolHead) != nullptr) {
            Entry* nxtPtr = entryToRelease->nxt;
            delete entryToRelease;
            poolHead = nxtPtr;
        }

        delete[] data;
        delete reusableIter;
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
    bool LongMap<E>::contains(E& value) const {
        for (int i = lengthMinusOne; i >= 0; i--) {
            Entry* e = data[i];
            while (e != nullptr) {
                if (*(e->value) == value) {
                    return true;
                }
                e = e->nxt;
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
            e = e->nxt;
        }
        return false;
    }

    template <typename E>
    E* LongMap<E>::get(long key) const {
        Entry* e = data[toArrayIndex(key)];
        while (e != nullptr) {
            if (e->key == key) {
                return e->value;
            }
            e = e->nxt;
        }
        return nullptr;
    }

    template <typename E>
    E* LongMap<E>::put(long key, E& value) {

        int index = toArrayIndex(key);

        Entry* e = data[index];

        while (e != nullptr) {
            if (e->key == key) {
                E* old = e->value;
                e->value = &value;
                return old;
            }
            e = e->nxt;
        }

        if (count >= threshold) {
            rehash();
            index = toArrayIndex(key); // lengthMinusOne has changed!
        }

        data[index] = getEntryFromPool(key, value, data[index]);

        count++;

        return nullptr;
    }

    template <typename E>
    E* LongMap<E>::remove(long key) {

        int index = toArrayIndex(key);

        Entry* e = data[index];
        Entry* prv = nullptr;

        while (e != nullptr) {
            if (e->key == key) {
                if (prv != nullptr) {
                    prv->nxt = e->nxt;
                } else {
                    data[index] = e->nxt;
                }

                E* oldValue = e->value;
                releaseEntryBackToPool(e);
                count--;

                return oldValue;
            }

            prv = e;
            e = e->nxt;
        }

        return nullptr;
    }

    template <typename E>
    void LongMap<E>::clear() {
        for (int index = lengthMinusOne; index >= 0; index--) {
            while (data[index] != nullptr) {
                Entry* nxt = data[index]->nxt;
                releaseEntryBackToPool(data[index]);
                data[index] = nxt;
            }
        }

        count = 0;
    }

    template <typename E>
    void LongMap<E>::rehash() {

        int oldCapacity = lengthMinusOne + 1;

        Entry** oldData = data;

        int newCapacity = oldCapacity * 2; // power of two, always!

        data = new Entry*[newCapacity]();
        lengthMinusOne = newCapacity - 1;

        threshold = static_cast<int>(round(newCapacity * loadFactor));

        for (int i = oldCapacity - 1; i >= 0; i--) {

            Entry* old = oldData[i];

            while (old != nullptr) {
                Entry* e = old;
                old = old->nxt;

                int index = toArrayIndex(e->key);
                e->nxt = data[index];
                data[index] = e;
            }
        }

        delete[] oldData; // don't forget
    }

    template <typename E>
    typename LongMap<E>::Entry* LongMap<E>::getEntryFromPool(long key, E& value, Entry* nxt) {

        Entry* newEntry = poolHead;

        if (newEntry != nullptr) {
            poolHead = newEntry->nxt;
        } else {
            newEntry = new Entry();
        }

        newEntry->key = key;
        newEntry->value = &value;
        newEntry->nxt = nxt;

        return newEntry;
    }

    template <typename E>
    void LongMap<E>::releaseEntryBackToPool(Entry* e) {
        e->value = nullptr;
        e->nxt = poolHead;
        poolHead = e;
    }

    template <typename E>
    int LongMap<E>::toArrayIndex(long key) const {
        return (static_cast<unsigned int>(key) & 0x7FFFFFFF) & lengthMinusOne;
    }

    template <typename E>
    typename LongMap<E>::ReusableIterator& LongMap<E>::iterator() {
        reusableIter->reset();
        return *reusableIter;
    }

    template <typename E>
    void LongMap<E>::ReusableIterator::reset() {
        size = outer->count;
        index = 0;
        dataIndex = 0;
        prv = nullptr;
        nxt = outer->data[0];
        entry = nullptr;
        wasRemoved = false;
    }

    template <typename E>
    bool LongMap<E>::ReusableIterator::hasNext() const {
        return index < size;
    }

    template <typename E>
    E& LongMap<E>::ReusableIterator::next() {

        if (index >= size) throw runtime_error("nothing to return");

        if (!wasRemoved) prv = entry;
        
        wasRemoved = false;

        entry = nxt;

        if (entry == nullptr) {
            while(entry == nullptr) {
                dataIndex++;
                entry = outer->data[dataIndex];
            }
            prv = nullptr;
        }

        index++;
        
        E* o = entry->value;

        outer->currIteratorKey = entry->key;

        nxt = entry->nxt;

        return *o;
    }

    template <typename E>
    void LongMap<E>::ReusableIterator::remove() {

        if (wasRemoved || entry == nullptr) throw runtime_error("nothing to remove");
        
        wasRemoved = true;

        if (prv == nullptr) {
            outer->data[dataIndex] = nxt;
        } else {
            prv->nxt = nxt;
        }

        outer->releaseEntryBackToPool(entry);

        entry = nullptr;

        outer->count--;
    }

}
