/* 
 * Copyright 2024 (c) CoralBlocks - http://www.coralblocks.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 */
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
        T* value; // removed unnecessary std::optional (thanks to Paul McKenzie at https://stackoverflow.com/users/3133316/paulmckenzie)
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
        newEntry->value = const_cast<E*>(&value);
        newEntry->next = next;

        return newEntry;
    }

    void free(Entry<E>* e) {
        e->value = nullptr;
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

    E* get(int key) const {
        Entry<E>* e = data[toArrayIndex(key)];
        while (e != nullptr) {
            if (e->key == key) {
                return e->value;
            }
            e = e->next;
        }
        return nullptr;
    }

    E* put(int key, const E& value) {
        int index = toArrayIndex(key);
        Entry<E>* e = data[index];

        while (e != nullptr) {
            if (e->key == key) {
                E* old = e->value;
                e->value = const_cast<E*>(&value);
                return old;
            }
            e = e->next;
        }

        data[index] = newEntry(key, value, data[index]);
        count++;
        return nullptr;
    }

    E* remove(int key) {
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

                E* oldValue = e->value;
                free(e);
                count--;
                return oldValue;
            }
            prev = e;
            e = e->next;
        }
        return nullptr;
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