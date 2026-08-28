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

import java.util.Locale;

import org.junit.Test;

public class BenchTest {

	@Test
	public void measuresWhenCounterPoolIsEmpty() {
		Bench bench = new Bench();

		assertTrue(bench.measure(1));
		assertEquals(1, bench.getMeasurements());
	}

	@Test
	public void acceptsWarmupCountsBeyondIntegerRange() {
		Bench bench = new Bench((long) Integer.MAX_VALUE + 1);

		assertTrue(bench.isWarmingUp());
		assertEquals(0L, bench.getIterations());
		assertEquals(0L, bench.getMeasurements());
	}

	@Test
	public void consumesMarkAfterOneMeasurement() {
		Bench bench = new Bench();

		bench.mark();
		assertTrue(bench.measure() >= 0);
		assertEquals(1, bench.getIterations());
		assertEquals(1, bench.getMeasurements());

		assertEquals(-1, bench.measure());
		assertEquals(1, bench.getIterations());
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

	@Test
	public void preservesFractionalPercentileAverages() {
		Bench bench = new Bench();
		bench.measure(10);
		bench.measure(11);

		String results = bench.results();
		assertTrue(results, results.contains("75% = [avg: 10.500 nanos, max: 11.000 nanos]"));
	}

	@Test
	public void formatsResultsIndependentlyOfDefaultLocale() {
		Locale originalLocale = Locale.getDefault();
		try {
			Locale.setDefault(Locale.GERMANY);
			Bench bench = new Bench();
			for(int i = 0; i < 1000; i++) {
				bench.measure(i % 2 == 0 ? 10 : 11);
			}

			String results = bench.results();
			assertTrue(results, results.contains("Measurements: 1,000"));
			assertTrue(results, results.contains("Avg Time: 10.500 nanos"));
			assertTrue(results, results.contains("75% = ["));
		} finally {
			Locale.setDefault(originalLocale);
		}
	}

	@Test
	public void spellsOutSeconds() {
		Bench bench = new Bench();
		bench.measure(1_000_000_000L);
		bench.measure(2_000_000_000L);

		String results = bench.results(false);
		assertTrue(results, results.contains("Avg Time: 1.500 seconds"));
		assertTrue(results, results.contains("Min Time: 1.000 second"));
		assertTrue(results, results.contains("Max Time: 2.000 seconds"));
	}
}
