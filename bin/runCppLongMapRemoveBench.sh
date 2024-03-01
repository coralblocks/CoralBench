#!/bin/bash

ITERATIONS=${1:-100000}
PASSES=${2:-6}
CORE_ID=${3:--1}

CMD="./src/main/c/LongMapRemoveBench $ITERATIONS $PASSES $CORE_ID"

echo $CMD

$CMD


