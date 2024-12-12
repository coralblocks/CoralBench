#!/bin/bash

PASSES=${1:-1000}

echo "Regular JIT with warm-up"
java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -cp target/classes:target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark 1000000 $PASSES

echo "Regular JIT without warm-up"
java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -cp target/classes:target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark 5 $PASSES

echo "-Xcomp -XX:-TieredCompilation with warm-up"
java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -Xcomp -XX:-TieredCompilation -cp target/classes:target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark 1000000 $PASSES

echo "-Xcomp -XX:-TieredCompilation without warm-up"
java -XX:+UnlockExperimentalVMOptions -XX:+UseJVMCICompiler -Xcomp -XX:-TieredCompilation -cp target/classes:target/coralbench-all.jar com.coralblocks.coralbench.example.SleepBenchmark 5 $PASSES
