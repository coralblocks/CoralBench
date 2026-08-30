#!/bin/bash

WARMUP=${1:-1000000}
MEASUREMENTS=${2:-10000000}

java25() {
    if [[ "$(uname -s)" == "Darwin" ]]; then
        sudo rm -f /Library/Java/JavaVirtualMachines/Default
        sudo ln -sf /Library/Java/JavaVirtualMachines/jdk25-oracle /Library/Java/JavaVirtualMachines/Default
    else
        rm -f /usr/java/default
        ln -s /usr/java/java25 /usr/java/default
    fi
}

graal25() {
    if [[ "$(uname -s)" == "Darwin" ]]; then
        sudo rm -f /Library/Java/JavaVirtualMachines/Default
        sudo ln -sf /Library/Java/JavaVirtualMachines/jdk25-graal /Library/Java/JavaVirtualMachines/Default
    else
        rm -f /usr/java/default
        ln -s /usr/java/graal25 /usr/java/default
    fi
}

CMD_JAVA="java -cp target/classes:target/coralbench-all.jar com.coralblocks.coralbench.example.IntMapBenchmark $WARMUP $MEASUREMENTS"
CMD_CPP="./target/cpp/int_map_benchmark $WARMUP $MEASUREMENTS"

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
./bin/compileIntMapBenchmarkCpp.sh
echo $CMD_CPP
$CMD_CPP
