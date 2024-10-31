package com.coralblocks.coralbench;

import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Iterator;
import java.util.Map;
import java.util.TreeMap;

import com.coralblocks.coralbench.util.LinkedObjectList;
import com.coralblocks.coralbench.util.LongMap;
import com.coralblocks.coralbench.util.MutableInt;

public class Bench {

	private static final int DEFAULT_WARMUP = 0;
	private static final int NUMBER_OF_DECIMALS = 3;

	private final DecimalFormat formatter = new DecimalFormat("#,###");
	
	private long time;
	private int count;
	private int size;
	private int warmup;
	private long totalTime;
	private long minTime;
	private long maxTime;
	
	private final LinkedObjectList<MutableInt> pool = new LinkedObjectList<MutableInt>(1024);
	private final LongMap<MutableInt> results = new LongMap<MutableInt>(4194304); // 2 ^ 22
	
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
	
	public void reset() {
		reset(false);
	}

	public void reset(boolean repeatWarmup) {
		time = 0;
		count = 0;
		totalTime = 0;
		if (!repeatWarmup) warmup = 0;
		minTime = Long.MAX_VALUE;
		maxTime = Long.MIN_VALUE;
		
		size = 0;
		Iterator<MutableInt> iter = results.iterator();
		while(iter.hasNext()) {
			releaseMutableInt(iter.next());
		}
		results.clear();
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
			
			MutableInt count = results.get(lastNanoTime);
			if (count == null) {
				results.put(lastNanoTime, getMutableInt(1));
			} else {
				count.set(count.get() + 1);
			}
			size++;
			return true;
		}
		return false;
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
		
		long x = Math.round(perc * size);
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
	
    public int getCount() {
    	return count;
    }
    
    public int getSize() {
    	return size;
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

	public String results() {
		final StringBuilder sb = new StringBuilder(128);
		final long realCount = count - warmup;
		sb.append("Measurements: ").append(formatter.format(realCount));
		sb.append(" | Warm-Up: ").append(formatter.format(warmup));
		sb.append(" | Iterations: ").append(formatter.format(count)).append('\n');
		sb.append("Avg Time: ").append(convertNanoTime(avg()));
		if (realCount > 0) {
			sb.append(" | Min Time: ").append(convertNanoTime(minTime));
			sb.append(" | Max Time: ").append(convertNanoTime(maxTime));
		}
		sb.append('\n');
		
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
		
		return sb.toString();
	}
	
	public void printResults() {
		System.out.println(results());
	}
	
}

