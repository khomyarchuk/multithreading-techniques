#include <iostream>
#include <vector>
#include <thread>
#include <semaphore.h>
#include <exception>

const int ARRAY_SIZE = 100;
const int NUM_THREADS = 4;

std::vector<int> array(ARRAY_SIZE);
// POSIX semaphore
sem_t sem;
int globalSum = 0;

void generateArray() {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = i + 1;
    }
}

void sumWithSemaphore(int start, int end) {
    try {
        int localSum = 0;
        for (int i = start; i < end; ++i) {
            if (array[i] % 2 == 0) {
                localSum += array[i];
            }
        }
        // protecting access semaphore
        sem_wait(&sem);
        // seizing a semaphore
        globalSum += localSum;
        // releasing a semaphore
        sem_post(&sem);
    } catch (const std::exception &e) {
        std::cerr << "Exception in thread: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in thread." << std::endl;
    }
}

int main() {
    try {
        // semaphore init
        sem_init(&sem, 0, 1);

        generateArray();

        std::vector<std::thread> threads;
        int chunkSize = ARRAY_SIZE / NUM_THREADS;

        for (int i = 0; i < NUM_THREADS; ++i) {
            int start = i * chunkSize;
            int end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : start + chunkSize;
            threads.emplace_back(sumWithSemaphore, start, end);
        }

        // waiting for all threads to complete
        for (auto &t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }

        std::cout << "Total sum of even numbers (using semaphore): " << globalSum << std::endl;

        // semaphore destroy
        sem_destroy(&sem);
    } catch (const std::exception &e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in main." << std::endl;
    }

    return 0;
}
