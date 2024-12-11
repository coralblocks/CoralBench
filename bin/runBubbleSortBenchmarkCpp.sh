#!/bin/bash

WARMUP=${1:-0}
MEASUREMENTS=${2:-10000000}

./target/cpp/bubble_sort_benchmark $WARMUP $MEASUREMENTS

