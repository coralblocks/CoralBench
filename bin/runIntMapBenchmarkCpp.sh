#!/bin/bash

WARMUP=${1:-0}
MEASUREMENTS=${2:-2000000}
CAPACITY=${3:-20000}

./target/cpp/int_map_benchmark $WARMUP $MEASUREMENTS $CAPACITY

