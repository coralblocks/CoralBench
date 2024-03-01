package com.coralblocks.coralbench.bench;

import com.coralblocks.coralbench.Bench;
import com.coralblocks.coralbench.util.LongMap;
import com.coralblocks.coralbench.util.Random;
import com.coralblocks.coralbench.util.ThreadPinning;

public class LongMapGetBench {
	
	private static final long THE_SEED = 123456789L;
	
	public static void main(String[] args) {
		
		final int iterations = Integer.parseInt(args[0]);
		
		final int passes = Integer.parseInt(args[1]);
		
		final int coreId = Integer.parseInt(args[2]);
		
		if (coreId >= 0) ThreadPinning.pinCurrentThread(coreId);
		
		final String filler = new String("FILLER");
		
		LongMap<String> map = new LongMap<String>(8388608); // 2 ^ 23
		
		Random rand = new Random(THE_SEED);
		
		Bench bench = new Bench();
		
		for(int i = 0; i < iterations; i++) map.put(rand.nextLong(), filler);
		
		System.out.println("Benchmarking GET operation...\n");
		
		for(int pass = 0; pass < passes; pass++) {
			
			bench.reset();
			rand.reset();
			
			String gotten = null;
		
			for(int i = 0; i < iterations; i++) {
				
				long key = rand.nextLong(); // this is deterministic (pseudo-random)
				
				bench.mark();
				gotten = map.get(key);
				bench.measure();
			}
			
			System.out.println("Last object gotten: " + gotten);
			System.out.println();
			bench.printResults();
		}
	}
}