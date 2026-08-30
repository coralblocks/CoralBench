#!/bin/bash

WARMUP=${1:-1000000}
MEASUREMENTS=${2:-10000000}

./target/graal/BubbleSortBenchmark $WARMUP $MEASUREMENTS
