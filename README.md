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

        // specify the number of warmup iterations to ignore
        Bench bench = new Bench(warmupIterations);

        // note that we always perform warmup + measurement iterations
        while(bench.getIterations() < totalIterations) {
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

## Forcing the JVM to optimize at startup (-Xcomp -XX:-TieredCompilation)

By default, the HotSpot JVM will take some time to profile a running Java application for hot spots in the code and then optimize by compiling (to assembly) and inlining these hot spot methods. That's great because the HotSpot JVM can surgically and aggressively optimize the parts of your program that matter the most instead of taking the AoT (ahead-of-time) approach of compiling and trying to optimize the whole thing beforehand. `For example, method inlining is an aggressive form of optimization that usually requires runtime profiling information since inlining everything is impractical/impossible.`

The HotSpot JVM offers an interesting startup option `-Xcomp` to compile every method right before its first invocation. The drawback is that without any profiling this optimization is conservative. `For example, even though some basic method inlining is still performed, a more aggressive inlining approach does not happen.` The advantage is that your application will be able to perform at a native/assembly level <strong>right when it starts</strong>, without having to wait until the HotSpot JVM have gathered enough profiling to compile and optimize the hot methods.

Below we explore some of our benchmark examples to better understand the difference in performance that these approaches can make.

<details>
  <summary>Benchmark Environment</summary>
    
```
$ java -version
java version "23.0.1" 2024-10-15
Java(TM) SE Runtime Environment (build 23.0.1+11-39)
Java HotSpot(TM) 64-Bit Server VM (build 23.0.1+11-39, mixed mode, sharing)

$ uname -a
Linux hivelocity 4.15.0-20-generic #21-Ubuntu SMP Tue Apr 24 06:16:15 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux

$ cat /etc/issue | head -n 1
Ubuntu 18.04.6 LTS \n \l

$ cat /proc/cpuinfo | grep "model name" | head -n 1 | awk -F ": " '{print $NF}'
Intel(R) Xeon(R) E-2288G CPU @ 3.70GHz
```
</details>

<details>
  <summary>MathBenchmark Numbers</summary>
    
#### Regular JIT <i>with</i> warm-up
```
$ java -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark 1000000 1000
Value computed: -55054840000
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 385.210 nanos | Min Time: 200.000 nanos | Max Time: 2.020 micros
75% = [avg: 360.000 nanos, max: 385.000 nanos]
90% = [avg: 365.000 nanos, max: 401.000 nanos]
99% = [avg: 376.000 nanos, max: 609.000 nanos]
99.9% = [avg: 383.000 nanos, max: 1.730 micros]
99.99% = [avg: 385.000 nanos, max: 2.020 micros]
99.999% = [avg: 385.000 nanos, max: 2.020 micros]
```
#### Regular JIT <i>without</i> warm-up
```
$ java -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark 5 1000
Value computed: -54580000
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 4.747 micros | Min Time: 200.000 nanos | Max Time: 214.941 micros
75% = [avg: 326.000 nanos, max: 375.000 nanos]
90% = [avg: 335.000 nanos, max: 386.000 nanos]
99% = [avg: 3.292 micros, max: 131.788 micros]
99.9% = [avg: 4.536 micros, max: 190.083 micros]
99.99% = [avg: 4.746 micros, max: 214.941 micros]
99.999% = [avg: 4.746 micros, max: 214.941 micros]
```
#### -Xcomp -XX:-TieredCompilation <i>with</i> warm-up
```
$ java -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark 1000000 1000
Value computed: -55054840000
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 96.430 nanos | Min Time: 90.000 nanos | Max Time: 3.987 micros
75% = [avg: 91.000 nanos, max: 92.000 nanos]
90% = [avg: 91.000 nanos, max: 93.000 nanos]
99% = [avg: 91.000 nanos, max: 96.000 nanos]
99.9% = [avg: 92.000 nanos, max: 406.000 nanos]
99.99% = [avg: 96.000 nanos, max: 3.987 micros]
99.999% = [avg: 96.000 nanos, max: 3.987 micros]
```
#### -Xcomp -XX:-TieredCompilation <i>without</i> warm-up
```
$ java -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark 5 1000
Value computed: -54580000
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 119.360 nanos | Min Time: 116.000 nanos | Max Time: 428.000 nanos
75% = [avg: 117.000 nanos, max: 119.000 nanos]
90% = [avg: 118.000 nanos, max: 120.000 nanos]
99% = [avg: 118.000 nanos, max: 128.000 nanos]
99.9% = [avg: 119.000 nanos, max: 408.000 nanos]
99.99% = [avg: 119.000 nanos, max: 428.000 nanos]
99.999% = [avg: 119.000 nanos, max: 428.000 nanos]
```
</details>
<details>
  <summary>BubbleSortBenchmark Numbers</summary>
    
#### Regular JIT <i>with</i> warm-up
```
$ java -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark 1000000 1000
Value computed: 1831830000
Array: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60]
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 1.228 micros | Min Time: 807.000 nanos | Max Time: 10.977 micros
75% = [avg: 1.145 micros, max: 1.312 micros]
90% = [avg: 1.177 micros, max: 1.375 micros]
99% = [avg: 1.198 micros, max: 1.478 micros]
99.9% = [avg: 1.218 micros, max: 9.687 micros]
99.99% = [avg: 1.227 micros, max: 10.977 micros]
99.999% = [avg: 1.227 micros, max: 10.977 micros]
```
#### Regular JIT <i>without</i> warm-up
```
$ java -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark 5 1000
Value computed: 1839150
Array: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60]
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 11.005 micros | Min Time: 916.000 nanos | Max Time: 91.532 micros
75% = [avg: 5.231 micros, max: 16.042 micros]
90% = [avg: 7.125 micros, max: 17.370 micros]
99% = [avg: 10.347 micros, max: 71.260 micros]
99.9% = [avg: 10.924 micros, max: 84.416 micros]
99.99% = [avg: 11.004 micros, max: 91.532 micros]
99.999% = [avg: 11.004 micros, max: 91.532 micros]
```
#### -Xcomp -XX:-TieredCompilation <i>with</i> warm-up
```
$ java -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark 1000000 1000
Value computed: 1831830000
Array: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60]
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 1.114 micros | Min Time: 1.083 micros | Max Time: 1.696 micros
75% = [avg: 1.107 micros, max: 1.118 micros]
90% = [avg: 1.109 micros, max: 1.125 micros]
99% = [avg: 1.111 micros, max: 1.146 micros]
99.9% = [avg: 1.113 micros, max: 1.682 micros]
99.99% = [avg: 1.113 micros, max: 1.696 micros]
99.999% = [avg: 1.113 micros, max: 1.696 micros]
```
#### -Xcomp -XX:-TieredCompilation <i>without</i> warm-up
```
$ java -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark 5 1000
Value computed: 1839150
Array: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60]
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 1.126 micros | Min Time: 1.079 micros | Max Time: 7.655 micros
75% = [avg: 1.109 micros, max: 1.128 micros]
90% = [avg: 1.113 micros, max: 1.142 micros]
99% = [avg: 1.116 micros, max: 1.164 micros]
99.9% = [avg: 1.119 micros, max: 1.875 micros]
99.99% = [avg: 1.126 micros, max: 7.655 micros]
99.999% = [avg: 1.126 micros, max: 7.655 micros]
```
</details>
<details>
  <summary>SleepBenchmark Numbers</summary>
    
#### Regular JIT <i>with</i> warm-up
```
$ java -cp target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark 1000000 1000
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 1.088 micros | Min Time: 1.038 micros | Max Time: 8.965 micros
75% = [avg: 1.053 micros, max: 1.078 micros]
90% = [avg: 1.060 micros, max: 1.110 micros]
99% = [avg: 1.066 micros, max: 1.189 micros]
99.9% = [avg: 1.080 micros, max: 7.271 micros]
99.99% = [avg: 1.088 micros, max: 8.965 micros]
99.999% = [avg: 1.088 micros, max: 8.965 micros]
```
#### Regular JIT <i>without</i> warm-up
```
$ java -cp target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark 5 1000
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 1.149 micros | Min Time: 1.068 micros | Max Time: 9.275 micros
75% = [avg: 1.107 micros, max: 1.138 micros]
90% = [avg: 1.114 micros, max: 1.161 micros]
99% = [avg: 1.123 micros, max: 1.740 micros]
99.9% = [avg: 1.140 micros, max: 5.375 micros]
99.99% = [avg: 1.149 micros, max: 9.275 micros]
99.999% = [avg: 1.149 micros, max: 9.275 micros]
```
#### -Xcomp -XX:-TieredCompilation <i>with</i> warm-up
```
$ java -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark 1000000 1000
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 1.046 micros | Min Time: 1.035 micros | Max Time: 1.095 micros
75% = [avg: 1.045 micros, max: 1.048 micros]
90% = [avg: 1.046 micros, max: 1.048 micros]
99% = [avg: 1.046 micros, max: 1.049 micros]
99.9% = [avg: 1.046 micros, max: 1.089 micros]
99.99% = [avg: 1.046 micros, max: 1.095 micros]
99.999% = [avg: 1.046 micros, max: 1.095 micros]
```
#### -Xcomp -XX:-TieredCompilation <i>without</i> warm-up
```
$ java -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark 5 1000
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 1.048 micros | Min Time: 1.034 micros | Max Time: 3.490 micros
75% = [avg: 1.044 micros, max: 1.048 micros]
90% = [avg: 1.045 micros, max: 1.049 micros]
99% = [avg: 1.045 micros, max: 1.050 micros]
99.9% = [avg: 1.045 micros, max: 1.073 micros]
99.99% = [avg: 1.048 micros, max: 3.490 micros]
99.999% = [avg: 1.048 micros, max: 3.490 micros]
```
</details>

As you can see from the latency numbers above, by using `-Xcomp -XX:-TieredCompilation` you may be able to `eliminate the need for your application to warm up by paying a small price in performance`. Of course this conclusion cannot be generalized for every application as it will depend heavily on the characteristics and particularities of the source code and its critical path. `But it is worth giving -Xcomp -XX:-TieredCompilation a try to see what kind of numbers you get and to evaluate if the trade-off is worth it.`

###### For how this applies to [CoralSequencer](https://www.coralblocks.com/coralsequencer) you can check [this article](https://www.coralblocks.com/index.php/hotspot-jit-aot-and-warm-up/).

## HotSpotVM (-Xcomp and JIT) vs LLVM (clang) vs GraalVM

In this section, we compare the `HotSpotVM` JIT against three forms of AOT: `LLVM` (clang), `GraalVM` (native-image), and `-Xcomp`. To ensure a fair and unbiased comparison, all Java and C++ source code is designed to be _equivalent_. Not only the `IntMap` class, which is the code being measured, but also the `Bench` class, which performs the measurements. `We made every effort to maintain consistency and fairness in the comparison but there might be aspects we have overlooked.` **If you notice anything that seems incorrect or could be improved, especially in the C++ code, please feel free to open an issue or submit a pull request.**

The Java `IntMap` implementation [is here](src/main/java/com/coralblocks/coralbench/example/IntMap.java). The C++ `IntMap` implementation [is here](src/main/c/int_map.cpp).<br/>
The Java benchmark code [is here](src/main/java/com/coralblocks/coralbench/example/IntMapBenchmark.java). The C++ benchmark code [is here](src/main/c/int_map_benchmark.cpp).<br/>
The Java `Bench` class [is here](src/main/java/com/coralblocks/coralbench/Bench.java). The C++ `Bench` class [is here](src/main/c/bench.cpp).<br/>

### Test Environment

```
$ uname -a
Linux hivelocity 4.15.0-20-generic #21-Ubuntu SMP Tue Apr 24 06:16:15 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux

$ cat /etc/issue | head -n 1
Ubuntu 18.04.6 LTS \n \l

$ cat /proc/cpuinfo | grep "model name" | head -n 1 | awk -F ": " '{print $NF}'
Intel(R) Xeon(R) E-2288G CPU @ 3.70GHz

$ arch
x86_64

$ clang++ --version
Ubuntu clang version 18.1.0 (++20240220094926+390dcd4cbbf5-1~exp1~20240220214944.50)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin

$ java -version
java version "23.0.1" 2024-10-15
Java(TM) SE Runtime Environment Oracle GraalVM 23.0.1+11.1 (build 23.0.1+11-jvmci-b01)
Java HotSpot(TM) 64-Bit Server VM Oracle GraalVM 23.0.1+11.1 (build 23.0.1+11-jvmci-b01, mixed mode, sharing)

$ native-image --version
native-image 23.0.1 2024-10-15
GraalVM Runtime Environment Oracle GraalVM 23.0.1+11.1 (build 23.0.1+11-jvmci-b01)
Substrate VM Oracle GraalVM 23.0.1+11.1 (build 23.0.1+11, serial gc, compressed references)
```

### HotSpotVM JIT

```
$ java -XX:+AlwaysPreTouch -Xms4g -Xmx4g -XX:NewSize=512m -XX:MaxNewSize=1024m \
       -cp target/classes:target/coralbench-all.jar \
       com.coralblocks.coralbench.example.IntMapBenchmark 0 10000000 5000000

Arguments: warmup=0 measurements=10000000 mapCapacity=5000000

Benchmarking put...
Measurements: 10,000,000 | Warm-Up: 0 | Iterations: 10,000,000
Avg Time: 22.690 nanos | Min Time: 18.000 nanos | Max Time: 32.311 micros
75% = [avg: 20.000 nanos, max: 22.000 nanos]
90% = [avg: 20.000 nanos, max: 28.000 nanos]
99% = [avg: 21.000 nanos, max: 59.000 nanos]
99.9% = [avg: 22.000 nanos, max: 103.000 nanos]
99.99% = [avg: 22.000 nanos, max: 408.000 nanos]
99.999% = [avg: 22.000 nanos, max: 4.592 micros]

Benchmarking get...
Measurements: 10,000,000 | Warm-Up: 0 | Iterations: 10,000,000
Avg Time: 18.930 nanos | Min Time: 14.000 nanos | Max Time: 23.941 micros
75% = [avg: 16.000 nanos, max: 18.000 nanos]
90% = [avg: 16.000 nanos, max: 27.000 nanos]
99% = [avg: 18.000 nanos, max: 76.000 nanos]
99.9% = [avg: 18.000 nanos, max: 96.000 nanos]
99.99% = [avg: 18.000 nanos, max: 383.000 nanos]
99.999% = [avg: 18.000 nanos, max: 2.139 micros]

Benchmarking remove...
Measurements: 10,000,000 | Warm-Up: 0 | Iterations: 10,000,000
Avg Time: 21.680 nanos | Min Time: 16.000 nanos | Max Time: 39.305 micros
75% = [avg: 19.000 nanos, max: 22.000 nanos]
90% = [avg: 19.000 nanos, max: 26.000 nanos]
99% = [avg: 20.000 nanos, max: 79.000 nanos]
99.9% = [avg: 21.000 nanos, max: 123.000 nanos]
99.99% = [avg: 21.000 nanos, max: 421.000 nanos]
99.999% = [avg: 21.000 nanos, max: 13.391 micros]
```

### HotSpot -Xcomp

```
$ java -Xcomp -XX:-TieredCompilation \
       -XX:+AlwaysPreTouch -Xms4g -Xmx4g -XX:NewSize=512m -XX:MaxNewSize=1024m \
       -cp target/classes:target/coralbench-all.jar \
       com.coralblocks.coralbench.example.IntMapBenchmark 0 10000000 5000000

Arguments: warmup=0 measurements=10000000 mapCapacity=5000000

Benchmarking put...
Measurements: 10,000,000 | Warm-Up: 0 | Iterations: 10,000,000
Avg Time: 32.550 nanos | Min Time: 28.000 nanos | Max Time: 3.969 millis
75% = [avg: 30.000 nanos, max: 32.000 nanos]
90% = [avg: 31.000 nanos, max: 34.000 nanos]
99% = [avg: 31.000 nanos, max: 39.000 nanos]
99.9% = [avg: 31.000 nanos, max: 100.000 nanos]
99.99% = [avg: 31.000 nanos, max: 363.000 nanos]
99.999% = [avg: 31.000 nanos, max: 13.340 micros]

Benchmarking get...
Measurements: 10,000,000 | Warm-Up: 0 | Iterations: 10,000,000
Avg Time: 26.710 nanos | Min Time: 21.000 nanos | Max Time: 1.394 millis
75% = [avg: 24.000 nanos, max: 27.000 nanos]
90% = [avg: 25.000 nanos, max: 29.000 nanos]
99% = [avg: 25.000 nanos, max: 39.000 nanos]
99.9% = [avg: 26.000 nanos, max: 97.000 nanos]
99.99% = [avg: 26.000 nanos, max: 389.000 nanos]
99.999% = [avg: 26.000 nanos, max: 13.194 micros]

Benchmarking remove...
Measurements: 10,000,000 | Warm-Up: 0 | Iterations: 10,000,000
Avg Time: 28.920 nanos | Min Time: 23.000 nanos | Max Time: 1.787 millis
75% = [avg: 26.000 nanos, max: 29.000 nanos]
90% = [avg: 27.000 nanos, max: 31.000 nanos]
99% = [avg: 27.000 nanos, max: 41.000 nanos]
99.9% = [avg: 28.000 nanos, max: 100.000 nanos]
99.99% = [avg: 28.000 nanos, max: 395.000 nanos]
99.999% = [avg: 28.000 nanos, max: 13.416 micros]
```

### LLVM (clang)

```
$ clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/int_map.cpp -o ./target/cpp/int_map.o
$ clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/bench.cpp -o ./target/cpp/bench.o
$ clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/int_map_benchmark.cpp -o ./target/cpp/int_map_benchmark.o
$ clang++ -Ofast -march=native -flto -std=c++17 -o ./target/cpp/int_map_benchmark ./target/cpp/int_map.o ./target/cpp/bench.o ./target/cpp/int_map_benchmark.o

$ ./target/cpp/int_map_benchmark 0 10000000 5000000

Arguments: warmup=0 measurements=10000000 mapCapacity=5000000

Benchmarking put...
Measurements: 10,000,000 | Warm-Up: 0 | Iterations: 10,000,000
Avg Time: 38.597 nanos | Min Time: 30.000 nanos | Max Time: 31.107 micros
75% = [avg: 32.235 nanos, max: 33.000 nanos]
90% = [avg: 32.501 nanos, max: 35.000 nanos]
99% = [avg: 33.093 nanos, max: 106.000 nanos]
99.9% = [avg: 37.288 nanos, max: 884.000 nanos]
99.99% = [avg: 38.177 nanos, max: 1.316 micros]
99.999% = [avg: 38.438 nanos, max: 15.270 micros]

Benchmarking get...
Measurements: 10,000,000 | Warm-Up: 0 | Iterations: 10,000,000
Avg Time: 24.109 nanos | Min Time: 19.000 nanos | Max Time: 15.886 micros
75% = [avg: 21.578 nanos, max: 25.000 nanos]
90% = [avg: 22.200 nanos, max: 26.000 nanos]
99% = [avg: 22.978 nanos, max: 91.000 nanos]
99.9% = [avg: 23.639 nanos, max: 160.000 nanos]
99.99% = [avg: 23.884 nanos, max: 432.000 nanos]
99.999% = [avg: 23.963 nanos, max: 13.451 micros]

Benchmarking remove...
Measurements: 10,000,000 | Warm-Up: 0 | Iterations: 10,000,000
Avg Time: 24.279 nanos | Min Time: 19.000 nanos | Max Time: 29.132 micros
75% = [avg: 21.899 nanos, max: 24.000 nanos]
90% = [avg: 22.426 nanos, max: 26.000 nanos]
99% = [avg: 23.140 nanos, max: 91.000 nanos]
99.9% = [avg: 23.798 nanos, max: 153.000 nanos]
99.99% = [avg: 24.017 nanos, max: 426.000 nanos]
99.999% = [avg: 24.125 nanos, max: 13.548 micros]
```

### GraalVM (native-image)

```
$ native-image --gc=G1 -R:+AlwaysPreTouch -R:InitialHeapSize=4g -R:MaxHeapSize=4g \
               -R:InitialHeapSize=512m -R:MaxHeapSize=1024m -march=native \
               -cp target/coralbench-all.jar com.coralblocks.coralbench.example.IntMapBenchmark \
               -o target/graal/IntMapBenchmark --no-fallback -O3 --initialize-at-build-time

$ ./target/graal/IntMapBenchmark 0 10000000 5000000

Arguments: warmup=0 measurements=10000000 mapCapacity=5000000

Benchmarking put...
Measurements: 10,000,000 | Warm-Up: 0 | Iterations: 10,000,000
Avg Time: 41.110 nanos | Min Time: 25.000 nanos | Max Time: 21.841 millis
75% = [avg: 29.000 nanos, max: 31.000 nanos]
90% = [avg: 29.000 nanos, max: 32.000 nanos]
99% = [avg: 30.000 nanos, max: 41.000 nanos]
99.9% = [avg: 30.000 nanos, max: 100.000 nanos]
99.99% = [avg: 30.000 nanos, max: 339.000 nanos]
99.999% = [avg: 30.000 nanos, max: 1.573 micros]

Benchmarking get...
Measurements: 10,000,000 | Warm-Up: 0 | Iterations: 10,000,000
Avg Time: 24.560 nanos | Min Time: 19.000 nanos | Max Time: 15.679 micros
75% = [avg: 22.000 nanos, max: 25.000 nanos]
90% = [avg: 23.000 nanos, max: 26.000 nanos]
99% = [avg: 23.000 nanos, max: 39.000 nanos]
99.9% = [avg: 24.000 nanos, max: 97.000 nanos]
99.99% = [avg: 24.000 nanos, max: 382.000 nanos]
99.999% = [avg: 24.000 nanos, max: 550.000 nanos]

Benchmarking remove...
Measurements: 10,000,000 | Warm-Up: 0 | Iterations: 10,000,000
Avg Time: 35.800 nanos | Min Time: 23.000 nanos | Max Time: 165.095 micros
75% = [avg: 30.000 nanos, max: 33.000 nanos]
90% = [avg: 30.000 nanos, max: 36.000 nanos]
99% = [avg: 31.000 nanos, max: 93.000 nanos]
99.9% = [avg: 32.000 nanos, max: 123.000 nanos]
99.99% = [avg: 32.000 nanos, max: 457.000 nanos]
99.999% = [avg: 34.000 nanos, max: 63.089 micros]
```
