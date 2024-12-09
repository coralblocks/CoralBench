#ifndef INT_MAP_HPP
#define INT_MAP_HPP

#include <optional>
#include <cstddef>

template <typename E>
class IntMap {

private:

    template <typename T>
    struct Entry {
        int key;
        std::optional<T> value;
        Entry<T>* next;
    };

    Entry<E>** data;
    int count;
    Entry<E>* head;
    const int capacity;

    Entry<E>* newEntry(int key, const E& value, Entry<E>* next) {
        Entry<E>* newEntry = head;

        if (newEntry != nullptr) {
            head = newEntry->next;
        } else {
            newEntry = new Entry<E>();
        }

        newEntry->key = key;
        newEntry->value = value;
        newEntry->next = next;

        return newEntry;
    }

    void free(Entry<E>* e) {
        e->value.reset();
        e->next = head;
        head = e;
    }

    int toArrayIndex(int key) const {
        return (key & 0x7FFFFFFF) % capacity;
    }

public:

    IntMap(int capacity) 
        : capacity(capacity), count(0), head(nullptr) {
        data = new Entry<E>*[capacity];
        for (int i = 0; i < capacity; i++) {
            data[i] = nullptr;
        }
    }

    ~IntMap() {
        clear();
        
        while (head != nullptr) {
            Entry<E>* temp = head;
            head = head->next;
            delete temp;
        }

        delete[] data;
    }

    int size() const {
        return count;
    }

    std::optional<E> get(int key) const {
        Entry<E>* e = data[toArrayIndex(key)];
        while (e != nullptr) {
            if (e->key == key) {
                return e->value;
            }
            e = e->next;
        }
        return std::nullopt;
    }

    std::optional<E> put(int key, const E& value) {
        int index = toArrayIndex(key);
        Entry<E>* e = data[index];

        while (e != nullptr) {
            if (e->key == key) {
                std::optional<E> old = e->value;
                e->value = value;
                return old;
            }
            e = e->next;
        }

        data[index] = newEntry(key, value, data[index]);
        count++;
        return std::nullopt;
    }

    std::optional<E> remove(int key) {
        int index = toArrayIndex(key);
        Entry<E>* e = data[index];
        Entry<E>* prev = nullptr;

        while (e != nullptr) {
            if (e->key == key) {
                if (prev != nullptr) {
                    prev->next = e->next;
                } else {
                    data[index] = e->next;
                }

                std::optional<E> oldValue = e->value;
                free(e);
                count--;
                return oldValue;
            }
            prev = e;
            e = e->next;
        }
        return std::nullopt;
    }

    void clear() {
        for (int index = capacity - 1; index >= 0; index--) {
            while (data[index] != nullptr) {
                Entry<E>* next = data[index]->next;
                free(data[index]);
                data[index] = next;
            }
        }
        count = 0;
    }
};

#endif // INT_MAP_HPP
