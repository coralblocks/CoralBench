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
<!--
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
HotSpotVM JIT:
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
GraalVM AOT without profiling:
```
native-image -cp target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark \
             -o bin/SleepBenchmark -march=native --gc=G1 -O3 --native-compiler-options="-O3"
```
```
Measurements: 2,000,000 | Warm-Up: 1,000,000 | Iterations: 3,000,000
Avg Time: 1.041 micros | Min Time: 1.034 micros | Max Time: 342.684 micros
75% = [avg: 1.039 micros, max: 1.041 micros]
90% = [avg: 1.039 micros, max: 1.042 micros]
99% = [avg: 1.039 micros, max: 1.045 micros]
99.9% = [avg: 1.040 micros, max: 1.053 micros]
99.99% = [avg: 1.040 micros, max: 1.903 micros]
99.999% = [avg: 1.040 micros, max: 15.600 micros]
```
GraalVM AOT with profiling:
```
native-image -cp target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark \
             -o bin/SleepBenchmark -march=native --gc=G1 --pgo-instrument --native-compiler-options="-O3"
native-image -cp target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark \
             -o bin/SleepBenchmark -march=native --gc=G1 --pgo=default.iprof -O3 --native-compiler-options="-O3"
```
```
Measurements: 2,000,000 | Warm-Up: 1,000,000 | Iterations: 3,000,000
Avg Time: 1.043 micros | Min Time: 1.030 micros | Max Time: 25.722 micros
75% = [avg: 1.040 micros, max: 1.046 micros]
90% = [avg: 1.041 micros, max: 1.048 micros]
99% = [avg: 1.042 micros, max: 1.051 micros]
99.9% = [avg: 1.042 micros, max: 1.090 micros]
99.99% = [avg: 1.042 micros, max: 1.920 micros]
99.999% = [avg: 1.043 micros, max: 15.760 micros]
```

#### MathBenchmark
HotSpotVM JIT:
```
java -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark
```
```
Value computed: -550000000000
Measurements: 9,000,000 | Warm-Up: 1,000,000 | Iterations: 10,000,000
Avg Time: 217.690 nanos | Min Time: 148.000 nanos | Max Time: 24.736 micros
75% = [avg: 198.000 nanos, max: 271.000 nanos]
90% = [avg: 210.000 nanos, max: 271.000 nanos]
99% = [avg: 216.000 nanos, max: 273.000 nanos]
99.9% = [avg: 217.000 nanos, max: 484.000 nanos]
99.99% = [avg: 217.000 nanos, max: 1.185 micros]
99.999% = [avg: 217.000 nanos, max: 5.538 micros]
```
GraalVM AOT without profiling:
```
native-image -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark \
             -o bin/MathBenchmark -march=native --gc=G1 -O3 --native-compiler-options="-O3"
```
```
Value computed: -550000000000
Measurements: 9,000,000 | Warm-Up: 1,000,000 | Iterations: 10,000,000
Avg Time: 15.420 nanos | Min Time: 14.000 nanos | Max Time: 23.545 micros
75% = [avg: 15.000 nanos, max: 16.000 nanos]
90% = [avg: 15.000 nanos, max: 16.000 nanos]
99% = [avg: 15.000 nanos, max: 17.000 nanos]
99.9% = [avg: 15.000 nanos, max: 17.000 nanos]
99.99% = [avg: 15.000 nanos, max: 21.000 nanos]
99.999% = [avg: 15.000 nanos, max: 28.000 nanos]
```
GraalVM AOT with profiling:
```
native-image -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark \
             -o bin/MathBenchmark -march=native --gc=G1 --pgo-instrument --native-compiler-options="-O3"
native-image -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark \
             -o bin/MathBenchmark -march=native --gc=G1 --pgo=default.iprof --native-compiler-options="-O3"
```
```
Value computed: -550000000000
Measurements: 9,000,000 | Warm-Up: 1,000,000 | Iterations: 10,000,000
Avg Time: 14.980 nanos | Min Time: 13.000 nanos | Max Time: 15.185 micros
75% = [avg: 14.000 nanos, max: 15.000 nanos]
90% = [avg: 14.000 nanos, max: 16.000 nanos]
99% = [avg: 14.000 nanos, max: 16.000 nanos]
99.9% = [avg: 14.000 nanos, max: 16.000 nanos]
99.99% = [avg: 14.000 nanos, max: 22.000 nanos]
99.999% = [avg: 14.000 nanos, max: 906.000 nanos]
```

#### BubbleSortBenchmark
HotSpotVM JIT:
```
java -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark
```
```
Value computed: 20130000000
Array: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60]
Measurements: 10,000,000 | Warm-Up: 1,000,000 | Iterations: 11,000,000
Avg Time: 838.090 nanos | Min Time: 804.000 nanos | Max Time: 379.253 micros
75% = [avg: 829.000 nanos, max: 840.000 nanos]
90% = [avg: 831.000 nanos, max: 845.000 nanos]
99% = [avg: 833.000 nanos, max: 864.000 nanos]
99.9% = [avg: 836.000 nanos, max: 1.491 micros]
99.99% = [avg: 837.000 nanos, max: 2.376 micros]
99.999% = [avg: 837.000 nanos, max: 16.052 micros]
```
GraalVM AOT without profiling:
```
native-image -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark \
             -o bin/BubbleSortBenchmark -march=native --gc=G1 -O3 --native-compiler-options="-O3"
```
```
Value computed: 20130000000
Array: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60]
Measurements: 10,000,000 | Warm-Up: 1,000,000 | Iterations: 11,000,000
Avg Time: 2.184 micros | Min Time: 2.083 micros | Max Time: 373.551 micros
75% = [avg: 2.154 micros, max: 2.190 micros]
90% = [avg: 2.164 micros, max: 2.246 micros]
99% = [avg: 2.174 micros, max: 2.339 micros]
99.9% = [avg: 2.180 micros, max: 3.643 micros]
99.99% = [avg: 2.181 micros, max: 8.462 micros]
99.999% = [avg: 2.183 micros, max: 19.133 micros]
```
GraalVM AOT with profiling:
```
native-image -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark \
             -o bin/BubbleSortBenchmark -march=native --gc=G1 --pgo-instrument --native-compiler-options="-O3"
native-image -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark \
             -o bin/BubbleSortBenchmark -march=native --gc=G1 --pgo=default.iprof --native-compiler-options="-O3"
```
```
Value computed: 20130000000
Array: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60]
Measurements: 10,000,000 | Warm-Up: 1,000,000 | Iterations: 11,000,000
Avg Time: 2.050 micros | Min Time: 1.971 micros | Max Time: 39.075 micros
75% = [avg: 2.028 micros, max: 2.052 micros]
90% = [avg: 2.033 micros, max: 2.065 micros]
99% = [avg: 2.038 micros, max: 2.194 micros]
99.9% = [avg: 2.046 micros, max: 3.706 micros]
99.99% = [avg: 2.048 micros, max: 8.486 micros]
99.999% = [avg: 2.049 micros, max: 18.493 micros]
```
-->
