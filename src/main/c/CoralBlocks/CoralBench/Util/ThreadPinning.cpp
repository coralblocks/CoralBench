#include <iostream>
#include <sched.h>
#include <stdexcept>

using std::runtime_error;

namespace CoralBlocks::CoralBench::Util {

    class ThreadPinning {
    public:
        static void pinCurrentThread(int proc) {
            cpu_set_t my_set;
            CPU_ZERO(&my_set);
            CPU_SET(proc, &my_set);
            if (sched_setaffinity(0, sizeof(cpu_set_t), &my_set) != 0) {
                throw new runtime_error("Cannot pin thread!");
            }
        }
    };
}