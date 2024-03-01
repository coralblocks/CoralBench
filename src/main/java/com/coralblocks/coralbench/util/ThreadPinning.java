package com.coralblocks.coralbench.util;

import com.sun.jna.Platform;
import com.sun.jna.Library;
import com.sun.jna.Native;

public class ThreadPinning {
	
	interface CLibrary extends Library {
	    // Define the library methods
	    int sched_setaffinity(int pid, int cpusetsize, long[] mask);
	}
	
    private static int getProcessId() {
        // Accessing the process ID in a platform-dependent way
        return (int) ProcessHandle.current().pid();
    }
    
    public static void pinCurrentThread(int coreId, int pid) {
    	pinCurrentThread(coreId, pid, Long.BYTES);
    }

	public static void pinCurrentThread(int coreId, int pid, int cpusetsize) {
		
		try {
		
	        if (Platform.isLinux()) {
	            // Load the C library using JNA
	            CLibrary cLibrary = Native.load("c", CLibrary.class);
	
	            // Create a mask with the specified core index
	            long[] mask = { 1L << coreId };
	
	            // Set CPU affinity using sched_setaffinity
	            int result = cLibrary.sched_setaffinity(pid, cpusetsize, mask);
	
	            if (result != 0) throw new RuntimeException("Cannot pin thread!");
	        }
	        
		} catch(Exception e) {
			throw new RuntimeException(e);
		}
	}
	
	public static class PinnedThread extends Thread {
		
		private final int initialCoreId;
		private volatile int pid = -1;
		private boolean isRunning = false;
		
		public PinnedThread(int initialCoreId) {
			super();
			this.initialCoreId = initialCoreId;
		}
		
		@Override
		public void run() {
			
			pid = getProcessId();
			
			pinCurrentThread(initialCoreId, pid);
			
			synchronized(this) {
				isRunning = true;
			}
			
			while(isRunning()) {
				// busyspin...
			}
			
			System.out.println("Thread exited!");
		}
		
		public synchronized void stopMe() {
			isRunning = false;
		}
		
		public synchronized boolean isRunning() {
			return isRunning;
		}
		
		public int getPid() { // Does access around pid need to be synchronized?
							  // I don't think so but I can be wrong
			return pid;
		}
	}
	
	public static void main(String[] args) throws Exception {
		
		PinnedThread pThread = new PinnedThread(1);
		
		pThread.start();
		
		Thread.sleep(6000);
		
		pinCurrentThread(2, pThread.getPid());
		
		Thread.sleep(6000);
		
		pinCurrentThread(3, pThread.getPid());
		
		Thread.sleep(6000);
		
		pinCurrentThread(1, pThread.getPid());
		
		Thread.sleep(6000);
		
		pThread.stopMe();
	}
}