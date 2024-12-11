#!/bin/bash

WARMUP=${1:-0}
MEASUREMENTS=${2:-10000000}
CAPACITY=${3:-5000000}

./target/cpp/int_map_benchmark $WARMUP $MEASUREMENTS $CAPACITY

