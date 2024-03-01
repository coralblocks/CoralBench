#include <iostream>
#include "CoralBlocks/CoralBench/Util/Random.cpp"

using std::cout;
using std::endl;

using namespace CoralBlocks::CoralBench::Util;

int main() {
    long sameSeed = 16238935L;

    Random r1(sameSeed);
    Random r2(sameSeed);

    for (int i = 0; i < 10; i++) {
        std::cout << r1.nextLong() << " " << r2.nextLong() << std::endl;
    }

    std::cout << std::endl;

    for (int i = 0; i < 10; i++) {
        std::cout << r1.nextLong(100000) << " " << r2.nextLong(100000) << std::endl;
    }

    std::cout << std::endl;

    for (int i = 0; i < 10; i++) {
        std::cout << r1.nextInt() << " " << r2.nextInt() << std::endl;
    }

    std::cout << std::endl;

    for (int i = 0; i < 10; i++) {
        std::cout << r1.nextInt(100000) << " " << r2.nextInt(100000) << std::endl;
    }

    std::cout << std::endl;

    return 0;
}
