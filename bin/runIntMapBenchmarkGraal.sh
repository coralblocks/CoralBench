#!/bin/bash

WARMUP=${1:-0}
MEASUREMENTS=${2:-2000000}
CAPACITY=${3:-20000}

./target/graal/IntMapBenchmark $WARMUP $MEASUREMENTS $CAPACITY

