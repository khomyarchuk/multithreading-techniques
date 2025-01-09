#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

// Sets
std::vector<int> A1 = {1, 2};
std::vector<int> A2 = {3, 4};
std::vector<int> A3 = {5, 6};
std::vector<int> A4 = {7, 8};


std::vector<std::vector<int>> crossSet;
std::mutex mtx;

void calculateCrossSet(const std::vector<int>& A1, const std::vector<int>& A2,
                               const std::vector<int>& A3, const std::vector<int>& A4,
                               int start, int end) {
    std::vector<std::vector<int>> localSet;

    for (int i = start; i < end; ++i) {
        for (int j = 0; j < A2.size(); ++j) {
            for (int k = 0; k < A3.size(); ++k) {
                for (int l = 0; l < A4.size(); ++l) {
                    localSet.push_back({A1[i], A2[j], A3[k], A4[l]});
                }
            }
        }
    }

    // sync access
    std::lock_guard<std::mutex> lock(mtx);
    crossSet.insert(crossSet.end(), localSet.begin(), localSet.end());
}

int main() {
    try {
        int numThreads = 2;
        int chunkSize = A1.size() / numThreads;
        std::vector<std::thread> threads;

        for (int i = 0; i < numThreads; ++i) {
            int start = i * chunkSize;
            int end = (i == numThreads - 1) ? A1.size() : start + chunkSize;
            threads.emplace_back(calculateCrossSet, std::ref(A1), std::ref(A2), std::ref(A3), std::ref(A4), start, end);
        }

        for (auto& t : threads) {
            t.join();
        }

        for (const auto& combination : crossSet) {
            for (int num : combination) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
