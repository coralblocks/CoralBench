#include <iostream>

using std::invalid_argument;

namespace CoralBlocks::CoralBench::Util {

    class Random {

    private:
        long seed;

    public:
        Random(long seed) : seed(seed) {}

        long nextLong() {
            seed = (seed * 1664525L + 1013904223L) & 0xFFFFFFFFL;
            long high = (seed >> 16) & 0xFFFFFFFFL;
            seed = (seed * 1664525L + 1013904223L) & 0xFFFFFFFFL;
            long low = (seed >> 16) & 0xFFFFFFFFL;
            return (high << 32) | low;
        }

        long nextLong(long bound) {
            if (bound <= 0) {
                throw invalid_argument("Bound must be positive");
            }
            return nextLong() % bound;
        }

        int nextInt() {
            seed = (seed * 1664525L + 1013904223L) & 0xFFFFFFFFL;
            return static_cast<int>(seed >> 16);
        }

        int nextInt(int bound) {
            if (bound <= 0) {
                throw std::invalid_argument("Bound must be positive");
            }
            return nextInt() % bound;
        }
    };
}
