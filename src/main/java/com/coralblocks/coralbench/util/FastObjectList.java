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
package com.coralblocks.coralbench.util;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class FastObjectList<E> implements Iterable<E> {
	
	private E[] values;
	private int pointer = -1;
	private int removed = 0;
	private final ReusableIterator iter = new ReusableIterator();
	
	@SuppressWarnings("unchecked")
    	public FastObjectList(int initialCapacity) {
		this.values = (E[]) new Object[initialCapacity];
	}
	
	public final int size() {
		return pointer + 1 - removed;
	}
	
	public final int getCapacity() {
		return values.length;
	}
	
	public final boolean isEmpty() {
		return size() == 0;
	}
	
	public final boolean isAtCapacity() {
		return pointer + 1 == values.length;
	}
	
	public final boolean add(E value) {
		if (pointer + 1 == values.length) return false;
		this.values[++pointer] = value;
		return true;
	}
	
	@SuppressWarnings("unchecked")
	public int grow() {
		
		int newCapacity = values.length + values.length / 2;
		
        E[] newValues = (E[]) new Object[newCapacity];
		
		System.arraycopy(values, 0, newValues, 0, values.length);
		
		this.values = newValues;
		
		return newCapacity;
	}
	
	public final E removeLast() {
		if (isEmpty()) throw new NoSuchElementException();
		for(int i = pointer; i >= 0; i--) {
			if (values[i] != null) {
				E old = values[i];
				values[i] = null;
				removed++;
				return old;
			}
		}
		throw new IllegalStateException("Should never happen!");
	}
	
	public final E removeFirst() {
		if (isEmpty()) throw new NoSuchElementException();
		for(int i = 0; i <= pointer; i++) {
			if (values[i] != null) {
				E save = values[i];
				values[i] = null;
				removed++;
				return save;
			}
		}
		throw new IllegalStateException("Should never happen!");
	}
	
	public final boolean remove(E value) {
		return remove(value, false);
	}
	
	public final boolean remove(E value, boolean fromTail) {
		if (!fromTail) {
    		for(int i = 0; i <= pointer; i++) {
    			if (values[i] == value) {
    				values[i] = null;
    				removed++;
    				return true;
    			}
    		}
		} else {
			for(int i = pointer; i >= 0; i--) {
    			if (values[i] == value) {
    				values[i] = null;
    				removed++;
    				return true;
    			}
    		}
		}
		return false;
	}
	
	public final E remove(int index) {
		return remove(index, false);
	}
	
	public final E remove(int index, boolean fromTail) {
		if (index >= size()) throw new NoSuchElementException("Cannot remove index " + index + " from a list of size " + size() + " (fromTail=" + fromTail + ")");
		if (!fromTail) {
			int count = -1;
			for(int i = 0; i <= pointer; i++) {
				if (values[i] != null && ++count == index) {
					E save = values[i];
					values[i] = null;
					removed++;
					return save;
				}
			}
		} else {
			int count = -1;
			for(int i = pointer; i >= 0; i--) {
    			if (values[i] != null && ++count == index) {
    				E save = values[i];
    				values[i] = null;
    				removed++;
    				return save;
    			}
    		}
		}
		return null;
	}
	
	public final E first() {
		if (isEmpty()) throw new NoSuchElementException();
		for(int i = 0; i <= pointer; i++) {
			if (values[i] != null) {
				return values[i];
			}
		}
		throw new IllegalStateException("Should never happen!");
	}
	
	public final E last() {
		if (isEmpty()) throw new NoSuchElementException();
		for(int i = pointer; i >= 0; i--) {
			if (values[i] != null) {
				return values[i];
			}
		}
		throw new IllegalStateException("Should never happen!");
	}
	
	public final void clear() {
		pointer = -1;
		removed = 0;
	}
	
	@Override
    public Iterator<E> iterator() {
	    iter.reset();
	    return iter;
    }
	
	private class ReusableIterator implements Iterator<E> {
		
		private int pointer;
		private int size;
		private int returned;
		
		public void reset() {
			this.size = FastObjectList.this.size();
			this.pointer = 0;
			this.returned = 0;
		}

		@Override
        public boolean hasNext() {
	        return this.returned < size;
        }

		@Override
        public E next() {
			E value;
			while((value = FastObjectList.this.values[this.pointer++]) == null);
			this.returned++;
			return value;
        }

		@Override
        public void remove() {
			throw new UnsupportedOperationException();
        }
	}
}
