package com.coralblocks.coralbench.bench;

import com.coralblocks.coralbench.Bench;

public class MathBenchmark {
	
    private final static long doSomething(int load, int i) {
        
        long x = 0;
         
        for(int j = 0; j < load; j++) {
            long pow = (i % 8) * (i % 16);
            if (i % 2 == 0) {
                x += pow;
            } else {
                x -= pow;
            }
        }
         
        return x;
    }
    
	public static void main(String[] args) {
		
		int warmupIterations = 1000000;
		int measurementIterations = 9000000;
		int load = 10000;
		
		Bench bench = new Bench(warmupIterations);
		
		long x = 0;
		
		for(int i = 0; i < measurementIterations + warmupIterations; i++) {
			bench.mark();
			x += doSomething(load, i);
			bench.measure();
		}
		
		System.out.println("Value computed: " + x);
		
		bench.printResults();
	}
}