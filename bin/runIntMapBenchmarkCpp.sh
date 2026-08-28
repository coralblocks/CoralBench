#!/bin/bash

WARMUP=${1:-0}
MEASUREMENTS=${2:-3000000}
CAPACITY=${3:-1000000}

./target/cpp/int_map_benchmark $WARMUP $MEASUREMENTS $CAPACITY
