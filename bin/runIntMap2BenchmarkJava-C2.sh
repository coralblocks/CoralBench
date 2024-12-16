#!/bin/bash

WARMUP=${1:-0}
MEASUREMENTS=${2:-2000000}
CAPACITY=${3:-20000}
POOLSIZE=${4:-2000000}

java -XX:-UseJVMCICompiler -XX:+AlwaysPreTouch -Xms4g -Xmx4g -XX:NewSize=512m -XX:MaxNewSize=1024m -cp target/classes:target/coralbench-all.jar com.coralblocks.coralbench.example.IntMap2Benchmark $WARMUP $MEASUREMENTS $CAPACITY $POOLSIZE
