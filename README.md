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

## The battle between JIT (HotSpotVM) and AOT (GraalVM)

Here we run all our example benchmarks in three different ways, to compare the performance:
- Oracle HotSpotVM 23 JIT
- GraalVM 23 Native-Image AOT without profiling
- GraalVM 23 Native-Image AOT with profiling

#### Linux Environment
```
$ uname -a
Linux hivelocity 4.15.0-20-generic #21-Ubuntu SMP Tue Apr 24 06:16:15 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux

$ cat /etc/issue | head -n 1
Ubuntu 18.04.6 LTS \n \l

$ gcc --version | head -n 1
gcc (Ubuntu 11.1.0-1ubuntu1~18.04.1) 11.1.0

$ cat /proc/cpuinfo | grep "model name" | head -n 1 | awk -F ": " '{print $NF}'
Intel(R) Xeon(R) E-2288G CPU @ 3.70GHz

$ arch
x86_64
```
#### JVMs
```
java version "23.0.1" 2024-10-15
Java(TM) SE Runtime Environment (build 23.0.1+11-39)
Java HotSpot(TM) 64-Bit Server VM (build 23.0.1+11-39, mixed mode, sharing)

java version "23.0.1" 2024-10-15
Java(TM) SE Runtime Environment Oracle GraalVM 23.0.1+11.1 (build 23.0.1+11-jvmci-b01)
Java HotSpot(TM) 64-Bit Server VM Oracle GraalVM 23.0.1+11.1 (build 23.0.1+11-jvmci-b01, mixed mode, sharing)

native-image 23.0.1 2024-10-15
GraalVM Runtime Environment Oracle GraalVM 23.0.1+11.1 (build 23.0.1+11-jvmci-b01)
Substrate VM Oracle GraalVM 23.0.1+11.1 (build 23.0.1+11, serial gc, compressed references)
```

#### SleepBenchmark
HotSpotVM:
```
java -cp target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark
```
```
Measurements: 2,000,000 | Warm-Up: 1,000,000 | Iterations: 3,000,000
Avg Time: 1.043 micros | Min Time: 1.030 micros | Max Time: 30.578 micros
75% = [avg: 1.039 micros, max: 1.047 micros]
90% = [avg: 1.040 micros, max: 1.048 micros]
99% = [avg: 1.041 micros, max: 1.051 micros]
99.9% = [avg: 1.041 micros, max: 1.105 micros]
99.99% = [avg: 1.042 micros, max: 1.923 micros]
99.999% = [avg: 1.042 micros, max: 15.565 micros]
```
GraalVM without profiling:
```
native-image -cp target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark \
             -o bin/SleepBenchmark -march=native --gc=G1 -O3 --native-compiler-options="-O3"
```

