#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <exception>

const int ARRAY_SIZE = 100;
const int NUM_THREADS = 4;


std::vector<int> array(ARRAY_SIZE);
std::atomic<int> globalSum(0);


void generateArray() {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = i + 1;
    }
}

void sumWithAtomic(int start, int end) {
    try {
        int localSum = 0;
        for (int i = start; i < end; ++i) {
            if (array[i] % 2 == 0) {
                localSum += array[i];
            }
        }
        // atomic func addition
        globalSum.fetch_add(localSum, std::memory_order_relaxed);
    } catch (const std::exception &e) {
        std::cerr << "Exception in thread: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in thread." << std::endl;
    }
}

int main() {
    try {
        generateArray();

        // streams creation
        std::vector<std::thread> threads;
        int chunkSize = ARRAY_SIZE / NUM_THREADS;

        for (int i = 0; i < NUM_THREADS; ++i) {
            int start = i * chunkSize;
            // Last thread processes the remainder
            int end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : start + chunkSize;
            threads.emplace_back(sumWithAtomic, start, end);
        }

        // waiting for all threads to complete
        for (auto &t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }

        std::cout << "Total sum of even numbers (using atomic): " << globalSum.load() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in main." << std::endl;
    }

    return 0;
}
