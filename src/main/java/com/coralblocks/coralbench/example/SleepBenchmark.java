package com.coralblocks.coralbench.example;

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
		
		final int warmupIterations = 1000000;
		final int measurementIterations = 1000000;
		final int total = measurementIterations + warmupIterations;
		
		Bench bench = new Bench(warmupIterations);
		
		while(bench.getCount() < total) {
			doSleep(bench);
		}
		
		bench.printResults();
	}
}