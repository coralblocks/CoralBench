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
		
		final int iterations = Integer.parseInt(args[0]);
		
		final int passes = Integer.parseInt(args[1]);
		
		final int coreId = Integer.parseInt(args[2]);
		
		if (coreId >= 0) ThreadPinning.pinCurrentThread(coreId);
		
		final String filler = new String("FILLER");
		
		LongMap<String> map = new LongMap<String>(8388608); // 2 ^ 23
		
		Random rand = new Random(THE_SEED);
		
		Bench bench = new Bench();
		
		System.out.println("\nBenchmarking put operation...\n");
		
		for(int pass = 0; pass < passes; pass++) {

			map.clear(); // we will re-insert everything...
			bench.reset();
			rand.reset();
			
			long time = System.nanoTime();
			
			for(int i = 0; i < iterations; i++) {
				
				long key = rand.nextLong(); // this is deterministic (pseudo-random)
				
				bench.mark();
				map.put(key, filler);
				bench.measure();
			}
			
			time = System.nanoTime() - time;
			
			System.out.println("Total time in nanoseconds: " + FORMATTER.format(time));
			System.out.println("Final size of map: " + map.size());
			System.out.println();
			bench.printResults();
		}
		
		System.out.println("Benchmarking get operation...\n");
		
		for(int pass = 0; pass < passes; pass++) {
			
			bench.reset();
			rand.reset();
			
			String gotten = null;
		
			long time = System.nanoTime();
			
			for(int i = 0; i < iterations; i++) {
				
				long key = rand.nextLong(); // this is deterministic (pseudo-random)
				
				bench.mark();
				gotten = map.get(key);
				bench.measure();
			}
			
			time = System.nanoTime() - time;
			
			System.out.println("Total time in nanoseconds: " + FORMATTER.format(time));
			System.out.println("Last object gotten: " + gotten);
			System.out.println();
			bench.printResults();
		}
		
		System.out.println("Benchmarking remove operation...\n");
		
		for(int pass = 0; pass < passes; pass++) {
			
			bench.reset();
			rand.reset();
			
			String removed = null;
		
			long time = System.nanoTime();
			
			for(int i = 0; i < iterations; i++) {
				
				long key = rand.nextLong(); // this is deterministic (pseudo-random)
				
				bench.mark();
				removed = map.remove(key);
				bench.measure();
			}
			
			time = System.nanoTime() - time;
			
			System.out.println("Total time in nanoseconds: " + FORMATTER.format(time));
			System.out.println("Last object removed: " + removed);
			System.out.println("Final size of map: " + map.size());
			System.out.println();
			bench.printResults();
		}
	}
}