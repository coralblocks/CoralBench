#include <iostream>
#include <iomanip>
#include <list>
#include <unordered_map>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <vector>
#include <thread>
#include <map>
#include <sstream>

using std::unordered_map;
using std::vector;
using std::runtime_error;

namespace CoralBlocks::CoralBench::Util {

    class Bench {
        
    private:

        static const int DEFAULT_WARMUP = 0;
        static const int NUMBER_OF_DECIMALS = 3;
        
        static const bool INCLUDE_STDEV = true;
        static const bool INCLUDE_MORE_PERCS = false;
        static const bool INCLUDE_WORST_PERCS = false;
        static const bool INCLUDE_TOTALS = false;

        long time;
        int count;
        int size;
        long totalTime;
        int warmup;
        long minTime;
        long maxTime;

        struct timespec ts;
        std::unordered_map<long, int> results;
        std::vector<long> tempList;

        long get_nano_ts() {
            clock_gettime(CLOCK_MONOTONIC, &ts);
            return ts.tv_sec * 1000000000 + ts.tv_nsec;
        }

    public:

        Bench() : Bench(DEFAULT_WARMUP) {}

        Bench(const int warmup) : warmup(warmup) {
            reset(true);
        }

        void reset(bool repeatWarmup) {
            time = 0;
            count = 0;
            size = 0;
            totalTime = 0;
            if (!repeatWarmup) warmup = 0;
            minTime = LONG_MAX;
            maxTime = LONG_MIN;
            results.clear();
            tempList.clear();
        }

        long measure() {
            if (time > 0) {
                long elapsed = get_nano_ts() - time;
                if (elapsed < 0) throw runtime_error("Found a negative elapsed time");
                const bool counted = measure(elapsed);
                if (counted) {
                    return elapsed;
                }
            }
            return -1;
        }

        bool isWarmingUp() const {
            return warmup <= count;
        }

        int getCount() const {
            return count;
        }

        double getAverage() const {
            return avg();
        }

        void printResults() {
            std::cout << resultsStr() << std::endl << std::endl;
        }

        static void doSleep(Bench& bench) {
            bench.mark();
            std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
            bench.measure();
        }

        long mark() {
            time = get_nano_ts();
            return time;
        }

        bool measure(long lastNanoTime) {
            if (++count > warmup) {
                totalTime += lastNanoTime;
                minTime = std::min(minTime, lastNanoTime);
                maxTime = std::max(maxTime, lastNanoTime);

                if (results.count(lastNanoTime) > 0) {
                    results[lastNanoTime] = results[lastNanoTime] + 1;
                } else {
                    results[lastNanoTime] = 1;
                }
                size++;
            }
            return false;
        }

    private:

        double avg() const {
            int realCount = count - warmup;
            if (realCount <= 0) return 0;
            double avg = static_cast<double>(totalTime) / static_cast<double>(realCount);
            double rounded = std::round(avg * 100.0) / 100.0;
            return rounded;
        }

        double round(double d, int decimals) const {
            double pow = std::pow(10, decimals);
            return std::round(d * pow) / pow;
        }

        double round(double d) const {
            return round(d, NUMBER_OF_DECIMALS);
        }

        std::string convertNanoTime(double nanoTime) const {
            std::string result;
            return convertNanoTime(nanoTime, result);
        }

        std::string& convertNanoTime(double nanoTime, std::string& result) const {
            if (nanoTime >= 1000000000L) {
                double seconds = round(nanoTime / 1000000000.0);
                result += std::to_string(seconds) + (seconds > 1 ? " secs" : " sec");
            } else if (nanoTime >= 1000000L) {
                double millis = round(nanoTime / 1000000.0);
                result += std::to_string(millis) + (millis > 1 ? " millis" : " milli");
            } else if (nanoTime >= 1000L) {
                double micros = round(nanoTime / 1000.0);
                result += std::to_string(micros) + (micros > 1 ? " micros" : " micro");
            } else {
                double nanos = round(nanoTime);
                result += std::to_string(nanos) + (nanos > 1 ? " nanos" : " nano");
            }
            return result;
        }

        std::string formatter(int value) const {
            std::stringstream ss;
            ss.imbue(std::locale(""));
            ss << std::fixed << value;
            return ss.str();
        }

        void addPercentile(std::string& result, double perc, std::map<long, int>& treeMap) {
            
            if (treeMap.empty()) {
                return;
            }

            tempList.clear();
            double stdevTop = -1;

            long maxTop = -1;
            long minBottom = -1;

            long x = std::round(perc * size);
            auto iter = treeMap.begin();
            int iTop = 0;
            int iBottom = 0;
            long sumTop = 0;
            long sumBottom = 0;
            bool trueForTopFalseForBottom = true;
            while (iter != treeMap.end()) {
                const long time = iter->first;
                const int count = iter->second;
                for (int a = 0; a < count; a++) {
                    if (trueForTopFalseForBottom) {
                        iTop++;
                        sumTop += time;
                        addTempTime(time);
                        if (iTop == x) {
                            maxTop = time;

                            if (INCLUDE_STDEV) {
                                double avg = static_cast<double>(sumTop) / static_cast<double>(iTop);
                                long sum = 0;
                                auto iter2 = tempList.begin();
                                while (iter2 != tempList.end()) {
                                    long t = *iter2;
                                    sum += (avg - t) * (avg - t);
                                    ++iter2;
                                }
                                double stdev = std::sqrt(static_cast<double>(sum) / static_cast<double>(tempList.size()));
                                double rounded = round(stdev, 2);
                                stdevTop = rounded;
                            }

                            if (INCLUDE_WORST_PERCS) {
                                trueForTopFalseForBottom = false;
                                tempList.clear();
                            } else {
                                goto END_LOOP;
                            }
                        }
                    } else {
                        iBottom++;
                        sumBottom += time;
                        if (minBottom == -1) {
                            minBottom = time;
                        }
                        addTempTime(time);
                    }
                }
                ++iter;
            }
            END_LOOP:

            result += " | " + formatPercentage(perc, 8);
            if (INCLUDE_TOTALS) result += " (" + formatter(iTop) + ")";
            result += " = [avg: " + convertNanoTime(sumTop / iTop);
            if (INCLUDE_STDEV) {
                result += ", stdev: " + std::to_string(stdevTop) + " nanos";
            }
            result += ", max: " + convertNanoTime(maxTop) + ']';
            if (INCLUDE_WORST_PERCS) {
                result += " - " + formatPercentage(1 - perc, 8);
                if (INCLUDE_TOTALS) result += " (" + (iBottom > 0 ? formatter(iBottom) : "0") + ")";
                result += " = [avg: " + (iBottom > 0 ? convertNanoTime(sumBottom / iBottom) : "?");
                if (INCLUDE_STDEV) {
                    result += ", stdev: ";
                    if (iBottom > 0) {
                        double avg = static_cast<double>(sumBottom) / static_cast<double>(iBottom);
                        long sum = 0;
                        auto iter2 = tempList.begin();
                        while (iter2 != tempList.end()) {
                            long t = *iter2;
                            sum += (avg - t) * (avg - t);
                            ++iter2;
                        }
                        double stdevBottom = std::sqrt(static_cast<double>(sum) / static_cast<double>(tempList.size()));
                        double rounded = round(stdevBottom, 2);
                        result += std::to_string(rounded) + " nanos";
                    } else {
                        result += "?";
                    }
                }
                result += ", min: " + (minBottom != -1 ? convertNanoTime(minBottom) : "?") + ']';
            }
        }

        std::string resultsStr() {
            std::string result;
            const long realCount = count - warmup;
            result += "Iterations: " + formatter(realCount) + " | Warm-Up: " + formatter(warmup) + " | Avg Time: " +
                    convertNanoTime(avg());
            if (realCount > 0) {
                result += " | Min Time: " + convertNanoTime(minTime) + " | Max Time: " + convertNanoTime(maxTime);
            }

            std::map<long, int> treeMap(results.begin(), results.end());

            addPercentile(result, 0.75, treeMap);
            addPercentile(result, 0.9, treeMap);
            addPercentile(result, 0.99, treeMap);
            addPercentile(result, 0.999, treeMap);
            addPercentile(result, 0.9999, treeMap);
            addPercentile(result, 0.99999, treeMap);
            if (INCLUDE_MORE_PERCS) {
                addPercentile(result, 0.999999, treeMap);
                addPercentile(result, 0.9999999, treeMap);
            }

            return result;
        }

        std::string formatPercentage(double x, int decimals) const {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(decimals) << x;
            return ss.str();
        }

        void addTempTime(long time) {
            tempList.push_back(time);
        }
    };
}

using namespace CoralBlocks::CoralBench::Util;

int main() {
    const int warmupIterations = 1000000;
    const int measurementIterations = 1000000;

    Bench bench(warmupIterations);

    while (bench.getCount() < measurementIterations + warmupIterations) {
        Bench::doSleep(bench);
    }

    bench.printResults();

    bench.reset(false); // false because we don't want to repeat warmup

    while (bench.getCount() < measurementIterations) {
        Bench::doSleep(bench);
    }

    bench.printResults();

    return 0;
}

