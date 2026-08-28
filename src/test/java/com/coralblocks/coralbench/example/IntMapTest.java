/* 
 * Copyright 2015-2024 (c) CoralBlocks LLC - http://www.coralblocks.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.coralblocks.coralbench.example;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;

import org.junit.Test;

public class IntMapTest {

	@Test
	public void putsWhenEntryPoolIsEmpty() {
		IntMap<String> map = new IntMap<String>(1, 1);

		assertNull(map.put(0, "value"));
		assertEquals("value", map.get(0));
	}

	@Test
	public void supportsNegativeKeys() {
		IntMap<String> map = new IntMap<String>(2, 2);

		assertNull(map.put(-1, "negative"));
		assertNull(map.put(1, "positive"));
		assertNull(map.put(Integer.MIN_VALUE, "minimum"));
		assertNull(map.put(0, "zero"));

		assertEquals("negative", map.get(-1));
		assertEquals("positive", map.get(1));
		assertEquals("minimum", map.get(Integer.MIN_VALUE));
		assertEquals("zero", map.get(0));
		assertEquals("negative", map.remove(-1));
		assertNull(map.get(-1));
	}
}
