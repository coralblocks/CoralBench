#!/bin/bash

WARMUP=${1:-0}
MEASUREMENTS=${2:-10000000}
CAPACITY=${3:-5000000}

java -Xcomp -XX:-TieredCompilation -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -XX:+AlwaysPreTouch -Xms4g -Xmx4g -XX:NewSize=512m -XX:MaxNewSize=1024m -cp target/classes:target/coralbench-all.jar com.coralblocks.coralbench.example.IntMapBenchmark $WARMUP $MEASUREMENTS $CAPACITY
