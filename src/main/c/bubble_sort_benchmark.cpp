/* 
 * Copyright 2024 (c) CoralBlocks - http://www.coralblocks.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 */
#include "bench.hpp"

static int* HEAP_ARRAY;

static void swapping(int* array, int x, int y) {
    int temp = array[x];
    array[x] = array[y];
    array[y] = temp;
}

static void bubbleSort(int* array, int size) {
    for(int i = 0; i < size; i++) {
        bool swaps = false; // flag to detect any swap
        for(int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) { // when current item is bigger than next
                swapping(array, j, j + 1);
                swaps = true;
            }
        }
        if (!swaps) break; // No swap in this pass, so array is sorted
    }
}

static void doSomething(int* array, int size) {

    for(int z = 0; z < size; z++) {
        array[z] = size - z;
    }

    bubbleSort(array, size);
}

int main(int argc, char* argv[]) {

    int warmup = (argc > 1) ? atoi(argv[1]) : 1000000;
    int measurements = (argc > 2) ? atoi(argv[2]) : 10000000;
    int total = measurements + warmup;
    int arraySize = 60;
    
    HEAP_ARRAY = (int*) malloc(sizeof(int) * arraySize);
    
    Bench* bench = new Bench(warmup);
    
    long x = 0;
    
    for(int i = 0; i < total; i++) {
        
        bench->mark();
        doSomething(HEAP_ARRAY, arraySize);
        bench->measure();
        
        for(int j = 0; j < arraySize; j++) {
            x += HEAP_ARRAY[j];
        }
    }
    
    printf("Value computed: %ld\n", x);
    
    printf("Array: [");
    for (int j = 0; j < arraySize; j++) {
        printf("%d", HEAP_ARRAY[j]);
        if (j < arraySize - 1) printf(", ");
    }
    printf("]\n");
    
    bench->printResults();
    
    free(HEAP_ARRAY);
    delete bench;

    return 0;
}
