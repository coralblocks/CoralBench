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
package com.coralblocks.coralbench.util;

public class LongMap2<E> {
	
	private static final int DEFAULT_CAPACITY = 128;
	
	private static class Entry<T> {
		long key;
		T value;
		Entry<T> next;
	}
	
	private Entry<E>[] data;

	private int lengthMinusOne;

	private int count;

	private Entry<E> poolHead;

	public LongMap2() {
		this(DEFAULT_CAPACITY);
	}

	@SuppressWarnings("unchecked")
	public LongMap2(int capacity) {
		this.data = new Entry[capacity];
		this.lengthMinusOne = capacity - 1;
	}
	
	private Entry<E> getEntryFromPool(long key, E value, Entry<E> next) {

		Entry<E> newEntry = poolHead;

		if (newEntry != null) {
			poolHead = newEntry.next;
		} else {
			newEntry = new Entry<E>();
		}

		newEntry.key = key;
		newEntry.value = value;
		newEntry.next = next;

		return newEntry;
	}

	private void releaseEntryBackToPool(Entry<E> e) {
		e.value = null;
		e.next = poolHead;
		poolHead = e;
	}
	
	public int size() {
		return count;
	}

	public boolean isEmpty() {
		return size() == 0;
	}

	private final int toArrayIndex(long key) {
		return (((int) key) & 0x7FFFFFFF) & lengthMinusOne;
	}

	public boolean containsKey(long key) {

		Entry<E> e = data[toArrayIndex(key)];

		while(e != null) {

			if (e.key == key) {

				return true;
			}

			e = e.next;
		}
		
		return false;
	}

	public E get(long key) {

		Entry<E> e = data[toArrayIndex(key)];

		while(e != null) {

			if (e.key == key) {

				return e.value;
			}

			e = e.next;
		}

		return null;
	}

	public E put(long key, E value) {

		if (value == null) {
			throw new IllegalArgumentException("Cannot put null value!");
		}

		int index = toArrayIndex(key);

		Entry<E> e = data[index];

		while(e != null) {

			if (e.key == key) {

				E old = e.value;

				e.value = value;

				return old;
			}

			e = e.next;
		}

		data[index] = getEntryFromPool(key, value, data[index]);

		count++;

		return null;
	}

	public E remove(long key) {

		int index = toArrayIndex(key);

		Entry<E> e = data[index];
		Entry<E> prev = null;

		while(e != null) {

			if (e.key == key) {

				if (prev != null) {

					prev.next = e.next;

				} else {

					data[index] = e.next;
				}

				E oldValue = e.value;

				releaseEntryBackToPool(e);

				count--;

				return oldValue;
			}

			prev = e;
			e = e.next;
		}

		return null;
	}

	public void clear() {

		for(int index = data.length - 1; index >= 0; index--) {

			while(data[index] != null) {

				Entry<E> next = data[index].next;

				releaseEntryBackToPool(data[index]);

				data[index] = next;
			}
		}

		count = 0;
	}
}