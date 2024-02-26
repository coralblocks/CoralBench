package com.coralblocks.coralbench;

import java.text.DecimalFormat;

public class Bench {

	public static boolean INCLUDE_STDEV = true;
	public static boolean INCLUDE_MORE_PERCS = false;
	public static boolean INCLUDE_WORST_PERCS = false;
	public static boolean INCLUDE_TOTALS = false;
	public static boolean INCLUDE_RATIOS = false;
	
	private static final int DEFAULT_WARMUP = 0;
	private static final int NUMBER_OF_DECIMALS = 3;

	private final DecimalFormat formatter = new DecimalFormat("#,###");
	
	private long time;
	private long count;
	private long totalTime;
	private int warmup;
	private long minTime;
	private long maxTime;
	
	public Bench() {
		this(DEFAULT_WARMUP);
	}

	public Bench(final int warmup) {
		
		this.warmup = warmup;
		
		try {
			// initialize it here so when you measure for the first time garbage is not created...
			Class.forName("java.lang.Math");
		} catch (final Exception e) {
			throw new RuntimeException(e);
		}
		
		reset(true);
	}

	public void reset(boolean repeatWarmup) {
		time = 0;
		count = 0;
		totalTime = 0;
		if (!repeatWarmup) warmup = 0;
		minTime = Long.MAX_VALUE;
		maxTime = Long.MIN_VALUE;
		
		System.gc(); // Good opportunity
	}
	
	public final long mark() {
		time = System.nanoTime();
		return time;
	}
	
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
	
	public final boolean isWarmingUp() {
		return warmup <= count;
	}

	private final boolean measure(long lastNanoTime) {
		if (++count > warmup) {
			totalTime += lastNanoTime;
			minTime = Math.min(minTime, lastNanoTime);
			maxTime = Math.max(maxTime, lastNanoTime);
			return true;
		}
		return false;
	}
	
    public long getCount() {
    	return count;
    }
	
	public final double getAverage() {
		return avg();
	}

	private final double avg() {
		final long realCount = count - warmup;
		if (realCount <= 0) {
			return 0;
		}
		final double avg = ((double) totalTime / (double) realCount);
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
	
	public static String convertNanoTime(double nanoTime) {
		StringBuilder sb = new StringBuilder();
		convertNanoTime(nanoTime, sb);
		return sb.toString();
	}
	
	public static StringBuilder convertNanoTime(double nanoTime, StringBuilder sb) {
		if (nanoTime >= 1000000000L) {
			// seconds...
			double seconds = round(nanoTime / 1000000000D);
			sb.append(seconds).append(seconds > 1 ? " secs" : " sec");
		} else if (nanoTime >= 1000000L) {
			// millis...
			double millis = round(nanoTime / 1000000D);
			sb.append(millis).append(millis > 1 ? " millis" : " milli");
		} else if (nanoTime >= 1000L) {
			// micros...
			double micros = round(nanoTime / 1000D);
			sb.append(micros).append(micros > 1 ? " micros" : " micro");
		} else {
			double nanos = round(nanoTime);
			sb.append(nanos).append(nanos > 1 ? " nanos" : " nano");
		}
		return sb;
	}

	public String results() {
		final StringBuilder sb = new StringBuilder(128);
		final long realCount = count - warmup;
		sb.append("Iterations: ").append(formatter.format(realCount));
		sb.append(" | Warm-Up: ").append(formatter.format(warmup));
		addAverage(sb);
		if (realCount > 0) {
			sb.append(" | Min Time: ").append(convertNanoTime(minTime));
			sb.append(" | Max Time: ").append(convertNanoTime(maxTime));
		}
		
		return sb.toString();
	}
	
	protected void addAverage(StringBuilder sb) {
		sb.append(" | Avg Time: ").append(convertNanoTime(avg()));
	}
	
	public void printResults() {
		System.out.println(results());
		System.out.println();
	}
	
	public static void main(String[] args) {
		
		int warmupIterations = 1000;
		int measurementIterations = 1000;
		
		Bench bench = new Bench(warmupIterations);
		
		while(bench.getCount() < measurementIterations + warmupIterations) {
			doSleep(bench);
		}
		
		bench.printResults();
		
		bench.reset(false); // false because we don't want to repeat warmup
		
		while(bench.getCount() < measurementIterations) {
			doSleep(bench);
		}
		
		bench.printResults();
	}
	
	private final static void doSleep(Bench bench) {
		bench.mark();
		try {
			Thread.sleep(1);
		} catch(InterruptedException e) {
			throw new RuntimeException(e);
		}
		bench.measure();
	}
}

