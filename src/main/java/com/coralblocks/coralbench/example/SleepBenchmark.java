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
		bench.mark(); // <===== timer starts
		sleepFor(1000);
		bench.measure(); // <===== timer stops
	}
	
	public final static void main(String[] args) {
		
		final int warmupIterations = args.length > 0 ? Integer.parseInt(args[0]) : 1_000_000;
		final int measurementIterations = args.length > 1 ? Integer.parseInt(args[1]) : 2_000_000;
		final int totalIterations = measurementIterations + warmupIterations;
		
		Bench bench = new Bench(warmupIterations); // specify the number of warmup iterations to ignore
		
		while(bench.getIterations() < totalIterations) { // note that we always perform warmup + measurement iterations
			doSleep(bench);
		}
		
		bench.printResults();
	}
}