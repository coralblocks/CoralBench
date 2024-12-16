#!/bin/bash

rm -f target/cpp/int_map2.o target/cpp/bench.o target/cpp/int_map2_benchmark.o target/cpp/int_map2_benchmark

mkdir -p target/cpp

clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/int_map2.cpp -o ./target/cpp/int_map2.o
clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/bench.cpp -o ./target/cpp/bench.o
clang++ -Ofast -march=native -flto -std=c++17 -I./src/main/c -c ./src/main/c/int_map2_benchmark.cpp -o ./target/cpp/int_map2_benchmark.o

clang++ -Ofast -march=native -flto -std=c++17 -o ./target/cpp/int_map2_benchmark ./target/cpp/int_map2.o ./target/cpp/bench.o ./target/cpp/int_map2_benchmark.o





