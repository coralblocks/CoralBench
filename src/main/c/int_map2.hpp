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
#ifndef INT_MAP2_HPP
#define INT_MAP2_HPP

#include <optional>
#include <cstddef>
#include <vector>

using namespace std;

template <typename E>
class IntMap2 {

private:

    template <typename T>
    struct Entry {
        size_t key;
        optional<T> value;
    };

    size_t count = 0;
    vector<vector<Entry<E>>> data;

    size_t toArrayIndex(size_t key) const {
        return key % data.size();
    }

public:

    IntMap2(size_t capacity)
        : data(capacity) {
    }

    size_t size() const {
        return count;
    }

    optional<E> get(size_t key) const {
        vector<Entry<E>> const& entries = data[toArrayIndex(key)];
        for (Entry<E> const& e : entries) {
            if (e.key == key) return e.value;
        }
        return std::nullopt;
    }

    optional<E> put(size_t key, const E& value) {
        vector<Entry<E>>& entries = data[toArrayIndex(key)];
        for (auto& e : entries) {
            if (e.key == key) {
                optional<E> old = std::move(e.value); // no copy
                e.value = value;
                return old;
            }
        }
        entries.push_back( {key, value } );
        count++;
        return nullopt;
    }

    optional<E> remove(size_t key) {
        vector<Entry<E>>& entries = data[toArrayIndex(key)];
        for (Entry<E>& e : entries) {
            if (e.key == key) {
                auto old = e.value; // copy
                swap( e, entries.back() );
                entries.resize( entries.size() - 1 );
                count--;
                return old;
            }
        }
        return nullopt;
    }

    void clear() {
        for (auto& entries : data) entries.clear();
        count = 0;
    }
};

#endif // INT_MAP2_HPP
