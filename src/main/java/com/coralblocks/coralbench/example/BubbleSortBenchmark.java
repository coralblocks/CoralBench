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
package com.coralblocks.coralbench.example;

import java.util.Arrays;

import com.coralblocks.coralbench.Bench;

public class BubbleSortBenchmark {
    
    private static int[] HEAP_ARRAY;
     
    public static final void main(String[] args) {

    	final int warmup = args.length > 0 ? Integer.parseInt(args[0]) : 1_000_000;
        final int measurements = args.length > 1 ? Integer.parseInt(args[1]) : 10_000_000;
        final int total = measurements + warmup;
        final int arraySize = 60;
         
        HEAP_ARRAY = new int[arraySize];
         
        Bench bench = new Bench(warmup);
         
        long x = 0;
         
        for(int i = 0; i < total; i++) {
             
            bench.mark();
             
            doSomething(HEAP_ARRAY, HEAP_ARRAY.length);
             
            bench.measure();
             
            for(int j = 0; j < HEAP_ARRAY.length; j++) {
                x += HEAP_ARRAY[j];
            }
        }
         
        System.out.println("Value computed: " + x);
        System.out.println("Array: " + Arrays.toString(HEAP_ARRAY));
        bench.printResults();
    }
     
    private static final void swapping(int[] array, int x, int y) {
        int temp = array[x];
        array[x] = array[y];
        array[y] = temp;
    }
     
    private static final void bubbleSort(int[] array, int size) {
        for(int i = 0; i < size; i++) {
            int swaps = 0; // flag to detect any swap is there or not
            for(int j = 0; j < size - i - 1; j++) {
                if (array[j] > array[j + 1]) { // when the current item is bigger than next
                    swapping(array, j, j + 1);
                    swaps = 1;
                }
            }
            if (swaps == 0) break; // No swap in this pass, so array is sorted
        }
    }
     
    private static final void doSomething(int[] array, int size) {
         
        for(int z = 0; z < size; z++) {
            array[z] = size - z;
        }
 
        bubbleSort(array, size);
    }
}