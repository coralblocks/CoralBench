#!/bin/bash

g++ -std=c++17 src/main/c/CoralBlocks/CoralBench/Util/MathUtils.cpp src/main/c/TestMathUtils.cpp -o src/main/c/TestMathUtils

g++ -std=c++17 src/main/c/CoralBlocks/CoralBench/Util/MathUtils.cpp src/main/c/TestLongMap.cpp -o src/main/c/TestLongMap

g++ -std=c++17 src/main/c/CoralBlocks/CoralBench/Util/MutableInt.cpp src/main/c/TestMutableInt.cpp -o src/main/c/TestMutableInt

g++ -std=c++17 src/main/c/TestFastObjectList.cpp -o src/main/c/TestFastObjectList

g++ -std=c++17 src/main/c/CoralBlocks/CoralBench/Util/Bench.cpp src/main/c/TestBench.cpp -o src/main/c/TestBench


