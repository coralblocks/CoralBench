package com.coralblocks.coralbench.bug;

public class OracleJvm23MathBug {
	
	// simple and small amount of math
	// =====> should be optimized/compiled/inlined for sure!
    private static final long doSomething(int load, int i) {
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
    
    /*
     * Execute this with OpenJDK/Zulu/Oracle JVM 23 => average 215 nanoseconds
     * Now execute this with Graal23 JVM 23 => average 7 nanoseconds
     * 
     * This bug can be observed in any platform (I tested on Linux and Mac)
     * 
     * $ java -version
     * java version "23.0.1" 2024-10-15
     * Java(TM) SE Runtime Environment (build 23.0.1+11-39)
     * Java HotSpot(TM) 64-Bit Server VM (build 23.0.1+11-39, mixed mode, sharing)
     * 
     * $ java -cp target/coralbench-all.jar com.coralblocks.coralbench.bug.OracleJvm23MathBug
     * Value computed: -550000000000
	 * Measurements: 10000000| Avg Time: 215 nanos | Min Time: 83 nanos | Max Time: 199750 nanos
     * 
     * $ java -version
     * java version "23.0.1" 2024-10-15
	 * Java(TM) SE Runtime Environment Oracle GraalVM 23.0.1+11.1 (build 23.0.1+11-jvmci-b01)
	 * Java HotSpot(TM) 64-Bit Server VM Oracle GraalVM 23.0.1+11.1 (build 23.0.1+11-jvmci-b01, mixed mode, sharing)
     * 
     * $ java -cp target/coralbench-all.jar com.coralblocks.coralbench.bug.OracleJvm23MathBug
     * Value computed: -550000000000
	 * Measurements: 10000000| Avg Time: 7 nanos | Min Time: 0 nanos | Max Time: 178625 nanos
     */
	public static final void main(String[] args) {
		final int iterations = 10_000_000;
		final int load = 10_000;
		NanoBench bench = new NanoBench();
		long computed = 0;
		for (int i = 0; i < iterations; i++) {
			bench.mark();
			computed += doSomething(load, i);
			bench.measure();
		}
		System.out.println("Value computed: " + computed);
		bench.printResults();
	}
	
	private static class NanoBench {

		private int measurements;
		private long totalTime, minTime, maxTime, time;
		private final StringBuilder sb = new StringBuilder(128);
		
		NanoBench() {
			reset();
		}
		
		public final void reset() {
			totalTime = time = measurements = 0;
			maxTime = Long.MIN_VALUE;
			minTime = Long.MAX_VALUE;
		}
		
		public final void mark() {
			time = System.nanoTime();
		}

		public final void measure() {
			long lastNanoTime = System.nanoTime() - time;
			totalTime += lastNanoTime;
			minTime = lastNanoTime < minTime ? lastNanoTime : minTime;
			maxTime = lastNanoTime > maxTime ? lastNanoTime : maxTime;
			measurements++;
		}
		
		public final void printResults() {
			sb.setLength(0);
			sb.append("Measurements: ").append(measurements);
			sb.append("| Avg Time: ").append((long) (totalTime / (double) measurements)).append(" nanos");
			sb.append(" | Min Time: ").append(minTime).append(" nanos");
			sb.append(" | Max Time: ").append(maxTime).append(" nanos\n\n");
			for(int i = 0; i < sb.length(); i++) System.out.print(sb.charAt(i));
		}
	}
}