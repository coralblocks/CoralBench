#!/bin/bash

rm -f target/graal/BubbleSortMapBenchmark

mkdir -p target/graal

native-image --native-compiler-path=/usr/bin/clang --gc=G1 -march=native -cp target/coralbench-all.jar com.coralblocks.coralbench.example.BubbleSortBenchmark -o target/graal/BubbleSortBenchmark -O3 --initialize-at-build-time
