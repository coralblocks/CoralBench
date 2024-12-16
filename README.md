# CoralBench
A <code>Benchmark</code> class for Java and C++ to measure latency in nanoseconds with percentiles. Equivalent benchmarks in Java and C++ are provided for performance comparison and research.

Jump to the [Java vs C++](https://github.com/coralblocks/CoralBench?tab=readme-ov-file#hotspotvm--xcomp-and-jit-vs-c-llvm-clang-vs-graalvm) section.

Jump to the [-Xcomp](https://github.com/coralblocks/CoralBench?tab=readme-ov-file#forcing-the-jvm-to-optimize-at-startup--xcomp--xx-tieredcompilation) section.

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
The full <code>SleepBenchmark.java</code> source code can be seen [here](src/main/java/com/coralblocks/coralbench/example/SleepBenchmark.java)

#### Measuring the elapsed time yourself
```Java
private final static void doSleep(Bench bench) {
    long start = System.nanoTime(); // <===== timer starts
    sleepFor(1000);
    long elapsed = System.nanoTime() - start; // <===== timer stops
    bench.measure(elapsed); // <===== provide the elapsed time yourself
}
```
<br/>
<details>
  <summary>&nbsp;&nbsp;<img src="https://cdn3.emoji.gg/emojis/8241-c-plus-plus.png" width="24px" height="24px" alt="c_plus_plus"/>&nbsp;&nbsp;Click here for the C++ version </summary>

&nbsp;<br/>
```Cpp
void sleepFor(long nanos) {
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start).count();
        if (elapsed >= nanos) {
            break;
        }
    }
}

void doSleep(Bench* bench) {
    bench->mark(); // <===== timer starts
    sleepFor(1000);
    bench->measure(); // <===== timer stops
}

int main() {
    const int warmupIterations = 1'000'000;
    const int measurementIterations = 2'000'000;
    const int totalIterations = measurementIterations + warmupIterations;

    // Specify the number of warmup iterations to ignore
    Bench* bench = new Bench(warmupIterations);

    // Perform warmup + measurement iterations
    while (bench->getIterations() < totalIterations) {
        doSleep(bench);
    }

    bench->printResults();

    delete bench;

    return 0;
}
```

#### Measuring the elapsed time yourself
```Cpp
void doSleep(Bench* bench) {
    auto start = std::chrono::high_resolution_clock::now(); // <===== timer starts
    sleepFor(1000);
    auto end = std::chrono::high_resolution_clock::now();   // <===== timer stops
    long elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    bench->measure(elapsed); // <===== provide the elapsed time yourself
}
```

The full <code>sleep_benchmark.cpp</code> source code can be seen [here](src/main/c/sleep_benchmark.cpp)

</details>

<br/>

## More examples
- [MathBenchmark.java](src/main/java/com/coralblocks/coralbench/example/MathBenchmark.java)
- [BubbleSortBenchmark.java](src/main/java/com/coralblocks/coralbench/example/BubbleSortBenchmark.java)
- [SleepBenchmark.java](src/main/java/com/coralblocks/coralbench/example/SleepBenchmark.java)

<br/>
<details>
  <summary>&nbsp;&nbsp;<img src="https://cdn3.emoji.gg/emojis/8241-c-plus-plus.png" width="24px" height="24px" alt="c_plus_plus"/>&nbsp;&nbsp;Click here for the C++ examples </summary>
    
- [math_benchmark.cpp](src/main/c/math_benchmark.cpp)
- [bubble_sort_benchmark.cpp](src/main/c/bubble_sort_benchmark.cpp)
- [sleep_benchmark.cpp](src/main/c/sleep_benchmark.cpp)
    
</details>
<br/>

## HotSpotVM (-Xcomp and JIT) vs C++ LLVM (clang) vs GraalVM

In this section, we compare the `HotSpotVM` JIT (JVMCI and C2) against three forms of AOT: `C++ LLVM` (clang), `GraalVM` (native-image), and `-Xcomp` (JVMCI and C2). To ensure an unbiased comparison, all Java and C++ source code is designed to be _equivalent_ **without compromising idiomatic C++ practices for performance**. This includes the `Bench` class, which performs the benchmark measurements. Unlike the C++ version, the Java code pools the `IntMap.Entry` objects to avoid creating garbage for the GC. The next step will be to analyze the assembly code generated by the JIT and compare it with the AOT. `If you notice anything incorrect or see areas for improvement, especially in the C++ code, please feel free to open an issue or submit a pull request.`

The Java `IntMap` implementation [is here](src/main/java/com/coralblocks/coralbench/example/IntMap.java). The C++ implementation [is here](src/main/c/int_map.hpp).<br/>
The Java benchmark code for the `IntMap` [is here](src/main/java/com/coralblocks/coralbench/example/IntMapBenchmark.java). The C++ implementation [is here](src/main/c/int_map_benchmark.cpp).<br/>
The Java `Bench` class [is here](src/main/java/com/coralblocks/coralbench/Bench.java). The C++ implementation [is here](src/main/c/bench.cpp).<br/>

<details>
  <summary> Benchmark Environment </summary>

<br/>

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
</details>

<details>
  <summary> HotSpotVM JIT (with Graal JVMCI JIT)</summary>

<br/>

```
$ java -XX:+AlwaysPreTouch -Xms4g -Xmx4g -XX:NewSize=512m -XX:MaxNewSize=1024m \
       -cp target/classes:target/coralbench-all.jar \
       com.coralblocks.coralbench.example.IntMapBenchmark 0 3000000 1000000

Arguments: warmup=0 measurements=3000000 mapCapacity=1000000

Benchmarking put...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 28.040 nanos | Min Time: 17.000 nanos | Max Time: 211.653 micros
75% = [avg: 23.000 nanos, max: 30.000 nanos]
90% = [avg: 24.000 nanos, max: 31.000 nanos]
99% = [avg: 26.000 nanos, max: 91.000 nanos]
99.9% = [avg: 27.000 nanos, max: 197.000 nanos]
99.99% = [avg: 27.000 nanos, max: 440.000 nanos]
99.999% = [avg: 27.000 nanos, max: 4.900 micros]

Benchmarking get...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 23.270 nanos | Min Time: 13.000 nanos | Max Time: 25.433 micros
75% = [avg: 18.000 nanos, max: 29.000 nanos]
90% = [avg: 20.000 nanos, max: 31.000 nanos]
99% = [avg: 22.000 nanos, max: 85.000 nanos]
99.9% = [avg: 22.000 nanos, max: 132.000 nanos]
99.99% = [avg: 23.000 nanos, max: 401.000 nanos]
99.999% = [avg: 23.000 nanos, max: 4.384 micros]

Benchmarking remove...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 29.450 nanos | Min Time: 19.000 nanos | Max Time: 36.736 micros
75% = [avg: 24.000 nanos, max: 35.000 nanos]
90% = [avg: 26.000 nanos, max: 36.000 nanos]
99% = [avg: 28.000 nanos, max: 95.000 nanos]
99.9% = [avg: 28.000 nanos, max: 222.000 nanos]
99.99% = [avg: 29.000 nanos, max: 437.000 nanos]
99.999% = [avg: 29.000 nanos, max: 13.447 micros]
```
</details>

```
PUT => Avg: 28 ns | Min: 17 ns | 99.9% = [avg: 27 ns, max: 197 ns]
GET => Avg: 23 ns | Min: 13 ns | 99.9% = [avg: 22 ns, max: 132 ns]
DEL => Avg: 29 ns | Min: 19 ns | 99.9% = [avg: 28 ns, max: 222 ns]
```

<details>
  <summary> C++ LLVM (clang) </summary>

<br/>

```
$ clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/int_map.cpp -o ./target/cpp/int_map.o
$ clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/bench.cpp -o ./target/cpp/bench.o
$ clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/int_map_benchmark.cpp -o ./target/cpp/int_map_benchmark.o
$ clang++ -Ofast -march=native -flto -std=c++17 -o ./target/cpp/int_map_benchmark ./target/cpp/int_map.o ./target/cpp/bench.o ./target/cpp/int_map_benchmark.o

$ ./target/cpp/int_map_benchmark 0 2000000 20000

Arguments: warmup=0 measurements=3000000 mapCapacity=1000000

Benchmarking put...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 47.750 nanos | Min Time: 34.000 nanos | Max Time: 18.413 micros
75% = [avg: 39.011 nanos, max: 42.000 nanos]
90% = [avg: 39.577 nanos, max: 43.000 nanos]
99% = [avg: 40.832 nanos, max: 116.000 nanos]
99.9% = [avg: 45.095 nanos, max: 1.040 micros]
99.99% = [avg: 46.853 nanos, max: 5.286 micros]
99.999% = [avg: 47.590 nanos, max: 15.470 micros]

Benchmarking get...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 25.980 nanos | Min Time: 20.000 nanos | Max Time: 15.910 micros
75% = [avg: 22.992 nanos, max: 25.000 nanos]
90% = [avg: 23.350 nanos, max: 26.000 nanos]
99% = [avg: 24.752 nanos, max: 94.000 nanos]
99.9% = [avg: 25.473 nanos, max: 213.000 nanos]
99.99% = [avg: 25.727 nanos, max: 429.000 nanos]
99.999% = [avg: 25.832 nanos, max: 13.545 micros]

Benchmarking remove...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 25.930 nanos | Min Time: 22.000 nanos | Max Time: 4.289 micros
75% = [avg: 23.580 nanos, max: 24.000 nanos]
90% = [avg: 23.674 nanos, max: 25.000 nanos]
99% = [avg: 24.942 nanos, max: 95.000 nanos]
99.9% = [avg: 25.655 nanos, max: 171.000 nanos]
99.99% = [avg: 25.875 nanos, max: 421.000 nanos]
99.999% = [avg: 25.912 nanos, max: 490.000 nanos]
```
</details>

```
PUT => Avg: 48 ns | Min: 34 ns | 99.9% = [avg: 45 ns, max: 1040 ns]
GET => Avg: 26 ns | Min: 20 ns | 99.9% = [avg: 25 ns, max: 213 ns]
DEL => Avg: 26 ns | Min: 22 ns | 99.9% = [avg: 26 ns, max: 171 ns]
```

<details>
  <summary> GraalVM (native-image) </summary>

<br/>

```
$ native-image --gc=G1 -R:+AlwaysPreTouch -R:InitialHeapSize=4g -R:MaxHeapSize=4g \
               -R:InitialHeapSize=512m -R:MaxHeapSize=1024m -march=native \
               -cp target/coralbench-all.jar com.coralblocks.coralbench.example.IntMapBenchmark \
               -o target/graal/IntMapBenchmark --no-fallback -O3 --initialize-at-build-time

$ ./target/graal/IntMapBenchmark 0 3000000 1000000

Arguments: warmup=0 measurements=3000000 mapCapacity=1000000

Benchmarking put...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 59.850 nanos | Min Time: 27.000 nanos | Max Time: 53.666 millis
75% = [avg: 32.000 nanos, max: 34.000 nanos]
90% = [avg: 32.000 nanos, max: 35.000 nanos]
99% = [avg: 33.000 nanos, max: 99.000 nanos]
99.9% = [avg: 34.000 nanos, max: 230.000 nanos]
99.99% = [avg: 34.000 nanos, max: 776.000 nanos]
99.999% = [avg: 34.000 nanos, max: 80.388 micros]

Benchmarking get...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 27.700 nanos | Min Time: 22.000 nanos | Max Time: 1.902 micros
75% = [avg: 25.000 nanos, max: 26.000 nanos]
90% = [avg: 25.000 nanos, max: 27.000 nanos]
99% = [avg: 26.000 nanos, max: 92.000 nanos]
99.9% = [avg: 27.000 nanos, max: 218.000 nanos]
99.99% = [avg: 27.000 nanos, max: 438.000 nanos]
99.999% = [avg: 27.000 nanos, max: 526.000 nanos]

Benchmarking remove...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 50.460 nanos | Min Time: 40.000 nanos | Max Time: 14.746 micros
75% = [avg: 46.000 nanos, max: 51.000 nanos]
90% = [avg: 47.000 nanos, max: 53.000 nanos]
99% = [avg: 49.000 nanos, max: 116.000 nanos]
99.9% = [avg: 50.000 nanos, max: 260.000 nanos]
99.99% = [avg: 50.000 nanos, max: 526.000 nanos]
99.999% = [avg: 50.000 nanos, max: 1.871 micros]
```
</details>

```
PUT => Avg: 60 ns | Min: 27 ns | 99.9% = [avg: 34 ns, max: 230 ns]
GET => Avg: 28 ns | Min: 22 ns | 99.9% = [avg: 27 ns, max: 218 ns]
DEL => Avg: 50 ns | Min: 40 ns | 99.9% = [avg: 50 ns, max: 260 ns]
```

<details>
  <summary> HotSpotVM JIT (with C2 JIT)</summary>

<br/>

```
$ java -XX:-UseJVMCICompiler -XX:+AlwaysPreTouch -Xms4g -Xmx4g -XX:NewSize=512m -XX:MaxNewSize=1024m \
       -cp target/classes:target/coralbench-all.jar \
       com.coralblocks.coralbench.example.IntMapBenchmark 0 3000000 1000000

Arguments: warmup=0 measurements=3000000 mapCapacity=1000000

Benchmarking put...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 32.400 nanos | Min Time: 24.000 nanos | Max Time: 197.576 micros
75% = [avg: 28.000 nanos, max: 31.000 nanos]
90% = [avg: 29.000 nanos, max: 33.000 nanos]
99% = [avg: 31.000 nanos, max: 95.000 nanos]
99.9% = [avg: 31.000 nanos, max: 236.000 nanos]
99.99% = [avg: 32.000 nanos, max: 438.000 nanos]
99.999% = [avg: 32.000 nanos, max: 13.616 micros]

Benchmarking get...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 29.130 nanos | Min Time: 21.000 nanos | Max Time: 25.899 micros
75% = [avg: 25.000 nanos, max: 28.000 nanos]
90% = [avg: 26.000 nanos, max: 30.000 nanos]
99% = [avg: 27.000 nanos, max: 94.000 nanos]
99.9% = [avg: 28.000 nanos, max: 202.000 nanos]
99.99% = [avg: 28.000 nanos, max: 438.000 nanos]
99.999% = [avg: 28.000 nanos, max: 4.932 micros]

Benchmarking remove...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 37.730 nanos | Min Time: 29.000 nanos | Max Time: 27.074 micros
75% = [avg: 33.000 nanos, max: 35.000 nanos]
90% = [avg: 33.000 nanos, max: 44.000 nanos]
99% = [avg: 36.000 nanos, max: 101.000 nanos]
99.9% = [avg: 36.000 nanos, max: 289.000 nanos]
99.99% = [avg: 37.000 nanos, max: 450.000 nanos]
99.999% = [avg: 37.000 nanos, max: 15.342 micros]
```
</details>

```
PUT => Avg: 32 ns | Min: 24 ns | 99.9% = [avg: 31 ns, max: 236 ns]
GET => Avg: 29 ns | Min: 21 ns | 99.9% = [avg: 28 ns, max: 202 ns]
DEL => Avg: 38 ns | Min: 29 ns | 99.9% = [avg: 36 ns, max: 289 ns]
```

<details>
  <summary> HotSpotVM -Xcomp (with Graal JVMCI JIT)</summary>

<br/>

```
$ java -Xcomp -XX:-TieredCompilation \
       -XX:+AlwaysPreTouch -Xms4g -Xmx4g -XX:NewSize=512m -XX:MaxNewSize=1024m \
       -cp target/classes:target/coralbench-all.jar \
       com.coralblocks.coralbench.example.IntMapBenchmark 0 3000000 1000000

Arguments: warmup=0 measurements=3000000 mapCapacity=1000000

Benchmarking put...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 36.720 nanos | Min Time: 28.000 nanos | Max Time: 3.691 millis
75% = [avg: 31.000 nanos, max: 33.000 nanos]
90% = [avg: 31.000 nanos, max: 35.000 nanos]
99% = [avg: 33.000 nanos, max: 99.000 nanos]
99.9% = [avg: 33.000 nanos, max: 160.000 nanos]
99.99% = [avg: 34.000 nanos, max: 437.000 nanos]
99.999% = [avg: 34.000 nanos, max: 1.601 micros]

Benchmarking get...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 31.940 nanos | Min Time: 23.000 nanos | Max Time: 2.040 millis
75% = [avg: 28.000 nanos, max: 30.000 nanos]
90% = [avg: 28.000 nanos, max: 32.000 nanos]
99% = [avg: 30.000 nanos, max: 95.000 nanos]
99.9% = [avg: 30.000 nanos, max: 158.000 nanos]
99.99% = [avg: 31.000 nanos, max: 433.000 nanos]
99.999% = [avg: 31.000 nanos, max: 13.363 micros]

Benchmarking remove...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 42.730 nanos | Min Time: 34.000 nanos | Max Time: 5.093 millis
75% = [avg: 37.000 nanos, max: 40.000 nanos]
90% = [avg: 38.000 nanos, max: 42.000 nanos]
99% = [avg: 39.000 nanos, max: 105.000 nanos]
99.9% = [avg: 40.000 nanos, max: 209.000 nanos]
99.99% = [avg: 40.000 nanos, max: 444.000 nanos]
99.999% = [avg: 40.000 nanos, max: 13.936 micros]
```
</details>

```
PUT => Avg: 37 ns | Min: 28 ns | 99.9% = [avg: 33 ns, max: 160 ns]
GET => Avg: 32 ns | Min: 23 ns | 99.9% = [avg: 30 ns, max: 158 ns]
DEL => Avg: 43 ns | Min: 34 ns | 99.9% = [avg: 40 ns, max: 209 ns]
```

<details>
  <summary> HotSpotVM -Xcomp (with C2 JIT)</summary>

<br/>

```
$ java -XX:-UseJVMCICompiler -Xcomp -XX:-TieredCompilation \
       -XX:+AlwaysPreTouch -Xms4g -Xmx4g -XX:NewSize=512m -XX:MaxNewSize=1024m \
       -cp target/classes:target/coralbench-all.jar \
       com.coralblocks.coralbench.example.IntMapBenchmark 0 3000000 1000000

Arguments: warmup=0 measurements=3000000 mapCapacity=1000000

Benchmarking put...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 39.850 nanos | Min Time: 29.000 nanos | Max Time: 4.653 millis
75% = [avg: 34.000 nanos, max: 37.000 nanos]
90% = [avg: 34.000 nanos, max: 39.000 nanos]
99% = [avg: 36.000 nanos, max: 101.000 nanos]
99.9% = [avg: 36.000 nanos, max: 164.000 nanos]
99.99% = [avg: 37.000 nanos, max: 439.000 nanos]
99.999% = [avg: 37.000 nanos, max: 1.604 micros]

Benchmarking get...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 33.600 nanos | Min Time: 25.000 nanos | Max Time: 2.449 millis
75% = [avg: 29.000 nanos, max: 32.000 nanos]
90% = [avg: 30.000 nanos, max: 33.000 nanos]
99% = [avg: 31.000 nanos, max: 96.000 nanos]
99.9% = [avg: 32.000 nanos, max: 162.000 nanos]
99.99% = [avg: 32.000 nanos, max: 430.000 nanos]
99.999% = [avg: 32.000 nanos, max: 1.448 micros]

Benchmarking remove...
Measurements: 3,000,000 | Warm-Up: 0 | Iterations: 3,000,000
Avg Time: 46.030 nanos | Min Time: 36.000 nanos | Max Time: 6.155 millis
75% = [avg: 40.000 nanos, max: 43.000 nanos]
90% = [avg: 41.000 nanos, max: 45.000 nanos]
99% = [avg: 42.000 nanos, max: 106.000 nanos]
99.9% = [avg: 43.000 nanos, max: 212.000 nanos]
99.99% = [avg: 43.000 nanos, max: 443.000 nanos]
99.999% = [avg: 43.000 nanos, max: 15.320 micros]
```
</details>

```
PUT => Avg: 40 ns | Min: 29 ns | 99.9% = [avg: 36 ns, max: 164 ns]
GET => Avg: 34 ns | Min: 25 ns | 99.9% = [avg: 32 ns, max: 162 ns]
DEL => Avg: 46 ns | Min: 36 ns | 99.9% = [avg: 43 ns, max: 212 ns]
```

<br/>

## Forcing the JVM to optimize at startup (-Xcomp -XX:-TieredCompilation)

By default, the HotSpot JVM will take some time to profile a running Java application for hot spots in the code and then optimize by compiling (to assembly) and inlining these hot spot methods. That's great because the HotSpot JVM can surgically and aggressively optimize the parts of your program that matter the most instead of taking the AoT (ahead-of-time) approach of compiling and trying to optimize the whole thing beforehand. `For example, method inlining is an aggressive form of optimization that usually requires runtime profiling information since inlining everything is impractical/impossible.`

The HotSpot JVM offers an interesting startup option `-Xcomp` to compile every method right before its first invocation. The drawback is that without any profiling this optimization is conservative. `For example, even though some basic method inlining is still performed, a more aggressive inlining approach does not happen.` The advantage is that your application will be able to perform at a native/assembly level <strong>right when it starts</strong>, without having to wait until the HotSpot JVM have gathered enough profiling to compile and optimize the hot methods.

Below we explore some of our benchmark examples to better understand the difference in performance that these approaches can make.

<details>
  <summary>Benchmark Environment</summary>

<br/>
    
```
$ uname -a
Linux hivelocity 4.15.0-20-generic #21-Ubuntu SMP Tue Apr 24 06:16:15 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux

$ cat /etc/issue | head -n 1
Ubuntu 18.04.6 LTS \n \l

$ cat /proc/cpuinfo | grep "model name" | head -n 1 | awk -F ": " '{print $NF}'
Intel(R) Xeon(R) E-2288G CPU @ 3.70GHz

$ arch
x86_64

$ java -version
java version "23.0.1" 2024-10-15
Java(TM) SE Runtime Environment (build 23.0.1+11-39)
Java HotSpot(TM) 64-Bit Server VM (build 23.0.1+11-39, mixed mode, sharing)
```
</details>

<details>
  <summary>MathBenchmark Numbers</summary>
    
#### Regular JIT <i>with</i> warm-up
```
$ java -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark 1000000 1000
Value computed: -55054840000
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 223.720 nanos | Min Time: 200.000 nanos | Max Time: 3.004 micros
75% = [avg: 207.000 nanos, max: 214.000 nanos]
90% = [avg: 209.000 nanos, max: 219.000 nanos]
99% = [avg: 218.000 nanos, max: 341.000 nanos]
99.9% = [avg: 220.000 nanos, max: 530.000 nanos]
99.99% = [avg: 223.000 nanos, max: 3.004 micros]
99.999% = [avg: 223.000 nanos, max: 3.004 micros]
```
#### Regular JIT <i>without</i> warm-up
```
$ java -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark 5 1000
Value computed: -54580000
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 4.341 micros | Min Time: 203.000 nanos | Max Time: 236.608 micros
75% = [avg: 211.000 nanos, max: 217.000 nanos]
90% = [avg: 212.000 nanos, max: 225.000 nanos]
99% = [avg: 2.993 micros, max: 78.217 micros]
99.9% = [avg: 4.108 micros, max: 193.372 micros]
99.99% = [avg: 4.341 micros, max: 236.608 micros]
99.999% = [avg: 4.341 micros, max: 236.608 micros]
```
#### -Xcomp -XX:-TieredCompilation <i>with</i> warm-up
```
$ java -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark 1000000 1000
Value computed: -55054840000
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 92.680 nanos | Min Time: 90.000 nanos | Max Time: 166.000 nanos
75% = [avg: 92.000 nanos, max: 93.000 nanos]
90% = [avg: 92.000 nanos, max: 93.000 nanos]
99% = [avg: 92.000 nanos, max: 96.000 nanos]
99.9% = [avg: 92.000 nanos, max: 152.000 nanos]
99.99% = [avg: 92.000 nanos, max: 166.000 nanos]
99.999% = [avg: 92.000 nanos, max: 166.000 nanos]
```
#### -Xcomp -XX:-TieredCompilation <i>without</i> warm-up
```
$ java -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark 5 1000
Value computed: -54580000
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 119.300 nanos | Min Time: 115.000 nanos | Max Time: 436.000 nanos
75% = [avg: 117.000 nanos, max: 118.000 nanos]
90% = [avg: 117.000 nanos, max: 119.000 nanos]
99% = [avg: 118.000 nanos, max: 130.000 nanos]
99.9% = [avg: 118.000 nanos, max: 415.000 nanos]
99.99% = [avg: 119.000 nanos, max: 436.000 nanos]
99.999% = [avg: 119.000 nanos, max: 436.000 nanos]
```
</details>
<details>
  <summary>MathBenchmark Numbers with new Graal JIT compiler (JVMCI)</summary>
    
#### Regular JIT <i>with</i> warm-up
```
$ java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark 1000000 1000
Value computed: -55054840000
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 88.750 nanos | Min Time: 16.000 nanos | Max Time: 4.586 micros
75% = [avg: 73.000 nanos, max: 94.000 nanos]
90% = [avg: 77.000 nanos, max: 102.000 nanos]
99% = [avg: 80.000 nanos, max: 155.000 nanos]
99.9% = [avg: 84.000 nanos, max: 1.676 micros]
99.99% = [avg: 88.000 nanos, max: 4.586 micros]
99.999% = [avg: 88.000 nanos, max: 4.586 micros]
```
#### Regular JIT <i>without</i> warm-up
```
$ java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark 5 1000
Value computed: -54580000
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 3.683 micros | Min Time: 63.000 nanos | Max Time: 209.935 micros
75% = [avg: 68.000 nanos, max: 72.000 nanos]
90% = [avg: 69.000 nanos, max: 79.000 nanos]
99% = [avg: 2.540 micros, max: 78.269 micros]
99.9% = [avg: 3.476 micros, max: 186.890 micros]
99.99% = [avg: 3.682 micros, max: 209.935 micros]
99.999% = [avg: 3.682 micros, max: 209.935 micros]
```
#### -Xcomp -XX:-TieredCompilation <i>with</i> warm-up
```
$ java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark 1000000 1000
Value computed: -55054840000
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 17.050 nanos | Min Time: 16.000 nanos | Max Time: 62.000 nanos
75% = [avg: 16.000 nanos, max: 17.000 nanos]
90% = [avg: 16.000 nanos, max: 18.000 nanos]
99% = [avg: 16.000 nanos, max: 21.000 nanos]
99.9% = [avg: 17.000 nanos, max: 54.000 nanos]
99.99% = [avg: 17.000 nanos, max: 62.000 nanos]
99.999% = [avg: 17.000 nanos, max: 62.000 nanos]
```
#### -Xcomp -XX:-TieredCompilation <i>without</i> warm-up
```
$ java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark 5 1000
Value computed: -54580000
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 45.000 nanos | Min Time: 42.000 nanos | Max Time: 354.000 nanos
75% = [avg: 43.000 nanos, max: 45.000 nanos]
90% = [avg: 44.000 nanos, max: 45.000 nanos]
99% = [avg: 44.000 nanos, max: 51.000 nanos]
99.9% = [avg: 44.000 nanos, max: 351.000 nanos]
99.99% = [avg: 45.000 nanos, max: 354.000 nanos]
99.999% = [avg: 45.000 nanos, max: 354.000 nanos]
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
  <summary>BubbleSortBenchmark Numbers with new Graal JIT compiler (JVMCI)</summary>
    
#### Regular JIT <i>with</i> warm-up
```
$ java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark 1000000 1000
Value computed: 1831830000
Array: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60]
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 1.092 micros | Min Time: 970.000 nanos | Max Time: 19.336 micros
75% = [avg: 1.002 micros, max: 1.034 micros]
90% = [avg: 1.011 micros, max: 1.069 micros]
99% = [avg: 1.018 micros, max: 1.715 micros]
99.9% = [avg: 1.073 micros, max: 16.361 micros]
99.99% = [avg: 1.092 micros, max: 19.336 micros]
99.999% = [avg: 1.092 micros, max: 19.336 micros]
```
#### Regular JIT <i>without</i> warm-up
```
$ java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark 5 1000
Value computed: 1839150
Array: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60]
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 11.897 micros | Min Time: 8.262 micros | Max Time: 95.115 micros
75% = [avg: 8.363 micros, max: 8.550 micros]
90% = [avg: 8.673 micros, max: 16.875 micros]
99% = [avg: 11.372 micros, max: 54.561 micros]
99.9% = [avg: 11.814 micros, max: 90.020 micros]
99.99% = [avg: 11.897 micros, max: 95.115 micros]
99.999% = [avg: 11.897 micros, max: 95.115 micros]
```
#### -Xcomp -XX:-TieredCompilation <i>with</i> warm-up
```
$ java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark 1000000 1000
Value computed: 1831830000
Array: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60]
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 1.872 micros | Min Time: 1.814 micros | Max Time: 8.732 micros
75% = [avg: 1.858 micros, max: 1.871 micros]
90% = [avg: 1.861 micros, max: 1.877 micros]
99% = [avg: 1.863 micros, max: 1.889 micros]
99.9% = [avg: 1.864 micros, max: 2.673 micros]
99.99% = [avg: 1.871 micros, max: 8.732 micros]
99.999% = [avg: 1.871 micros, max: 8.732 micros]
```
#### -Xcomp -XX:-TieredCompilation <i>without</i> warm-up
```
$ java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark 5 1000
Value computed: 1839150
Array: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60]
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 1.816 micros | Min Time: 1.775 micros | Max Time: 7.835 micros
75% = [avg: 1.802 micros, max: 1.817 micros]
90% = [avg: 1.805 micros, max: 1.825 micros]
99% = [avg: 1.808 micros, max: 1.858 micros]
99.9% = [avg: 1.809 micros, max: 2.174 micros]
99.99% = [avg: 1.815 micros, max: 7.835 micros]
99.999% = [avg: 1.815 micros, max: 7.835 micros]
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
<details>
  <summary>SleepBenchmark Numbers with new Graal JIT compiler (JVMCI)</summary>
    
#### Regular JIT <i>with</i> warm-up
```
$ java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -cp target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark 1000000 1000
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 1.130 micros | Min Time: 1.039 micros | Max Time: 19.635 micros
75% = [avg: 1.051 micros, max: 1.062 micros]
90% = [avg: 1.053 micros, max: 1.083 micros]
99% = [avg: 1.059 micros, max: 1.405 micros]
99.9% = [avg: 1.111 micros, max: 15.164 micros]
99.99% = [avg: 1.129 micros, max: 19.635 micros]
99.999% = [avg: 1.129 micros, max: 19.635 micros]
```
#### Regular JIT <i>without</i> warm-up
```
$ java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -cp target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark 5 1000
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 1.213 micros | Min Time: 1.041 micros | Max Time: 21.365 micros
75% = [avg: 1.056 micros, max: 1.105 micros]
90% = [avg: 1.068 micros, max: 1.146 micros]
99% = [avg: 1.080 micros, max: 1.923 micros]
99.9% = [avg: 1.192 micros, max: 20.685 micros]
99.99% = [avg: 1.212 micros, max: 21.365 micros]
99.999% = [avg: 1.212 micros, max: 21.365 micros]
```
#### -Xcomp -XX:-TieredCompilation <i>with</i> warm-up
```
$ java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark 1000000 1000
Measurements: 1,000 | Warm-Up: 1,000,000 | Iterations: 1,001,000
Avg Time: 1.047 micros | Min Time: 1.035 micros | Max Time: 1.122 micros
75% = [avg: 1.045 micros, max: 1.048 micros]
90% = [avg: 1.046 micros, max: 1.049 micros]
99% = [avg: 1.046 micros, max: 1.050 micros]
99.9% = [avg: 1.046 micros, max: 1.083 micros]
99.99% = [avg: 1.046 micros, max: 1.122 micros]
99.999% = [avg: 1.046 micros, max: 1.122 micros]
```
#### -Xcomp -XX:-TieredCompilation <i>without</i> warm-up
```
$ java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -Xcomp -XX:-TieredCompilation -cp target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark 5 1000
Measurements: 1,000 | Warm-Up: 5 | Iterations: 1,005
Avg Time: 1.044 micros | Min Time: 1.036 micros | Max Time: 4.846 micros
75% = [avg: 1.039 micros, max: 1.041 micros]
90% = [avg: 1.039 micros, max: 1.042 micros]
99% = [avg: 1.039 micros, max: 1.047 micros]
99.9% = [avg: 1.039 micros, max: 1.075 micros]
99.99% = [avg: 1.043 micros, max: 4.846 micros]
99.999% = [avg: 1.043 micros, max: 4.846 micros]
```
</details>

As you can see from the latency numbers above, by using `-Xcomp -XX:-TieredCompilation` you may be able to `eliminate the need for your application to warm up by paying a small price in performance`. Of course this conclusion cannot be generalized for every application as it will depend heavily on the characteristics and particularities of the source code and its critical path. `But it is worth giving -Xcomp -XX:-TieredCompilation a try to see what kind of numbers you get and to evaluate if the trade-off is worth it.`

##### For how this applies to [CoralSequencer](https://www.coralblocks.com/coralsequencer) you can check [this article](https://www.coralblocks.com/index.php/hotspot-jit-aot-and-warm-up/).
