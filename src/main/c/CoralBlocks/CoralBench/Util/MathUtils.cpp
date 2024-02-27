#include "MathUtils.h"
#include <stdexcept>
#include <string>

using namespace std;

namespace CoralBlocks::CoralBench::Util {
    
    bool MathUtils::isPowerOfTwo(long long l) {
        return (l & (l - 1)) == 0;
    }

    void MathUtils::ensurePowerOfTwo(long long number) {
        if (!isPowerOfTwo(number)) {
            throw invalid_argument("Not a power of two: " + to_string(number));
        }
    }
}