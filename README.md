# CoralBench
A <code>Benchmark</code> class to measure latency in Java code with percentiles.

## How to use it
```Java
public class SleepBenchmark {
    
    private final static void sleepFor(long nanos) {
        long time = System.nanoTime();
        while((System.nanoTime() - time) < nanos);
    }
    
    private final static void doSleep(Bench bench) {
        bench.mark(); // <===== timer starts
        sleepFor(1000);
        bench.measure(); // <===== timer stops
    }
    
    public static void main(String[] args) {
        
        final int warmupIterations = 1_000_000;
        final int measurementIterations = 2_000_000;
        final int totalIterations = measurementIterations + warmupIterations;
        
        Bench bench = new Bench(warmupIterations); // specify the number of warmup iterations to ignore
        
        while(bench.getIterations() < totalIterations) { // note that we always perform warmup + measurement iterations
            doSleep(bench);
        }
        
        bench.printResults();
    }
}
```
#### Output:
```Plain
Measurements: 2,000,000 | Warm-Up: 1,000,000 | Iterations: 3,000,000
Avg Time: 1.025 micros | Min Time: 1.000 micro | Max Time: 56.500 micros
75% = [avg: 1.016 micros, max: 1.042 micros]
90% = [avg: 1.020 micros, max: 1.042 micros]
99% = [avg: 1.022 micros, max: 1.042 micros]
99.9% = [avg: 1.022 micros, max: 1.125 micros]
99.99% = [avg: 1.023 micros, max: 7.791 micros]
99.999% = [avg: 1.024 micros, max: 15.959 micros]
```
The full <code>SleepBenchmark</code> source code can be seen [here](src/main/java/com/coralblocks/coralbench/example/SleepBenchmark.java)

### Measuring the elapsed time yourself
```Java
private final static void doSleep(Bench bench) {
    long start = System.nanoTime(); // <===== timer starts
    sleepFor(1000);
    long elapsed = System.nanoTime() - start; // <===== timer stops
    bench.measure(elapsed); // <===== provide the elapsed time yourself
}
```
## More examples
- [MathBenchmark](src/main/java/com/coralblocks/coralbench/example/MathBenchmark.java)
- [BubbleSortBenchmark](src/main/java/com/coralblocks/coralbench/example/BubbleSortBenchmark.java)
- [SleepBenchmark](src/main/java/com/coralblocks/coralbench/example/SleepBenchmark.java)

## Forcing the HotSpot JVM to optimize at startup (-Xcomp -XX:-TieredCompilation)

By default, the HotSpot JVM will take some time to profile a running Java application for hot spots in the code and then optimize by compiling (to assembly) and inlining these hot spot methods. That's great because the HotSpot JVM can surgically and agressively optimize the parts of your program that matter the most instead of taking the AoT (ahead-of-time) approach of compiling and trying to optimize the whole thing. `For example, method inlining is an aggressive form of optimization that usually requires runtime profiling information since inlining everything is impractical/impossible.`

The HotSpot JVM offers an interesting option to compile every method right before its first invocation. The drawback is that without any profiling this optimization is conservative. `For example, even though some basic method inlining is still performed, a more agressive inlining approach does not happen.` The advantage is that your application will be able to perform at a native/assembly level <strong>right when it starts</strong>, without having to wait until the HotSpot JVM have gathered enough profiling to compile and optimize the hot methods.

Below we explore some of our benchmark examples to better understand the different approaches offered by the HotSpot JVM:

