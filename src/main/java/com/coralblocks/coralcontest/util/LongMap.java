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
package com.coralblocks.coralcontest.util;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class LongMap<E> implements Iterable<E> {
	
	private static final int DEFAULT_INITIAL_CAPACITY = 128;
	private static final float DEFAULT_LOAD_FACTOR = 0.80f;
	
	private static class Entry<T> {
		long key;
		T value;
		Entry<T> next;
	}
	
	private Entry<E>[] data;

	private int lengthMinusOne;

	private int count;

	private int threshold;

	private float loadFactor;

	private Entry<E> poolHead;

	private final ReusableIterator reusableIter = new ReusableIterator();

	private long currIteratorKey;

	public LongMap() {
		this(DEFAULT_INITIAL_CAPACITY, DEFAULT_LOAD_FACTOR);
	}

	public LongMap(int initialCapacity) {
		this(initialCapacity, DEFAULT_LOAD_FACTOR);
	}

	@SuppressWarnings("unchecked")
	public LongMap(int initialCapacity, float loadFactor) {

		if (!MathUtils.isPowerOfTwo(initialCapacity)) {
			throw new IllegalArgumentException("Size must be power of two: " + initialCapacity);
		}

		this.data = new Entry[initialCapacity];
		this.lengthMinusOne = initialCapacity - 1;
		this.loadFactor = loadFactor;
		this.threshold =  Math.round(initialCapacity * loadFactor);
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
	
	public final long getCurrIteratorKey() {
		return currIteratorKey;
	}

	public int size() {
		return count;
	}

	public boolean isEmpty() {
		return size() == 0;
	}

	public boolean contains(E value) {

		for(int i = data.length - 1; i >= 0; i--) {

			Entry<E> e = data[i];

			while(e != null) {

				if (e.value.equals(value)) {

					return true;
				}

				e = e.next;
			}
		}
		
		return false;
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

	@SuppressWarnings("unchecked")
	private void rehash() {

		int oldCapacity = data.length;

		Entry<E> oldData[] = data;

		int newCapacity = oldCapacity * 2; // power of two, always!

		data = new Entry[newCapacity];
		lengthMinusOne = newCapacity - 1;

		threshold = Math.round(newCapacity * loadFactor);

		for(int i = oldCapacity - 1; i >= 0; i--) {

			Entry<E> old = oldData[i];

			while(old != null) {

				Entry<E> e = old;

				old = old.next;

				int index = toArrayIndex(e.key);

				e.next = data[index];

				data[index] = e;
			}
		}
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

		if (count >= threshold) {

			rehash();

			index = toArrayIndex(key); // lengthMinusOne has changed!

			data[index] = getEntryFromPool(key, value, data[index]);

		} else {

			data[index] = getEntryFromPool(key, value, data[index]);
		}

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

	private class ReusableIterator implements Iterator<E> {

		int size = count;
		int index = 0;
		int dataIndex = 0;
		Entry<E> prev = null;
		Entry<E> next = null;
		Entry<E> entry = null;
		boolean wasRemoved = false;

		public void reset() {
			this.size = count;
			this.index = 0;
			this.dataIndex = 0;
			this.prev = null;
			this.next = data[0];
			this.entry = null;
			this.wasRemoved = false;
		}

		@Override
		public final boolean hasNext() {
			return index < size;
		}

		@Override
		public final E next() {

			if (index >= size) throw new NoSuchElementException();

			if (!wasRemoved) prev = entry;
			
			wasRemoved = false;

			entry = next;

			if (entry == null) {
    			while(entry == null) {
    				dataIndex++;
    				entry = data[dataIndex];
    			}
    			prev = null;
			}

			index++;
			
			E o = entry.value;

			currIteratorKey = entry.key;

			next = entry.next;

			return o;
		}

		@Override
		public final void remove() {

			if (wasRemoved || entry == null) {
				throw new NoSuchElementException();
			}
			
			wasRemoved = true;

			if (prev == null) {
				data[dataIndex] = next;
			} else {
				prev.next = next;
			}

			releaseEntryBackToPool(entry);

			entry = null;

			count--;
		}

	}

	@Override
	public Iterator<E> iterator() {
		reusableIter.reset();
		return reusableIter;
	}
	
}