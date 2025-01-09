#include <iostream>
#include <vector>
#include <future>
#include <exception>

const int ARRAY_SIZE = 100;
const int NUM_THREADS = 4;

std::vector<int> array(ARRAY_SIZE);

void generateArray() {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = i + 1;
    }
}

int sumWithFutures(int start, int end) {
    try {
        int localSum = 0;
        for (int i = start; i < end; ++i) {
            if (array[i] % 2 == 0) {
                localSum += array[i];
            }
        }
        return localSum;
    } catch (...) {
        std::cerr << "Unknown exception in thread." << std::endl;
        return 0;
    }
}


int main() {
    try {
        generateArray();

        std::vector<std::future<int>> futures;
        int chunkSize = ARRAY_SIZE / NUM_THREADS;

        for (int i = 0; i < NUM_THREADS; ++i) {
            int start = i * chunkSize;
            int end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : start + chunkSize;
            futures.push_back(std::async(std::launch::async, sumWithFutures, start, end));
        }

        // getting results from futures
        int totalSum = 0;
        for (auto &f : futures) {
            totalSum += f.get();
        }

        std::cout << "Total sum of even numbers (using futures): " << totalSum << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in main." << std::endl;
    }

    return 0;
}
