#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>
#include <iostream>

// Function to shuffle the list
std::vector<std::pair<int, int>> shuffleList(std::vector<uint8_t>& list) {
    std::vector<std::pair<int, int>> swapHistory;
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<int> dist(0, list.size() - 1);

    for (int i = list.size() - 1; i > 0; --i) {
        int j = dist(g) % (i + 1);
        swapHistory.push_back({ i, j });
        std::swap(list[i], list[j]);
    }

    return swapHistory;
}

void unshuffleList(std::vector<uint8_t>& list, const std::vector<std::pair<int, int>>& swapHistory) {
    for (auto it = swapHistory.rbegin(); it != swapHistory.rend(); ++it)
        std::swap(list[it->first], list[it->second]);
}

// Function to write a string to a file
void writeStringToFile(const std::string& str, const std::string& filename) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << str;
        outFile.close();
    }
    else { std::cerr << "Unable to open file for writing." << std::endl; }
}

int main() {
    const uint8_t dllBuffer[] = {0x4D, 0x5A, 0x90, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00};        // example of a dll buffer
    std::vector<uint8_t> list(dllBuffer, dllBuffer + sizeof(dllBuffer) / sizeof(dllBuffer[0]));

    std::ostringstream oss;

    // Shuffle the list
    std::vector<std::pair<int, int>> swapHistory = shuffleList(list);

    oss << "const uint8_t dllBuffer[] = { ";
    for (size_t i = 0; i < list.size(); ++i) {
        oss << "0x" << std::hex << static_cast<int>(i) << ", ";
        if (i < list.size() - 1) { oss << ", ";  }
    }
    oss << "\n};";

    // Write the shuffled list to a file
    writeStringToFile(oss.str(), "shuffled_list.txt");

    // Clear the stringstream for reuse
    oss.str("");
    oss.clear();

    // Unshuffle the list
    unshuffleList(list, swapHistory);

    oss << "Unshuffled list: ";
    for (uint8_t i : list) oss << "0x" << std::hex << static_cast<int>(i) << ", ";
    oss << std::endl;

    std::cout << oss.str();

    return 0;
}
