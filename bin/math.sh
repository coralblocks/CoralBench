#!/bin/bash

WARMUP=${1:-1000000}
MEASUREMENTS=${2:-10000000}

java25() {
    rm -f /usr/java/default
    ln -s /usr/java/java25 /usr/java/default
}

graal25() {
    rm -f /usr/java/default
    ln -s /usr/java/graal25 /usr/java/default
}

CMD_JAVA="java -cp target/classes:target/coralbench-all.jar com.coralblocks.coralbench.example.MathBenchmark $WARMUP $MEASUREMENTS"
CMD_CPP="./target/cpp/math_benchmark $WARMUP $MEASUREMENTS"

echo "Regular JIT:"
java25
java --version
echo $CMD_JAVA
$CMD_JAVA

echo "GraalVM JIT:"
graal25
java --version
echo $CMD_JAVA
$CMD_JAVA

echo "C/C++:"
./bin/compileMathBenchmarkCpp.sh
echo $CMD_CPP
$CMD_CPP

