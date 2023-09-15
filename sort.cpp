#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

int main() {
    size_t array_size = 2000000;  
    std::vector<double> large_array(array_size);

    // Wypełnianie tablicy losowymi wartościami
    for(size_t i = 0; i < array_size; ++i) {
        large_array[i] = static_cast<double>(rand()) / RAND_MAX;  // Losowe wartości typu double w zakresie [0, 1)
    }

    // Pomiar czasu sortowania
    // auto start = std::chrono::high_resolution_clock::now();
    std::sort(large_array.begin(), large_array.end());
    // auto stop = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Sorting end";

    return 0;
}
