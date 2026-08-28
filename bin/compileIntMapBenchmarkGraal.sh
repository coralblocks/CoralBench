#!/bin/bash

rm -f target/graal/IntMapBenchmark

mkdir -p target/graal

native-image --gc=G1 -R:+AlwaysPreTouch -R:InitialHeapSize=4g -R:MaxHeapSize=4g -march=native -cp target/coralbench-all.jar com.coralblocks.coralbench.example.IntMapBenchmark -o target/graal/IntMapBenchmark --no-fallback -O3 --initialize-at-build-time

