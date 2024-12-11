#!/bin/bash

WARMUP=${1:-0}
MEASUREMENTS=${2:-10000000}

./target/cpp/sleep_benchmark $WARMUP $MEASUREMENTS

