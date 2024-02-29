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
using std::string;
using std::stringstream;
using std::map;
using std::fixed;
using std::setprecision;
using std::round;

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
                if (measure(elapsed)) {
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

        string getResults() {
            string result;
            int realCount = count - warmup;
            result += "Iterations: " + formatWithCommas(realCount) + " | Warm-Up: " + formatWithCommas(warmup) + " | Avg Time: " + convertNanoTime(avg());
            if (realCount > 0) {
                result += " | Min Time: " + convertNanoTime(minTime) + " | Max Time: " + convertNanoTime(maxTime);
            }

            map<long, int> treeMap(results.begin(), results.end());

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

        void printResults() {
            std::cout << getResults() << std::endl << std::endl;
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
            std::cout << std::to_string(avg) << std::endl;
            return avg;
        }

        void addPercentile(string& result, double perc, std::map<long, int>& treeMap) {
            
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
                        tempList.push_back(time);
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
                                double rounded = roundToThreeDecimals(stdev);
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
                        tempList.push_back(time);
                    }
                }
                ++iter;
            }
            END_LOOP:

            result += " | " + formatPercentage(perc, 8);
            if (INCLUDE_TOTALS) result += " (" + formatWithCommas(iTop) + ")";
            result += " = [avg: " + convertNanoTime(sumTop / iTop);
            if (INCLUDE_STDEV) {
                result += ", stdev: " + formatToThreeDecimals(stdevTop) + " nanos";
            }
            result += ", max: " + convertNanoTime(maxTop) + ']';
            if (INCLUDE_WORST_PERCS) {
                result += " - " + formatPercentage(1 - perc, 8);
                if (INCLUDE_TOTALS) result += " (" + (iBottom > 0 ? formatWithCommas(iBottom) : "0") + ")";
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
                        double rounded = roundToThreeDecimals(stdevBottom);
                        result += formatToThreeDecimals(rounded) + " nanos";
                    } else {
                        result += "?";
                    }
                }
                result += ", min: " + (minBottom != -1 ? convertNanoTime(minBottom) : "?") + ']';
            }
        }

        string formatWithCommas(int value) const {
            stringstream ss;
            ss.imbue(std::locale("en_US")); // for #,###
            ss << std::fixed << value;
            return ss.str();
        }

        string trimTrailingZeros(const string& input) {

            size_t dotPosition = input.find('.');
            
            if (dotPosition != std::string::npos) {
                size_t lastNonZero = input.find_last_not_of('0');
                
                if (lastNonZero != std::string::npos && lastNonZero > dotPosition) {
                    return input.substr(0, lastNonZero + 1);
                } else {
                    return input.substr(0, dotPosition);
                }
            }
            
            return input;
        }

        string formatPercentage(double x, int decimals) {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(decimals) << x * 100;
            return trimTrailingZeros(ss.str()) + "%";
        }

        double roundToThreeDecimals(double d) const {
            double pow = std::pow(10, 3);
            return round(d * pow) / pow;
        }

        string formatToThreeDecimals(double value) const {
            stringstream ss;
            ss << fixed << setprecision(3);
            ss << value;
            return ss.str();
        }

        string convertNanoTime(double nanoTime) const {
            string result;
            if (nanoTime >= 1000000000L) {
                double seconds = nanoTime / 1000000000.0;
                result += formatToThreeDecimals(seconds) + (seconds > 1 ? " secs" : " sec");
            } else if (nanoTime >= 1000000L) {
                double millis = nanoTime / 1000000.0;
                result += formatToThreeDecimals(millis) + (millis > 1 ? " millis" : " milli");
            } else if (nanoTime >= 1000L) {
                double micros = nanoTime / 1000.0;
                result += formatToThreeDecimals(micros) + (micros > 1 ? " micros" : " micro");
            } else {
                double nanos = nanoTime;
                result += formatToThreeDecimals(nanos) + (nanos > 1 ? " nanos" : " nano");
            }
            return result;
        }
    };
}
