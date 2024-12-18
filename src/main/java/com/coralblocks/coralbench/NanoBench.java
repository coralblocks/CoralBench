/* 
 * Copyright 2015-2024 (c) CoralBlocks LLC - http://www.coralblocks.com
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
package com.coralblocks.coralbench;

/**
 * <p>A garbage-free implementation of a very small class to benchmark Java code for latency in nanoseconds.
 * It prints only the bare minimum of information (see below) and the code is supposed to be as fast 
 * as possible and to use no external dependencies or libraries.
 * Checks and formats are avoided on purpose for size and simplicity.</p>
 * 
 * <p>For comprehensive benchmarks of course you should be using the {@link Bench} class instead.</p>
 * 
 * <pre>
 * Measurements: 2000000 | Avg Time: 1025 nanos | Min Time: 122 nanos | Max Time: 56500 nanos
 * </pre> 
 */
public class NanoBench {

	private int measurements;
	private long totalTime, minTime, maxTime, time;
	private final StringBuilder sb = new StringBuilder(128);
	
	/**
	 * Creates a new <code>QuickBench</code>
	 */
	public NanoBench() {
		reset();
	}
	
	/**
	 * Resets this benchmark so that it can be used again
	 */
	public final void reset() {
		totalTime = time = measurements = 0;
		maxTime = Long.MIN_VALUE;
		minTime = Long.MAX_VALUE;
	}
	
	/**
	 * Starts the timer to measure
	 */
	public final void mark() {
		time = System.nanoTime();
	}
	
	/**
	 * Stops the timer and measure
	 */
	public final void measure() {
		long lastNanoTime = System.nanoTime() - time;
		totalTime += lastNanoTime;
		minTime = lastNanoTime < minTime ? lastNanoTime : minTime;
		maxTime = lastNanoTime > maxTime ? lastNanoTime : maxTime;
		measurements++;
	}
	
	/**
	 * Print the results
	 */
	public final void printResults() {
		sb.setLength(0);
		sb.append("Measurements: ").append(measurements);
		sb.append("| Avg Time: ").append((long) (totalTime / (double) measurements)).append(" nanos");
		sb.append(" | Min Time: ").append(minTime).append(" nanos");
		sb.append(" | Max Time: ").append(maxTime).append(" nanos\n\n");
		for(int i = 0; i < sb.length(); i++) System.out.print(sb.charAt(i));
	}
	
	public static void main(String[] args) {
		
		NanoBench bench = new NanoBench();
		
		for(int i = 0; i < 1_000_000; i++) {
			bench.mark();
			sleepFor(1500);
			bench.measure();
		}
		
		bench.printResults();
	}
	
	private static final void sleepFor(long nanos) {
		long time = System.nanoTime();
		while((System.nanoTime() - time) < nanos);
	}
}

