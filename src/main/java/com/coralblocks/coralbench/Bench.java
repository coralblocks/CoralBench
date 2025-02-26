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

import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Iterator;
import java.util.Map;
import java.util.TreeMap;

import com.coralblocks.coralbench.util.MutableInt;
import com.coralblocks.coralds.list.LinkedList;
import com.coralblocks.coralds.map.LongMap;

/**
 * <p>A class to benchmark Java code for latency. It prints some pretty results with percentiles. See an example below:</p>
 * <pre>
 * Measurements: 2,000,000 | Warm-Up: 1,000,000 | Iterations: 3,000,000
 * Avg Time: 1.025 micros | Min Time: 1.000 micro | Max Time: 56.500 micros
 * 75% = [avg: 1.016 micros, max: 1.042 micros]
 * 90% = [avg: 1.020 micros, max: 1.042 micros]
 * 99% = [avg: 1.022 micros, max: 1.042 micros]
 * 99.9% = [avg: 1.022 micros, max: 1.125 micros]
 * 99.99% = [avg: 1.023 micros, max: 7.791 micros]
 * 99.999% = [avg: 1.024 micros, max: 15.959 micros]
 * </pre> 
 */
public class Bench {

	private static final int DEFAULT_WARMUP = 0;
	private static final int NUMBER_OF_DECIMALS = 3;
	private static final String VERBOSE = "\n==CoralBenchVerbose==> ";
	private static final int DEFAULT_LOG_EVERY = 10000;

	private final DecimalFormat formatter = new DecimalFormat("#,###");
	
	private long time;
	private int iterations;
	private int measurements;
	private int toWarmup;
	private long totalTime;
	private long minTime;
	private long maxTime;
	
	private final LinkedList<MutableInt> pool = new LinkedList<MutableInt>(1024);
	private final LongMap<MutableInt> results = new LongMap<MutableInt>(4194304); // 2 ^ 22
	private final boolean verbose;
	private final int verboseLogEvery;
	
	/**
	 * Creates a <code>Bench</code> object without any warmup (warmup count is assumed to be zero)
	 */
	public Bench() {
		this(DEFAULT_WARMUP);
	}

	/**
	 * Creates a <code>Bench</code> object with the given warmup count
	 * 
	 * @param warmup how many iterations to warmup and ignore results
	 */
	public Bench(final int warmup) {
		
		this.toWarmup = warmup;
		
		try {
			// initialize it here so when you measure for the first time garbage is not created...
			Class.forName("java.lang.Math");
		} catch (final Exception e) {
			throw new RuntimeException(e);
		}
		
		String s = System.getProperty("coralBenchVerbose");
		this.verbose = s != null && s.equalsIgnoreCase("true");
		
		s = System.getProperty("coralBenchVerboseLogEvery");
		if (s == null) {
			this.verboseLogEvery = DEFAULT_LOG_EVERY;
		} else {
			this.verboseLogEvery = Integer.parseInt(s);
		}
		
		reset(true);
	}
	
	private final MutableInt getMutableInt(int x) {
		MutableInt mi = pool.removeLast();
		if (mi != null) {
			mi.set(x);
			return mi;
		}
		return new MutableInt(x);
	}
	
	private final void releaseMutableInt(MutableInt mi) {
		pool.addLast(mi);
	}
	
	/**
	 * Reset this benchmark so that it can be re-used for a new and fresh benchmark. If this benchmark has a warmup count, it will NOT be repeated.
	 */
	public void reset() {
		reset(false);
	}

	/**
	 * Reset this benchmark so that it can be re-used for a new and fresh benchmark.
	 * 
	 * @param repeatWarmup should warmup be repeated or not?
	 */
	public void reset(boolean repeatWarmup) {
		time = 0;
		iterations = 0;
		totalTime = 0;
		if (!repeatWarmup) toWarmup = 0;
		minTime = Long.MAX_VALUE;
		maxTime = Long.MIN_VALUE;
		
		measurements = 0;
		Iterator<MutableInt> iter = results.iterator();
		while(iter.hasNext()) {
			releaseMutableInt(iter.next());
		}
		results.clear();
	}
	
	/**
	 * Starts the timer to measure
	 * 
	 * @return the start timestamp in nanoseconds
	 */
	public final long mark() {
		time = System.nanoTime();
		return time;
	}
	
	/**
	 * Measure (and possibly store if not warming up) the elapsed time
	 * 
	 * @return the elapsed time in nanoseconds
	 */
	public long measure() {
		if (time > 0) {
			long now = System.nanoTime();
			long elapsed = now - time;
			if (elapsed < 0) throw new RuntimeException("Found a negative elapsed time: now=" + now + " start=" + time + " diff=" + elapsed);
			final boolean counted = measure(elapsed);
			if (counted) {
				return elapsed;
			}
		}
		return -1;
	}
	
	/**
	 * Are we currently warming up or are we currently measuring?
	 * 
	 * @return true if warming up
	 */
	public final boolean isWarmingUp() {
		return toWarmup <= iterations;
	}

	/**
	 * Register and take into account (and possibly store if not warming up) the provided elapsed time
	 * 
	 * @param lastNanoTime the elapsed time you want to take into account
	 * @return true if it was taken into account because we are not warming up
	 */
	public final boolean measure(final long lastNanoTime) {
		
		final boolean isToMeasure = ++iterations > toWarmup;
		
		if (isToMeasure) {

			totalTime += lastNanoTime;
			minTime = Math.min(minTime, lastNanoTime);
			maxTime = Math.max(maxTime, lastNanoTime);
			
			MutableInt count = results.get(lastNanoTime);
			if (count == null) {
				results.put(lastNanoTime, getMutableInt(1));
			} else {
				count.set(count.get() + 1);
			}
			measurements++;
		}
		
		if (verbose) {
			if (iterations == 1 || iterations % verboseLogEvery == 0) System.out.println(VERBOSE + "Iteration " + iterations + " => " + lastNanoTime); 
		}
		
		return isToMeasure;
	}
	
	private String formatPercentage(double x, int decimals) {
		NumberFormat percentFormat = NumberFormat.getPercentInstance();
		percentFormat.setMaximumFractionDigits(decimals);
		return percentFormat.format(x);
	}
	
	private void addPercentile(StringBuilder sb, double perc, TreeMap<Long, MutableInt> treeMap) {
		
		if (treeMap.isEmpty()) {
			return;
		}

		long maxTop = -1;
		
		long x = Math.round(perc * measurements);
		Iterator<Map.Entry<Long, MutableInt>> iter = treeMap.entrySet().iterator();
		int iTop = 0;
		long sumTop = 0;
		
		LOOP: while(iter.hasNext()) {
			
			Map.Entry<Long, MutableInt> entry = iter.next();
			Long time = entry.getKey();
			MutableInt count = entry.getValue();
			
			for(int a = 0; a < count.get(); a++) {
				
				iTop++;
				sumTop += time;
				
				if (iTop == x) {
					maxTop = time;
					break LOOP;
				}
			}
		}
		
		sb.append(formatPercentage(perc, 8));
		sb.append(" = [");
		sb.append("avg: ").append(convertNanoTime(sumTop / iTop));
		sb.append(", max: ").append(convertNanoTime(maxTop)).append(']');
		sb.append('\n');
	}
	
	/**
	 * How many iterations have we done so far, including warming up?
	 * 
	 * @return the total number of iterations so far
	 */
    public int getIterations() {
    	return iterations;
    }
    
    /**
     * How many measurements have we done so far, excluding warming up?
     * 
     * @return the total number of measurements so far
     */
    public int getMeasurements() {
    	return measurements;
    }
	
    /**
     * Return the average latency of all measurements
     * 
     * @return the average latency
     */
	public final double getAverage() {
		return avg();
	}

	private final double avg() {
		if (measurements <= 0) {
			return 0;
		}
		final double avg = ((double) totalTime / (double) measurements);
		final double rounded = Math.round(avg * 100D) / 100D;
		return rounded;
	}
	
	private static double round(double d) {
		return round(d, NUMBER_OF_DECIMALS);
	}
	
	private static double round(double d, int decimals) {
		double pow = Math.pow(10, decimals);
		return ((double) Math.round(d * pow)) / pow;
	}
	
	private static String convertNanoTime(double nanoTime) {
		StringBuilder sb = new StringBuilder();
		convertNanoTime(nanoTime, sb);
		return sb.toString();
	}
	
	private static StringBuilder convertNanoTime(double nanoTime, StringBuilder sb) {
		if (nanoTime >= 1000000000L) {
			// seconds...
			double seconds = round(nanoTime / 1000000000D);
			sb.append(formatThreeDecimals(seconds)).append(seconds > 1 ? " secs" : " sec");
		} else if (nanoTime >= 1000000L) {
			// millis...
			double millis = round(nanoTime / 1000000D);
			sb.append(formatThreeDecimals(millis)).append(millis > 1 ? " millis" : " milli");
		} else if (nanoTime >= 1000L) {
			// micros...
			double micros = round(nanoTime / 1000D);
			sb.append(formatThreeDecimals(micros)).append(micros > 1 ? " micros" : " micro");
		} else {
			double nanos = round(nanoTime);
			sb.append(formatThreeDecimals(nanos)).append(nanos > 1 ? " nanos" : " nano");
		}
		return sb;
	}
	
	private static String formatThreeDecimals(double d) {
		return String.format("%.3f", d);
	}

	/**
	 * Get the results as a string
	 * 
	 * @param includePercentiles true to include percentiles
	 * @return the results as a string
	 */
	public String results(boolean includePercentiles) {
		final StringBuilder sb = new StringBuilder(128);
		sb.append("Measurements: ").append(formatter.format(measurements));
		sb.append(" | Warm-Up: ").append(formatter.format(toWarmup));
		sb.append(" | Iterations: ").append(formatter.format(iterations)).append('\n');
		if (measurements > 0) {
			sb.append("Avg Time: ").append(convertNanoTime(avg()));
			sb.append(" | Min Time: ").append(convertNanoTime(minTime));
			sb.append(" | Max Time: ").append(convertNanoTime(maxTime));

			sb.append('\n');
		
			if (includePercentiles) {
				TreeMap<Long, MutableInt> treeMap = new TreeMap<Long, MutableInt>();
				Iterator<MutableInt> iter = results.iterator();
				while(iter.hasNext()) {
					MutableInt counter = iter.next();
					Long time = results.getCurrIteratorKey();
					treeMap.put(time, counter);
				}
		
				addPercentile(sb, 0.75D, treeMap);
				addPercentile(sb, 0.9D, treeMap);
				addPercentile(sb, 0.99D, treeMap);
				addPercentile(sb, 0.999D, treeMap);
				addPercentile(sb, 0.9999D, treeMap);
				addPercentile(sb, 0.99999D, treeMap);
			}
		}
		
		return sb.toString();
	}
	
	/**
	 * This method simply calls <code>results(true)</code>.
	 * 
	 *  @return the results as a string
	 */
	public String results() {
		return results(true);
	}
	
	/**
	 * Print the results to stdout
	 * 
	 * @param includePercentiles true to include percentiles
	 */
	public void printResults(boolean includePercentiles) {
		System.out.println(results(includePercentiles));
	}
	
	/**
	 * This method simply calls <code>printResults(true)</code>.
	 */
	public void printResults() {
		printResults(true);
	}
	
}

