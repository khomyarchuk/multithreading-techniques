#include <iostream>
#include <vector>
#include <future>
#include <exception>
#include <stdexcept>

std::vector<int> A1 = {1, 2};
std::vector<int> A2 = {3, 4};
std::vector<int> A3 = {5, 6};
std::vector<int> A4 = {7, 8};


std::vector<std::vector<int>> computeCombinationWithFutures(const std::vector<int>& A1, const std::vector<int>& A2,
                                                             const std::vector<int>& A3, const std::vector<int>& A4,
                                                             int start, int end) {
    std::vector<std::vector<int>> localCombination;

    try {
        for (int i = start; i < end; ++i) {
            for (int j = 0; j < A2.size(); ++j) {
                for (int k = 0; k < A3.size(); ++k) {
                    for (int l = 0; l < A4.size(); ++l) {
                        localCombination.push_back({A1[i], A2[j], A3[k], A4[l]});
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in computeCombinationWithFutures: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in computeCombinationWithFutures." << std::endl;
    }

    return localCombination;
}

int main() {
    try {
        int numThreads = 2;
        int chunkSize = A1.size() / numThreads;
        std::vector<std::future<std::vector<std::vector<int>>>> futures;

        // creating futures
        for (int i = 0; i < numThreads; ++i) {
            int start = i * chunkSize;
            int end = (i == numThreads - 1) ? A1.size() : start + chunkSize;

            // async launch
            futures.push_back(std::async(std::launch::async, computeCombinationWithFutures,
                                         std::ref(A1), std::ref(A2), std::ref(A3), std::ref(A4), start, end));
        }

        // futures results
        std::vector<std::vector<int>> cartesianProduct;
        for (auto& future : futures) {
            std::vector<std::vector<int>> result = future.get();
            cartesianProduct.insert(cartesianProduct.end(), result.begin(), result.end());
        }


        for (const auto& combination : cartesianProduct) {
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
