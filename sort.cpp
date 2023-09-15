#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>
#include <sstream>
#include <sys/sysinfo.h>

void sortAndSave(std::vector<int>& numbers, int runIndex) {
    std::sort(numbers.begin(), numbers.end());
    std::ofstream output("run" + std::to_string(runIndex), std::ios::binary);
    for (int num : numbers) {
        output.write(reinterpret_cast<char*>(&num), sizeof(int));
    }
}

void externalSort(const std::string &inputFile, const std::string &outputFile, size_t memorySize) {
    std::ifstream input(inputFile, std::ios::binary);
    std::vector<int> buffer(memorySize);
    int runIndex = 0;

    while (input.read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(int))) {
        sortAndSave(buffer, runIndex);
        ++runIndex;
    }

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    std::vector<std::ifstream> runs(runIndex);

    for (int i = 0; i < runIndex; ++i) {
        runs[i].open("run" + std::to_string(i), std::ios::binary);
        int num;
        runs[i].read(reinterpret_cast<char*>(&num), sizeof(int));
        pq.push({ num, i });
    }

    std::ofstream output(outputFile, std::ios::binary);
    while (!pq.empty()) {
        auto [num, i] = pq.top();
        pq.pop();

        output.write(reinterpret_cast<char*>(&num), sizeof(int));

        if (runs[i].read(reinterpret_cast<char*>(&num), sizeof(int))) {
            pq.push({ num, i });
        }
    }
}

int main() {
    // Create input binary file
    std::ofstream input("input.bin", std::ios::binary);
    for (int i = 100; i >= 0; --i) {
        input.write(reinterpret_cast<char*>(&i), sizeof(int));
    }

    // External sort
    externalSort("input.bin", "output.bin", 10);

    // Read output binary file
    std::ifstream output("output.bin", std::ios::binary);
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        size_t memorySize = info.freeram / 4; // Ustal rozmiar buforu jako ćwiartkę dostępnej pamięci RAM
        std::cout << "Using buffer size: " << memorySize << " bytes." << std::endl;
        externalSort("input.bin", "output.bin", memorySize);
    }
    // int num;
    // while (output.read(reinterpret_cast<char*>(&num), sizeof(int))) {
    //     std::cout << num << " ";
    // }
    // std::cout << std::endl;

    return 0;
}
