#!/bin/bash

WARMUP=${1:-1000000}
MEASUREMENTS=${2:-1000000}
CAPACITY=${3:-100000}

java -cp target/classes:target/coralbench-all.jar com.coralblocks.coralbench.example.IntMapBenchmark $WARMUP $MEASUREMENTS $CAPACITY
