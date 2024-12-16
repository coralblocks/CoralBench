#!/bin/bash

rm -f target/graal/IntMap2Benchmark

mkdir -p target/graal

native-image --gc=G1 -R:+AlwaysPreTouch -R:InitialHeapSize=4g -R:MaxHeapSize=4g -R:InitialHeapSize=512m -R:MaxHeapSize=1024m -march=native -cp target/coralbench-all.jar com.coralblocks.coralbench.example.IntMap2Benchmark -o target/graal/IntMap2Benchmark --no-fallback -O3 --initialize-at-build-time


