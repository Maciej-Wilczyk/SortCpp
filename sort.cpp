#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <sys/sysinfo.h>

struct Entry {
    int value;
    int run;

    bool operator>(const Entry& other) const {
        return value > other.value;
    }
};

void generateInputFile(const std::string& filename, long long elements) {
    std::ofstream file(filename, std::ios::binary);
    for (long long i = 0; i < elements; ++i) {
        int random_number = std::rand();
        file.write(reinterpret_cast<char*>(&random_number), sizeof(int));
    }
    file.close();
}

void externalSort(const std::string& inputFile, const std::string& outputFile, size_t memorySize) {
    int chunkSize = memorySize / sizeof(int);
    std::vector<int> chunk(chunkSize);

    std::ifstream input(inputFile, std::ios::binary);
    std::vector<std::string> runFiles;

    // Phase 1: Sorting
    while (input.read(reinterpret_cast<char*>(chunk.data()), chunk.size() * sizeof(int))) {
        std::sort(chunk.begin(), chunk.end());

        std::string runFile = "run_" + std::to_string(runFiles.size()) + ".bin";
        std::ofstream runOut(runFile, std::ios::binary);
        runOut.write(reinterpret_cast<char*>(chunk.data()), chunk.size() * sizeof(int));
        runOut.close();

        runFiles.push_back(runFile);
    }

    input.close();

    // Phase 2: Merging
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> minHeap;
    std::vector<std::ifstream> runInputs(runFiles.size());
    std::ofstream output(outputFile, std::ios::binary);

    for (int i = 0; i < runFiles.size(); ++i) {
        runInputs[i].open(runFiles[i], std::ios::binary);
        int val;
        if (runInputs[i].read(reinterpret_cast<char*>(&val), sizeof(int))) {
            minHeap.push(Entry{ val, i });
        }
    }

    while (!minHeap.empty()) {
        Entry entry = minHeap.top();
        minHeap.pop();

        output.write(reinterpret_cast<char*>(&entry.value), sizeof(int));

        int nextValue;
        if (runInputs[entry.run].read(reinterpret_cast<char*>(&nextValue), sizeof(int))) {
            minHeap.push(Entry{ nextValue, entry.run });
        }
    }

    output.close();

    for (auto& runInput : runInputs) {
        runInput.close();
    }
}

int main() {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        size_t memorySize = info.freeram / 4;  // Use 1/4 of free RAM for buffer
        std::cout << "Using buffer size: " << memorySize << " bytes." << std::endl;

        generateInputFile("input.bin", 2000000000LL);
        externalSort("input.bin", "output.bin", memorySize);
         if (std::remove("input.bin") != 0) {
             std::cout << "koniec";
        } else {
            std::cout << "koniec";
        }
    }

    return 0;
}
