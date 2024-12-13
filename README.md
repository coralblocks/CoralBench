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

In this section, we compare the `HotSpotVM` JIT (JVMCI and C2) against three forms of AOT: `C++ LLVM` (clang), `GraalVM` (native-image), and `-Xcomp` (JVMCI and C2). To ensure a fair and unbiased comparison, all Java and C++ source code is designed to be _equivalent_. Not only the `IntMap` class, which is the code being measured, but also the `Bench` class, which performs the measurements. `We made every effort to maintain consistency and fairness in the comparison.` **If you notice anything that seems incorrect or could be improved, especially in the C++ code, please feel free to open an issue or submit a pull request.**

The Java `IntMap` implementation [is here](src/main/java/com/coralblocks/coralbench/example/IntMap.java). The C++ `IntMap` implementation [is here](src/main/c/int_map.hpp).<br/>
The Java benchmark code [is here](src/main/java/com/coralblocks/coralbench/example/IntMapBenchmark.java). The C++ benchmark code [is here](src/main/c/int_map_benchmark.cpp).<br/>
The Java `Bench` class [is here](src/main/java/com/coralblocks/coralbench/Bench.java). The C++ `Bench` class [is here](src/main/c/bench.cpp).<br/>

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
       com.coralblocks.coralbench.example.IntMapBenchmark 0 2000000 20000

Arguments: warmup=0 measurements=2000000 mapCapacity=20000

Benchmarking put on empty map... (1) => creating new Entry objects
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 371.140 nanos | Min Time: 28.000 nanos | Max Time: 35.143 micros
75% = [avg: 189.000 nanos, max: 611.000 nanos]
90% = [avg: 283.000 nanos, max: 917.000 nanos]
99% = [avg: 356.000 nanos, max: 1.400 micros]
99.9% = [avg: 367.000 nanos, max: 1.743 micros]
99.99% = [avg: 369.000 nanos, max: 14.847 micros]
99.999% = [avg: 370.000 nanos, max: 18.452 micros]

Benchmarking put after clear()... (2) => hitting the pool of Entry objects
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 612.590 nanos | Min Time: 27.000 nanos | Max Time: 25.693 micros
75% = [avg: 427.000 nanos, max: 908.000 nanos]
90% = [avg: 525.000 nanos, max: 1.146 micros]
99% = [avg: 596.000 nanos, max: 1.622 micros]
99.9% = [avg: 606.000 nanos, max: 2.184 micros]
99.99% = [avg: 610.000 nanos, max: 16.229 micros]
99.999% = [avg: 612.000 nanos, max: 18.765 micros]

Benchmarking get...
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 614.570 nanos | Min Time: 14.000 nanos | Max Time: 47.144 micros
75% = [avg: 426.000 nanos, max: 905.000 nanos]
90% = [avg: 524.000 nanos, max: 1.142 micros]
99% = [avg: 595.000 nanos, max: 1.670 micros]
99.9% = [avg: 607.000 nanos, max: 2.549 micros]
99.99% = [avg: 612.000 nanos, max: 16.254 micros]
99.999% = [avg: 614.000 nanos, max: 19.005 micros]

Benchmarking remove...
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 662.350 nanos | Min Time: 18.000 nanos | Max Time: 65.518 micros
75% = [avg: 460.000 nanos, max: 982.000 nanos]
90% = [avg: 567.000 nanos, max: 1.254 micros]
99% = [avg: 646.000 nanos, max: 1.808 micros]
99.9% = [avg: 658.000 nanos, max: 2.538 micros]
99.99% = [avg: 660.000 nanos, max: 6.448 micros]
99.999% = [avg: 662.000 nanos, max: 23.845 micros]
```
</details>

```
PUT => Avg: 371 ns | Min: 28 ns | 99.9% = [avg: 367 ns, max: 1.743 micros]
PUT => Avg: 613 ns | Min: 27 ns | 99.9% = [avg: 606 ns, max: 2.184 micros]
GET => Avg: 615 ns | Min: 14 ns | 99.9% = [avg: 607 ns, max: 2.549 micros]
DEL => Avg: 662 ns | Min: 18 ns | 99.9% = [avg: 658 ns, max: 2.538 micros]
```

<details>
  <summary> C++ LLVM (clang) </summary>

<br/>

```
$ clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/int_map.cpp -o ./target/cpp/int_map.o
$ clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/bench.cpp -o ./target/cpp/bench.o
$ clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/int_map_benchmark.cpp -o ./target/cpp/int_map_benchmark.o
$ clang++ -Ofast -march=native -flto -std=c++17 -o ./target/cpp/int_map_benchmark ./target/cpp/int_map.o ./target/cpp/bench.o ./target/cpp/int_map_benchmark.o

$ ./target/cpp/int_map_benchmark 0 10000000 5000000

Arguments: warmup=0 measurements=2000000 mapCapacity=20000

Benchmarking put on empty map... (1) => creating new Entry objects
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 725.830 nanos | Min Time: 30.000 nanos | Max Time: 32.898 micros
75% = [avg: 213.945 nanos, max: 827.000 nanos]
90% = [avg: 462.161 nanos, max: 2.558 micros]
99% = [avg: 692.651 nanos, max: 3.568 micros]
99.9% = [avg: 720.035 nanos, max: 4.097 micros]
99.99% = [avg: 723.810 nanos, max: 16.642 micros]
99.999% = [avg: 725.549 nanos, max: 27.622 micros]

Benchmarking put after clear()... (2) => hitting the pool of Entry objects
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 856.870 nanos | Min Time: 18.000 nanos | Max Time: 34.051 micros
75% = [avg: 621.592 nanos, max: 1.270 micros]
90% = [avg: 751.984 nanos, max: 1.542 micros]
99% = [avg: 836.167 nanos, max: 1.964 micros]
99.9% = [avg: 848.394 nanos, max: 2.933 micros]
99.99% = [avg: 855.174 nanos, max: 16.988 micros]
99.999% = [avg: 856.656 nanos, max: 19.138 micros]

Benchmarking get...
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 873.800 nanos | Min Time: 18.000 nanos | Max Time: 30.037 micros
75% = [avg: 636.786 nanos, max: 1.294 micros]
90% = [avg: 768.257 nanos, max: 1.560 micros]
99% = [avg: 852.247 nanos, max: 1.987 micros]
99.9% = [avg: 864.806 nanos, max: 3.010 micros]
99.99% = [avg: 872.055 nanos, max: 17.053 micros]
99.999% = [avg: 873.564 nanos, max: 20.451 micros]

Benchmarking remove...
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 874.940 nanos | Min Time: 19.000 nanos | Max Time: 29.175 micros
75% = [avg: 643.087 nanos, max: 1.304 micros]
90% = [avg: 775.314 nanos, max: 1.569 micros]
99% = [avg: 858.983 nanos, max: 1.979 micros]
99.9% = [avg: 871.128 nanos, max: 2.810 micros]
99.99% = [avg: 873.556 nanos, max: 5.865 micros]
99.999% = [avg: 874.707 nanos, max: 21.086 micros]
```
</details>

```
PUT => Avg: 726 ns | Min: 30 ns | 99.9% = [avg: 720 ns, max: 4.097 micros]
PUT => Avg: 857 ns | Min: 18 ns | 99.9% = [avg: 848 ns, max: 2.933 micros]
GET => Avg: 874 ns | Min: 18 ns | 99.9% = [avg: 865 ns, max: 3.010 micros]
DEL => Avg: 875 ns | Min: 19 ns | 99.9% = [avg: 871 ns, max: 2.810 micros]
```

<details>
  <summary> GraalVM (native-image) </summary>

<br/>

```
$ native-image --gc=G1 -R:+AlwaysPreTouch -R:InitialHeapSize=4g -R:MaxHeapSize=4g \
               -R:InitialHeapSize=512m -R:MaxHeapSize=1024m -march=native \
               -cp target/coralbench-all.jar com.coralblocks.coralbench.example.IntMapBenchmark \
               -o target/graal/IntMapBenchmark --no-fallback -O3 --initialize-at-build-time

$ ./target/graal/IntMapBenchmark 0 10000000 5000000

Arguments: warmup=0 measurements=2000000 mapCapacity=20000

Benchmarking put on empty map... (1) => creating new Entry objects
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 190.000 nanos | Min Time: 21.000 nanos | Max Time: 9.728 millis
75% = [avg: 121.000 nanos, max: 267.000 nanos]
90% = [avg: 152.000 nanos, max: 368.000 nanos]
99% = [avg: 179.000 nanos, max: 610.000 nanos]
99.9% = [avg: 183.000 nanos, max: 814.000 nanos]
99.99% = [avg: 184.000 nanos, max: 1.098 micros]
99.999% = [avg: 184.000 nanos, max: 15.573 micros]

Benchmarking put after clear()... (2) => hitting the pool of Entry objects
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 658.830 nanos | Min Time: 23.000 nanos | Max Time: 20.209 micros
75% = [avg: 390.000 nanos, max: 1.027 micros]
90% = [avg: 529.000 nanos, max: 1.485 micros]
99% = [avg: 640.000 nanos, max: 2.201 micros]
99.9% = [avg: 656.000 nanos, max: 2.762 micros]
99.99% = [avg: 658.000 nanos, max: 4.202 micros]
99.999% = [avg: 658.000 nanos, max: 6.371 micros]

Benchmarking get...
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 398.720 nanos | Min Time: 21.000 nanos | Max Time: 18.758 micros
75% = [avg: 198.000 nanos, max: 558.000 nanos]
90% = [avg: 291.000 nanos, max: 1.011 micros]
99% = [avg: 382.000 nanos, max: 1.751 micros]
99.9% = [avg: 396.000 nanos, max: 2.124 micros]
99.99% = [avg: 398.000 nanos, max: 2.690 micros]
99.999% = [avg: 398.000 nanos, max: 15.835 micros]

Benchmarking remove...
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 323.300 nanos | Min Time: 27.000 nanos | Max Time: 18.084 micros
75% = [avg: 163.000 nanos, max: 423.000 nanos]
90% = [avg: 234.000 nanos, max: 802.000 nanos]
99% = [avg: 309.000 nanos, max: 1.489 micros]
99.9% = [avg: 321.000 nanos, max: 1.850 micros]
99.99% = [avg: 322.000 nanos, max: 2.330 micros]
99.999% = [avg: 323.000 nanos, max: 4.592 micros]
```
</details>

```
PUT => Avg: 190 ns | Min: 21 ns | 99.9% = [avg: 183 ns, max: 814 ns]
PUT => Avg: 659 ns | Min: 23 ns | 99.9% = [avg: 656 ns, max: 2.762 micros]
GET => Avg: 399 ns | Min: 21 ns | 99.9% = [avg: 396 ns, max: 2.124 micros]
DEL => Avg: 323 ns | Min: 27 ns | 99.9% = [avg: 321 ns, max: 1.850 micros]
```

<details>
  <summary> HotSpotVM JIT (with C2 JIT)</summary>

<br/>

```
$ java -XX:-UseJVMCICompiler -XX:+AlwaysPreTouch -Xms4g -Xmx4g -XX:NewSize=512m -XX:MaxNewSize=1024m \
       -cp target/classes:target/coralbench-all.jar \
       com.coralblocks.coralbench.example.IntMapBenchmark 0 10000000 5000000

Arguments: warmup=0 measurements=2000000 mapCapacity=20000

Benchmarking put on empty map... (1) => creating new Entry objects
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 341.770 nanos | Min Time: 29.000 nanos | Max Time: 36.715 micros
75% = [avg: 177.000 nanos, max: 532.000 nanos]
90% = [avg: 258.000 nanos, max: 835.000 nanos]
99% = [avg: 327.000 nanos, max: 1.335 micros]
99.9% = [avg: 338.000 nanos, max: 1.661 micros]
99.99% = [avg: 340.000 nanos, max: 14.384 micros]
99.999% = [avg: 341.000 nanos, max: 17.877 micros]

Benchmarking put after clear()... (2) => hitting the pool of Entry objects
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 596.090 nanos | Min Time: 28.000 nanos | Max Time: 34.792 micros
75% = [avg: 415.000 nanos, max: 876.000 nanos]
90% = [avg: 509.000 nanos, max: 1.113 micros]
99% = [avg: 578.000 nanos, max: 1.596 micros]
99.9% = [avg: 589.000 nanos, max: 2.161 micros]
99.99% = [avg: 594.000 nanos, max: 16.180 micros]
99.999% = [avg: 595.000 nanos, max: 17.084 micros]

Benchmarking get...
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 599.480 nanos | Min Time: 20.000 nanos | Max Time: 31.256 micros
75% = [avg: 418.000 nanos, max: 882.000 nanos]
90% = [avg: 512.000 nanos, max: 1.115 micros]
99% = [avg: 581.000 nanos, max: 1.602 micros]
99.9% = [avg: 592.000 nanos, max: 2.275 micros]
99.99% = [avg: 597.000 nanos, max: 16.152 micros]
99.999% = [avg: 599.000 nanos, max: 16.908 micros]

Benchmarking remove...
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 826.480 nanos | Min Time: 23.000 nanos | Max Time: 65.205 micros
75% = [avg: 516.000 nanos, max: 1.267 micros]
90% = [avg: 675.000 nanos, max: 1.705 micros]
99% = [avg: 797.000 nanos, max: 2.756 micros]
99.9% = [avg: 817.000 nanos, max: 3.420 micros]
99.99% = [avg: 824.000 nanos, max: 16.860 micros]
99.999% = [avg: 826.000 nanos, max: 18.824 micros]
```
</details>

```
PUT => Avg: 342 ns | Min: 29 ns | 99.9% = [avg: 338 ns, max: 1.661 micros]
PUT => Avg: 596 ns | Min: 28 ns | 99.9% = [avg: 589 ns, max: 2.161 micros]
GET => Avg: 599 ns | Min: 20 ns | 99.9% = [avg: 592 ns, max: 2.275 micros]
DEL => Avg: 826 ns | Min: 23 ns | 99.9% = [avg: 817 ns, max: 3.420 micros]
```

<details>
  <summary> HotSpotVM -Xcomp (with Graal JVMCI JIT)</summary>

<br/>

```
$ java -Xcomp -XX:-TieredCompilation \
       -XX:+AlwaysPreTouch -Xms4g -Xmx4g -XX:NewSize=512m -XX:MaxNewSize=1024m \
       -cp target/classes:target/coralbench-all.jar \
       com.coralblocks.coralbench.example.IntMapBenchmark 0 10000000 5000000

Arguments: warmup=0 measurements=2000000 mapCapacity=20000

Benchmarking put on empty map... (1) => creating new Entry objects
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 351.450 nanos | Min Time: 25.000 nanos | Max Time: 3.904 millis
75% = [avg: 180.000 nanos, max: 539.000 nanos]
90% = [avg: 263.000 nanos, max: 859.000 nanos]
99% = [avg: 334.000 nanos, max: 1.358 micros]
99.9% = [avg: 345.000 nanos, max: 1.680 micros]
99.99% = [avg: 347.000 nanos, max: 14.904 micros]
99.999% = [avg: 348.000 nanos, max: 16.979 micros]

Benchmarking put after clear()... (2) => hitting the pool of Entry objects
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 622.520 nanos | Min Time: 26.000 nanos | Max Time: 26.968 micros
75% = [avg: 442.000 nanos, max: 927.000 nanos]
90% = [avg: 540.000 nanos, max: 1.160 micros]
99% = [avg: 610.000 nanos, max: 1.587 micros]
99.9% = [avg: 620.000 nanos, max: 1.991 micros]
99.99% = [avg: 621.000 nanos, max: 4.363 micros]
99.999% = [avg: 622.000 nanos, max: 24.722 micros]

Benchmarking get...
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 628.130 nanos | Min Time: 22.000 nanos | Max Time: 1.479 millis
75% = [avg: 441.000 nanos, max: 930.000 nanos]
90% = [avg: 540.000 nanos, max: 1.165 micros]
99% = [avg: 612.000 nanos, max: 1.683 micros]
99.9% = [avg: 623.000 nanos, max: 2.418 micros]
99.99% = [avg: 626.000 nanos, max: 5.492 micros]
99.999% = [avg: 627.000 nanos, max: 23.719 micros]

Benchmarking remove...
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 886.830 nanos | Min Time: 24.000 nanos | Max Time: 5.870 millis
75% = [avg: 561.000 nanos, max: 1.351 micros]
90% = [avg: 726.000 nanos, max: 1.805 micros]
99% = [avg: 853.000 nanos, max: 2.804 micros]
99.9% = [avg: 874.000 nanos, max: 3.912 micros]
99.99% = [avg: 882.000 nanos, max: 17.076 micros]
99.999% = [avg: 883.000 nanos, max: 19.558 micros]
```
</details>

```
PUT => Avg: 351 ns | Min: 25 ns | 99.9% = [avg: 345 ns, max: 1.680 micros]
PUT => Avg: 623 ns | Min: 26 ns | 99.9% = [avg: 620 ns, max: 1.991 micros]
GET => Avg: 628 ns | Min: 22 ns | 99.9% = [avg: 623 ns, max: 2.418 micros]
DEL => Avg: 887 ns | Min: 24 ns | 99.9% = [avg: 874 ns, max: 3.912 micros]
```

<details>
  <summary> HotSpotVM -Xcomp (with C2 JIT)</summary>

<br/>

```
$ java -XX:-UseJVMCICompiler -Xcomp -XX:-TieredCompilation \
       -XX:+AlwaysPreTouch -Xms4g -Xmx4g -XX:NewSize=512m -XX:MaxNewSize=1024m \
       -cp target/classes:target/coralbench-all.jar \
       com.coralblocks.coralbench.example.IntMapBenchmark 0 10000000 5000000

Arguments: warmup=0 measurements=2000000 mapCapacity=20000

Benchmarking put on empty map... (1) => creating new Entry objects
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 351.310 nanos | Min Time: 26.000 nanos | Max Time: 4.111 millis
75% = [avg: 176.000 nanos, max: 536.000 nanos]
90% = [avg: 261.000 nanos, max: 872.000 nanos]
99% = [avg: 334.000 nanos, max: 1.395 micros]
99.9% = [avg: 345.000 nanos, max: 1.728 micros]
99.99% = [avg: 346.000 nanos, max: 14.244 micros]
99.999% = [avg: 348.000 nanos, max: 16.885 micros]

Benchmarking put after clear()... (2) => hitting the pool of Entry objects
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 642.030 nanos | Min Time: 25.000 nanos | Max Time: 34.457 micros
75% = [avg: 449.000 nanos, max: 954.000 nanos]
90% = [avg: 552.000 nanos, max: 1.208 micros]
99% = [avg: 627.000 nanos, max: 1.722 micros]
99.9% = [avg: 638.000 nanos, max: 2.199 micros]
99.99% = [avg: 640.000 nanos, max: 5.793 micros]
99.999% = [avg: 641.000 nanos, max: 18.050 micros]

Benchmarking get...
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 628.040 nanos | Min Time: 22.000 nanos | Max Time: 739.780 micros
75% = [avg: 447.000 nanos, max: 936.000 nanos]
90% = [avg: 546.000 nanos, max: 1.158 micros]
99% = [avg: 614.000 nanos, max: 1.588 micros]
99.9% = [avg: 624.000 nanos, max: 2.043 micros]
99.99% = [avg: 626.000 nanos, max: 6.296 micros]
99.999% = [avg: 627.000 nanos, max: 16.825 micros]

Benchmarking remove...
Measurements: 2,000,000 | Warm-Up: 0 | Iterations: 2,000,000
Avg Time: 859.440 nanos | Min Time: 25.000 nanos | Max Time: 3.341 millis
75% = [avg: 555.000 nanos, max: 1.331 micros]
90% = [avg: 716.000 nanos, max: 1.749 micros]
99% = [avg: 835.000 nanos, max: 2.614 micros]
99.9% = [avg: 853.000 nanos, max: 3.249 micros]
99.99% = [avg: 856.000 nanos, max: 14.535 micros]
99.999% = [avg: 857.000 nanos, max: 19.239 micros]
```
</details>

```
PUT => Avg: 351 ns | Min: 26 ns | 99.9% = [avg: 638 ns, max: 2.199 micros]
PUT => Avg: 642 ns | Min: 25 ns | 99.9% = [avg: 638 ns, max: 2.199 micros]
GET => Avg: 628 ns | Min: 22 ns | 99.9% = [avg: 624 ns, max: 2.043 micros]
DEL => Avg: 859 ns | Min: 25 ns | 99.9% = [avg: 853 ns, max: 3.249 micros]
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
