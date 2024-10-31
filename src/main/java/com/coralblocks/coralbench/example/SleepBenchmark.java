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

public class SleepBenchmark {
	
	private final static void sleepFor(long nanos) {
		long time = System.nanoTime();
		while((System.nanoTime() - time) < nanos);
	}
	
	private final static void doSleep(Bench bench) {
		bench.mark();
		sleepFor(1000);
		bench.measure();
	}
	
	public static void main(String[] args) {
		
		final int warmupIterations = 1000000;
		final int measurementIterations = 1000000;
		final int total = measurementIterations + warmupIterations;
		
		Bench bench = new Bench(warmupIterations);
		
		while(bench.getCount() < total) {
			doSleep(bench);
		}
		
		bench.printResults();
	}
}