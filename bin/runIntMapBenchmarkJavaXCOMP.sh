#!/bin/bash

WARMUP=${1:-1000000}
MEASUREMENTS=${2:-1000000}
CAPACITY=${3:-100000}

java -verbose:gc -Xcomp -XX:-TieredCompilation -XX:+AlwaysPreTouch -Xms4g -Xmx4g -XX:NewSize=512m -XX:MaxNewSize=1024m -cp target/classes:target/coralbench-all.jar com.coralblocks.coralbench.example.IntMapBenchmark $WARMUP $MEASUREMENTS $CAPACITY
