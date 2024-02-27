#include <iostream>
#include "CoralBlocks/CoralBench/Util/MathUtils.h"

using std::cout;
using std::cerr;
using std::endl;

using namespace CoralBlocks::CoralBench::Util;

int main() {

    try {
        MathUtils::ensurePowerOfTwo(16);
        cout << "Number is a power of two." << endl;
    } catch (const std::invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}