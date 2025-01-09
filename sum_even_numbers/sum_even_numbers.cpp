#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

const int ARRAY_SIZE = 100;
const int NUM_THREADS = 4;

std::vector<int> array(ARRAY_SIZE);
std::mutex mtx;
int globalSum = 0;

void generateArray() {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = i + 1;
    }
}

void calculateSum(int start, int end) {
    int localSum = 0;
    for (int i = start; i < end; ++i) {
        if (array[i] % 2 == 0) {
            localSum += array[i];
        }
    }
    // protecting Access with mutex
    std::lock_guard<std::mutex> lock(mtx);
    globalSum += localSum;
}

int main() {
    try {
        generateArray();
        std::vector<std::thread> threads;
        int chunkSize = ARRAY_SIZE / NUM_THREADS;

        for (int i = 0; i < NUM_THREADS; ++i) {
            int start = i * chunkSize;
            int end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : start + chunkSize;
            threads.emplace_back(calculateSum, start, end);
        }

        // waiting for all threads to complete
        for (auto &t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }

        std::cout << "Total sum of even numbers: " << globalSum << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occurred." << std::endl;
    }

    return 0;
}
