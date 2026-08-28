#!/bin/bash

rm -f target/cpp/bench.o target/cpp/int_map_benchmark.o target/cpp/int_map_benchmark

mkdir -p target/cpp

clang++ -O3 -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/bench.cpp -o ./target/cpp/bench.o
clang++ -O3 -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/int_map_benchmark.cpp -o ./target/cpp/int_map_benchmark.o

clang++ -O3 -march=native -flto -std=c++17 -o ./target/cpp/int_map_benchmark ./target/cpp/bench.o ./target/cpp/int_map_benchmark.o



