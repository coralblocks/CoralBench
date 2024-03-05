package com.coralblocks.coralbench.bench;

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
		
		int warmupIterations = 1000000;
		int measurementIterations = 1000000;
		
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
}