package com.coralblocks.coralbench.util;

import com.sun.jna.LastErrorException;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.PointerType;
import com.sun.jna.ptr.LongByReference;

public class ThreadPinning {
	
	private static boolean DEBUG = false;
	
	interface CLibrary extends Library {
		
		public int sched_setaffinity(final int pid, final int cpusetsize, final PointerType cpuset) throws LastErrorException;
	}
	
	private static CLibrary cLibrary = null;
	
	static {
		
		 if (Platform.isLinux()) {
			 // Load the C library using JNA
			 cLibrary = Native.load("c", CLibrary.class);
			 if (DEBUG) System.out.println("CLibrary loaded for thread pinning!");
		 } else {
			 if (DEBUG) System.out.println("Not on Linux! Thread pinning will have no effect!");
		 }
	}
	
	public static void pinCurrentThread(int coreId) {
		
		try {
		
	        if (cLibrary != null) {
	
	            // Create a mask with the specified core index
	            long mask =  1L << (long) coreId;
	
	            // Set CPU affinity using sched_setaffinity
	            int result = cLibrary.sched_setaffinity(0, Long.BYTES, new LongByReference(mask));
	
	            if (result != 0) throw new RuntimeException("Cannot pin thread!");
	            
	            if (DEBUG) System.out.println("Set affinity to " + coreId + " for thread " + Thread.currentThread().getName());
	        }
	        
		} catch(Exception e) {
			throw new RuntimeException(e);
		}
	}
}