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
package com.coralblocks.coralbench.example;

import java.util.ArrayList;

import com.coralblocks.coralbench.util.LinkedObjectList;

class IntMap2<E> {

	static class Entry<T> {
		int key;
		T value;
	}

	private ArrayList<Entry<E>>[] data;
	private int count;
	private final LinkedObjectList<Entry<E>> entryPool;

	@SuppressWarnings("unchecked")
	IntMap2(int capacity, int initialBucketSize, int initialEntryPoolSize) {
		this.data = (ArrayList<Entry<E>>[]) new ArrayList[capacity];
		for(int i = 0; i < capacity; i++) {
			data[i] = new ArrayList<Entry<E>>(initialBucketSize);
		}
		this.entryPool = new LinkedObjectList<Entry<E>>(initialEntryPoolSize);
	}

	public final int size() {
		return count;
	}

	private final int toArrayIndex(int key) {
		return key % data.length;
	}

	public final E get(int key) {

		ArrayList<Entry<E>> entries = data[toArrayIndex(key)];
		
		final int size = entries.size();
		
		for(int i = 0; i < size; i++) {
			Entry<E> e = entries.get(i);
			if (e.key == key) return e.value;
		}
		
		return null;
	}
	
	public final E put(int key, E value) {

		ArrayList<Entry<E>> entries = data[toArrayIndex(key)];
		
		final int size = entries.size();
		
		for(int i = 0; i < size; i++) {
			Entry<E> e = entries.get(i);
			if (e.key == key) {
				E old = e.value;
				e.value = value;
				return old;
			}
		}
		
		Entry<E> e = entryPool.removeLast();
		
		if (e == null) e = new Entry<E>();
		
		e.key = key;
		e.value = value;
		
		entries.add(e);
		
		count++;
		
		return null;
	}
	
	public final E remove(int key) {
		
		ArrayList<Entry<E>> entries = data[toArrayIndex(key)];
		
		final int size = entries.size();
		
		for(int i = 0; i < size; i++) {
			Entry<E> e = entries.get(i);
			if (e.key == key) {
				E toReturn = e.value;
				e.value = null;
				entryPool.addLast(e);
				Entry<E> last = entries.remove(size - 1);
				if (last == e) {
					// nothing to do
				} else {
					entries.set(i, last); // swap
				}
				count--;
				return toReturn;
			}
		}

		return null;
	}

	public final void clear() {
		
		for(int i = 0; i < data.length; i++) {
			ArrayList<Entry<E>> entries = data[i];
			final int size = entries.size();
			for(int j = 0; j < size; j++) {
				Entry<E> e = entries.get(j);
				e.value = null;
				entryPool.addLast(e);
			}
			entries.clear();
		}

		count = 0;
	}
}