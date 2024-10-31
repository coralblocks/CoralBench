package com.coralblocks.coralbench.example;

import java.util.Arrays;

import com.coralblocks.coralbench.Bench;

public class BubbleSortBenchmark {
    
    private static int[] HEAP_ARRAY;
     
    public static void main(String[] args) {
         
        int measurements = 10000000;
        int warmup = 1000000;
        int arraySize = 60;
         
        HEAP_ARRAY = new int[arraySize];
         
        Bench bench = new Bench(warmup);
         
        long x = 0;
         
        for(int i = 0; i < measurements + warmup; i++) {
             
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
     
    private final static void swapping(int[] array, int x, int y) {
        int temp = array[x];
        array[x] = array[y];
        array[y] = temp;
    }
     
    private final static void bubbleSort(int[] array, int size) {
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
     
    private final static void doSomething(int[] array, int size) {
         
        for(int z = 0; z < size; z++) {
            array[z] = size - z;
        }
 
        bubbleSort(array, size);
    }
}