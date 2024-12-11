#!/bin/bash

rm -f target/cpp/sleep_benchmark target/cpp/bench.o target/cpp/sleep_benchmark.o

mkdir -p target/cpp

clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/bench.cpp -o ./target/cpp/bench.o
clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/sleep_benchmark.cpp -o ./target/cpp/sleep_benchmark.o

clang++ -Ofast -march=native -flto -std=c++17 -o ./target/cpp/sleep_benchmark ./target/cpp/bench.o ./target/cpp/sleep_benchmark.o





