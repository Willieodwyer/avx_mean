#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <immintrin.h> 

int main() {
    std::ifstream file("mean.csv");

    if (!file.is_open()) {
        std::cerr << "file not found" << std::endl;
        exit(-1);
    }

    std::stringstream nums;
    nums << file.rdbuf();

    std::vector<double> doubles;
    uint size(0);
    std::string line;
    while (std::getline(nums, line)) {
        doubles.push_back(std::stod(line));
        ++size;
    }

    auto now = std::chrono::high_resolution_clock::now();

    int i = 0;
    double sum = 0;
#if 1
    // Process 4 doubles at a time
    __m256d sum_256_vec = _mm256_setzero_pd(); // Initialize a 256-bit vector for the sum
    for (; i + 3 < size; i += 4) {
        __m256d temp = _mm256_loadu_pd(&doubles[i]);     // Load 4 doubles
        sum_256_vec = _mm256_add_pd(sum_256_vec, temp);  // Accumulate sums
    }


    // Perform horizontal addition to sum up all elements in sum_256_vec
    __m128d low = _mm256_extractf128_pd(sum_256_vec, 0); // Lower 128 bits
    __m128d high = _mm256_extractf128_pd(sum_256_vec, 1); // Upper 128 bits
    __m128d sum128 = _mm_add_pd(low, high); // Add lower and upper parts

    // Horizontal addition (final sum)
    sum128 = _mm_hadd_pd(sum128, sum128);  // Horizontal addition (2 pairs)
    sum = _mm_cvtsd_f64(sum128);    // Extract scalar sum from the result
#endif

    // Process any remaining elements
    for (; i < size; ++i) {
        sum += doubles[i];
    }

    // Compute the mean
    double mean = sum / (double) size;
    auto then = std::chrono::high_resolution_clock::now();

    std::cout << "Total: " << sum << "\n";
    std::cout << "Size:  " << size << "\n";
    std::cout << "Mean:  " << std::setprecision(10) << mean  << "\n";
    std::cout << "Time:  " << std::chrono::duration_cast<std::chrono::microseconds>((then - now)).count() << "ns\n";
    return 0;
}
