#!/bin/bash

rm -f target/cpp/bubble_sort_benchmark target/cpp/bench.o target/cpp/bubble_sort_benchmark.o

mkdir -p target/cpp

clang++ -O3 -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/bench.cpp -o ./target/cpp/bench.o
clang++ -O3 -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/bubble_sort_benchmark.cpp -o ./target/cpp/bubble_sort_benchmark.o

clang++ -O3 -march=native -flto -std=c++17 -o ./target/cpp/bubble_sort_benchmark ./target/cpp/bench.o ./target/cpp/bubble_sort_benchmark.o



