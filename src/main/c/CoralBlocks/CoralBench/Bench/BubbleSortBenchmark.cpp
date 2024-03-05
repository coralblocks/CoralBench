#include "../Bench.cpp"

using namespace CoralBlocks::CoralBench;
using namespace std;

void swapping(int* array, int x, int y) {
    int temp = array[x];
    array[x] = array[y];
    array[y] = temp;
}

void bubbleSort(int* array, int size) {
    for (int i = 0; i < size; i++) {
        int swaps = 0; // flag to detect any swap is there or not
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) { // when the current item is bigger than next
                swapping(array, j, j + 1);
                swaps = 1;
            }
        }
        if (swaps == 0) break; // No swap in this pass, so array is sorted
    }
}

void doSomething(int* array, int size) {

    for (int z = 0; z < size; z++) {
        array[z] = size - z;
    }

    bubbleSort(array, size);
}

int main(int argc, char* argv[]) {

    const int warmupIterations = 1000000;
    const int measurementIterations = 10000000;
    const int arraySize = 60;

    Bench& bench = *(new Bench(warmupIterations)); // don't want to use -> to dereference

    long x = 0;

    int* array = new int[arraySize];

    for (int i = 0; i < measurementIterations + warmupIterations; i++) {
        bench.mark();
        doSomething(array, arraySize);
        bench.measure();

        for (int j = 0; j < arraySize; j++) {
            x += array[j];
        }
    }

    cout << "Value computed: " << x << endl;
    cout << "Array: [";
    for (int j = 0; j < arraySize; j++) {
        cout << array[j] << (j < arraySize - 1 ? ", " : "");
    }
    cout << "]" << endl;

    bench.printResults();

    delete[] array;
    delete &bench;

    return 0;
}
