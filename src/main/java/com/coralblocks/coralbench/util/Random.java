package com.coralblocks.coralbench.util;

public class Random {
	
    private long seed;

    public Random(long seed) {
        this.seed = seed;
    }

    public long nextLong() {
        seed = (seed * 1664525L + 1013904223L) & 0xFFFFFFFFL;
        long high = (seed >> 16) & 0xFFFFFFFFL;
        seed = (seed * 1664525L + 1013904223L) & 0xFFFFFFFFL;
        long low = (seed >> 16) & 0xFFFFFFFFL;
        return (high << 32) | low;
    }

    public long nextLong(long bound) {
        if (bound <= 0) {
            throw new IllegalArgumentException("Bound must be positive");
        }
        return nextLong() % bound;
    }
    
    public int nextInt() {
        seed = (seed * 1664525L + 1013904223L) & 0xFFFFFFFFL;
        return (int) (seed >> 16);
    }

    public int nextInt(int bound) {
        if (bound <= 0) {
            throw new IllegalArgumentException("Bound must be positive");
        }
        return nextInt() % bound;
    }

    public static void main(String[] args) {
    	
    	long sameSeed = 16238935L;
    	
    	Random r1 = new Random(sameSeed);
    	Random r2 = new Random(sameSeed);
    	
    	for(int i = 0; i < 10; i++) {
    		System.out.println(r1.nextLong() + " " + r2.nextLong());
    	}
    	
    	System.out.println();
    	
    	for(int i = 0; i < 10; i++) {
    		System.out.println(r1.nextLong(100000) + " " + r2.nextLong(100000));
    	}
    	
    	System.out.println();
    	
    	for(int i = 0; i < 10; i++) {
    		System.out.println(r1.nextInt() + " " + r2.nextInt());
    	}
    	
    	System.out.println();
    	
    	for(int i = 0; i < 10; i++) {
    		System.out.println(r1.nextInt(100000) + " " + r2.nextInt(100000));
    	}
    	
    	System.out.println();
    }
}
