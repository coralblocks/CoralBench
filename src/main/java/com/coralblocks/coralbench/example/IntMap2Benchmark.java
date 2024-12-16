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

public class IntMap2Benchmark {
	
	public static void main(String[] args) {
		
		final int warmup = args.length > 0 ? Integer.parseInt(args[0]) : 1_000_000;
		final int measurements = args.length > 1 ? Integer.parseInt(args[1]) : 1_000_000;
		final int totalIterations = measurements + warmup;
		final int mapCapacity = args.length > 2 ? Integer.parseInt(args[2]) : 100_000;
		final int initialEntryPoolSize = args.length > 3 ? Integer.parseInt(args[3]) : totalIterations;
		final int initialBucketSize = totalIterations / mapCapacity;
		
		System.out.println("\nArguments: warmup=" + warmup + " measurements=" + measurements + 
						   " mapCapacity=" + mapCapacity + " initialBucketSize=" + initialBucketSize + 
						   " initialEntryPoolSize=" + initialEntryPoolSize + "\n");
		
		final IntMap2<Object> map = new IntMap2<Object>(mapCapacity, initialBucketSize, initialEntryPoolSize);
		
		final Object dummy = new Object();
		
		Bench bench = new Bench(warmup);

		System.out.println("Benchmarking put on empty map... (1) => creating new Entry objects");
		for(int i = 0; i < totalIterations; i++) {
			bench.mark();
			map.put(i, dummy);
			bench.measure();
		}
		bench.printResults();
		
		System.out.println("Benchmarking put after clear()... (2) => hitting the pool of Entry objects");
		map.clear(); // clear the map, but the entry pool will be there
		bench.reset(true);
		for(int i = 0; i < totalIterations; i++) {
			bench.mark();
			map.put(i, dummy);
			bench.measure();
		}
		bench.printResults();
		
		System.out.println("Benchmarking get...");
		bench.reset(true);
		for(int i = 0; i < totalIterations; i++) {
			bench.mark();
			map.get(i);
			bench.measure();
		}
		bench.printResults();
		
		System.out.println("Benchmarking remove...");
		bench.reset(true);
		for(int i = 0; i < totalIterations; i++) {
			bench.mark();
			map.remove(i);
			bench.measure();
		}
		bench.printResults();
	}
}