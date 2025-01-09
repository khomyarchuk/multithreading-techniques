#include <iostream>
#include <vector>
#include <thread>
#include <semaphore.h>
#include <exception>

std::vector<int> A1 = {1, 2};
std::vector<int> A2 = {3, 4};
std::vector<int> A3 = {5, 6};
std::vector<int> A4 = {7, 8};


std::vector<std::vector<int>> crossSet;

// POSIX semaphore
sem_t sem;

void computeCombinationWithSemaphore(const std::vector<int>& A1, const std::vector<int>& A2,
                                     const std::vector<int>& A3, const std::vector<int>& A4,
                                     int start, int end) {
    try {
        std::vector<std::vector<int>> localCombination;

        for (int i = start; i < end; ++i) {
            for (int j = 0; j < A2.size(); ++j) {
                for (int k = 0; k < A3.size(); ++k) {
                    for (int l = 0; l < A4.size(); ++l) {
                        localCombination.push_back({A1[i], A2[j], A3[k], A4[l]});
                    }
                }
            }
        }

        // sync with semaphore
        sem_wait(&sem);
        crossSet.insert(crossSet.end(), localCombination.begin(), localCombination.end());
        // releasing a semaphore
        sem_post(&sem);
    } catch (const std::exception& e) {
        std::cerr << "Exception in computeCombinationWithSemaphore: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in computeCombinationWithSemaphore." << std::endl;
    }
}

int main() {
    try {
        // semaphore init
        sem_init(&sem, 0, 1);

        int numThreads = 2;
        // range size for each thread
        int chunkSize = A1.size() / numThreads;
        std::vector<std::thread> threads;

        // threads init
        for (int i = 0; i < numThreads; ++i) {
            int start = i * chunkSize;
            int end = (i == numThreads - 1) ? A1.size() : start + chunkSize;
            threads.emplace_back(computeCombinationWithSemaphore, std::ref(A1), std::ref(A2), std::ref(A3), std::ref(A4), start, end);
        }

        // waiting for all threads to complete
        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }

        for (const auto& combination : crossSet) {
            for (int num : combination) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }

        // semaphore destroy
        sem_destroy(&sem);
    } catch (const std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in main." << std::endl;
    }

    return 0;
}

