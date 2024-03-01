package com.coralblocks.coralbench.bench;

import java.text.DecimalFormat;

import com.coralblocks.coralbench.Bench;
import com.coralblocks.coralbench.util.LongMap;
import com.coralblocks.coralbench.util.Random;
import com.coralblocks.coralbench.util.ThreadPinning;

public class LongMapBench {
	
	private static final long THE_SEED = 123456789L;
	private static final DecimalFormat FORMATTER = new DecimalFormat("#,###");
	
	public static void main(String[] args) {
		
		final int warmupIterations = Integer.parseInt(args[0]);
		
		final int benchIterations = Integer.parseInt(args[1]);
		
		final int passes = Integer.parseInt(args[2]);
		
		final int coreId = Integer.parseInt(args[3]);
		
		if (coreId >= 0) ThreadPinning.pinCurrentThread(coreId);
		
		final String filler = new String("FILLER");
		
		LongMap<String> map = new LongMap<String>(8388608); // 2 ^ 23
		
		Random rand = new Random(THE_SEED);
		
		Bench bench = new Bench(warmupIterations);
		
		System.out.println();
		
		System.out.println("Benchmarking put operation...");
		
		System.out.println();
		
		for(int pass = 0; pass < passes; pass++) {

			if (pass > 0) { // no need to reset on the first pass...
				bench.reset(false);
				rand.reset();
			}
			
			// first pass has warmup... after that there is no warmup for subsequent passes...
			int totalIterations = pass == 0 ? (warmupIterations + benchIterations) : benchIterations;
			
			map.clear();
		
			long time = System.nanoTime();
			
			for(int i = 0; i < totalIterations; i++) {
				
				long key = rand.nextLong();
				
				bench.mark();
				map.put(key, filler);
				bench.measure();
			}
			
			time = System.nanoTime() - time;
			
			System.out.println("Total time in nanoseconds: " + FORMATTER.format(time) +  " (" + (pass == 0 ? "with" : "without") + " warmup)");
			System.out.println("Final size of map: " + map.size());
			System.out.println();
			bench.printResults();
		}
		
		System.out.println("Benchmarking get operation...");
		
		System.out.println();
		
		rand.reset(); // first pass below will not be reset so reset here...
		
		for(int pass = 0; pass < passes; pass++) {
			
			if (pass > 0) { // no need to reset on the first pass...
				bench.reset(false);
				rand.reset();
			}
			
			// first pass has warmup... after that there is no warmup for subsequent passes...
			int totalIterations = pass == 0 ? (warmupIterations + benchIterations) : benchIterations;
			
			String gotten = null;
		
			long time = System.nanoTime();
			
			for(int i = 0; i < totalIterations; i++) {
				
				long key = rand.nextLong();
				
				bench.mark();
				gotten = map.get(key);
				bench.measure();
			}
			
			time = System.nanoTime() - time;
			
			System.out.println("Total time in nanoseconds: " + FORMATTER.format(time) +  " (" + (pass == 0 ? "with" : "without") + " warmup)");
			System.out.println("Last object gotten: " + gotten);
			System.out.println();
			bench.printResults();
		}
	}
}