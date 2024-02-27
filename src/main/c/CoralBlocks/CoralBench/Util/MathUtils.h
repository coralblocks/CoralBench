#pragma once

namespace CoralBlocks::CoralBench::Util {

    class MathUtils {
    public:
        static bool isPowerOfTwo(long long l);
        static void ensurePowerOfTwo(long long number);
    };
    
}