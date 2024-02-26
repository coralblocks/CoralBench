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
package com.coralblocks.coralcontest.util;

public class MutableInt {
	
	private static int DEFAULT_VALUE = 0;
	
	private int value;
	
	private boolean isNull;
	
	public MutableInt(int value) {
		set(value);
	}
	
	public MutableInt() {
		this(DEFAULT_VALUE);
	}
	
	public void set(int value) {
		this.value = value;
		this.isNull = false;
	}
	
	public boolean isNull() {
		return isNull;
	}
	
	public void setNull() {
		isNull = true;
	}
	
	public int get() {
		if (isNull) throw new NullPointerException();
		return value;
	}
	
	@Override
	public String toString() {
		return isNull ? "NULL" : String.valueOf(value);
	}
}
