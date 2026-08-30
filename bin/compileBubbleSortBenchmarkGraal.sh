#!/bin/bash

rm -f target/graal/BubbleSortMapBenchmark

mkdir -p target/graal

native-image --gc=G1 -march=native -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark -o target/graal/BubbleSortBenchmark --no-fallback -O3 --initialize-at-build-time
