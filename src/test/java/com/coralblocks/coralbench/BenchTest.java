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
package com.coralblocks.coralbench;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

public class BenchTest {

	@Test
	public void measuresWhenMutableIntPoolIsEmpty() {
		Bench bench = new Bench();

		assertTrue(bench.measure(1));
		assertEquals(1, bench.getMeasurements());
	}

	@Test
	public void reportsWhetherWarmupIsInProgress() {
		Bench bench = new Bench(2);

		assertTrue(bench.isWarmingUp());
		bench.measure(1);
		assertTrue(bench.isWarmingUp());
		bench.measure(1);
		assertFalse(bench.isWarmingUp());
	}
}
