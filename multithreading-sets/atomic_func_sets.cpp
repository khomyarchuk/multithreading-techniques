#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <exception>

std::vector<int> A1 = {1, 2};
std::vector<int> A2 = {3, 4};
std::vector<int> A3 = {5, 6};
std::vector<int> A4 = {7, 8};

std::vector<std::vector<int>> crossSet;

std::atomic<int> atomicIndex(0);

std::mutex resultMutex;


void computeCrossSetWithAtomic(const std::vector<int>& A1, const std::vector<int>& A2,
                                  const std::vector<int>& A3, const std::vector<int>& A4) {
    try {
        while (true) {
            int currentIndex = atomicIndex.fetch_add(1);
            if (currentIndex >= A1.size()) {
                break; // if index is out of bound thread stops
            }

            std::vector<std::vector<int>> localCombination;

            for (int j = 0; j < A2.size(); ++j) {
                for (int k = 0; k < A3.size(); ++k) {
                    for (int l = 0; l < A4.size(); ++l) {
                        localCombination.push_back({A1[currentIndex], A2[j], A3[k], A4[l]});
                    }
                }
            }

            std::lock_guard<std::mutex> lock(resultMutex);
            crossSet.insert(crossSet.end(), localCombination.begin(), localCombination.end());
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in computeCombinationWithAtomic: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in computeCombinationWithAtomic." << std::endl;
    }
}

int main() {
    try {
        int numThreads = 4;
        std::vector<std::thread> threads;

        // threads init
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(computeCrossSetWithAtomic, std::ref(A1), std::ref(A2), std::ref(A3), std::ref(A4));
        }

        // waiting each thread to be completed
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
    } catch (const std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in main." << std::endl;
    }

    return 0;
}
