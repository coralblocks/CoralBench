#!/bin/bash

WARMUP=${1:-1000000}
MEASUREMENTS=${2:-1000000}
CAPACITY=${3:-100000}

./target/cpp/int_map_benchmark $WARMUP $MEASUREMENTS $CAPACITY

