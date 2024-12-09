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

import com.coralblocks.coralbench.Bench;

public class IntMapBenchmark {
	
	public static void main(String[] args) {
		
		final int warmup = args.length > 0 ? Integer.parseInt(args[0]) : 1_000_000;
		final int measurements = args.length > 1 ? Integer.parseInt(args[1]) : 1_000_000;
		final int totalIterations = measurements + warmup;
		final int mapCapacity = args.length > 2 ? Integer.parseInt(args[2]) : 100_000;
		
		System.out.println("\nArguments: warmup=" + warmup + " measurements=" + measurements + " mapCapacity=" + mapCapacity + "\n");
		
		IntMap<Object> map = new IntMap<Object>(mapCapacity); // Object source code is down below...
		
		final Object dummy = new Object();
		
		Bench bench = new Bench(warmup);

		System.out.println("Benchmarking put...");
		bench.reset(true);
		for(int i = 0; i < totalIterations; i++) {
			bench.mark();
			map.put(i, dummy);
			bench.measure();
		}
		bench.printResults(false);
		
		System.out.println("Benchmarking get...");
		bench.reset(true);
		for(int i = 0; i < totalIterations; i++) {
			bench.mark();
			map.get(i);
			bench.measure();
		}
		bench.printResults(false);
		
		System.out.println("Benchmarking remove...");
		bench.reset(true);
		for(int i = 0; i < totalIterations; i++) {
			bench.mark();
			map.remove(i);
			bench.measure();
		}
		bench.printResults(false);
	}

	static class IntMap<E> {
		
		static class Entry<T> {
			int key;
			T value;
			Entry<T> next;
		}
	
		private Entry<E>[] data;
		private int count;
		private Entry<E> head;
		private final int capacity;
	
		@SuppressWarnings("unchecked")
		IntMap(int capacity) {
			this.capacity = capacity;
			this.data = new Entry[capacity];
		}
		
		private Entry<E> newEntry(int key, E value, Entry<E> next) {
	
			Entry<E> newEntry = head;
	
			if (newEntry != null) {
				head = newEntry.next;
			} else {
				newEntry = new Entry<E>();
			}
	
			newEntry.key = key;
			newEntry.value = value;
			newEntry.next = next;
	
			return newEntry;
		}
	
		private void free(Entry<E> e) {
			e.value = null;
			e.next = head;
			head = e;
		}
		
		public final int size() {
			return count;
		}
	
		private final int toArrayIndex(int key) {
			return (key & 0x7FFFFFFF) % capacity;
		}
	
		public final E get(int key) {
	
			Entry<E> e = data[toArrayIndex(key)];
	
			while(e != null) {
	
				if (e.key == key) {
	
					return e.value;
				}
	
				e = e.next;
			}
			return null;
		}
	
		public final E put(int key, E value) {
	
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
	
			data[index] = newEntry(key, value, data[index]);
	
			count++;
	
			return null;
		}
		
		public final E remove(int key) {
	
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
	
					free(e);
	
					count--;
	
					return oldValue;
				}
	
				prev = e;
				e = e.next;
			}
	
			return null;
		}
	
		public final void clear() {
	
			for(int index = data.length - 1; index >= 0; index--) {
	
				while(data[index] != null) {
	
					Entry<E> next = data[index].next;
	
					free(data[index]);
	
					data[index] = next;
				}
			}
	
			count = 0;
		}
	}
}
