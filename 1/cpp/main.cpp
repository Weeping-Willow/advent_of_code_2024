#include <fstream>
#include <iostream>
#include <algorithm>


std::string getValueByDelimeter(std::string &line, const std::string &delim) {
    size_t pos = line.find(delim);
    if (pos == std::string::npos) {
        return line;
    }

    if (pos == 0) {
        line.erase(0, delim.length());
        return getValueByDelimeter(line, delim);
    }

    std::string value = line.substr(0, pos);
    line.erase(0, pos + delim.length());

    return value;
}

void quicksort(int *arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        int pi = i + 1;

        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int main() {
    std::string fileContent;
    std::ifstream inputFile("input.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    auto count = std::count_if(std::istreambuf_iterator<char>{inputFile}, {}, [](char c) { return c == '\n'; })+ 1;

    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);

    int rowA[count], rowB[count];

    std::cout << "Starting to read the file... with lines: " << count << std::endl;

    int rowIndex = 0;
    while( std::getline(inputFile, fileContent)) {
        std::string rowAValue = getValueByDelimeter(fileContent, " ");
        std::string rowBValue = getValueByDelimeter(fileContent, " ");
        
        if (rowAValue.empty() || rowBValue.empty()) {
            std::cerr << "Error: Not enough columns in the input file" << std::endl;
            return 1;
        }

        int rowAConverted= std::stoi(rowAValue);
        int rowBConverted = std::stoi(rowBValue);
        if (rowAConverted <= 0 || rowBConverted <= 0) {
            std::cerr << "Error: Negative or zero values are not allowed" << std::endl;
            return 1;
        }

        rowA[rowIndex] = rowAConverted;
        rowB[rowIndex] = rowBConverted;

        rowIndex++;
    }

    quicksort(rowA, 0, count - 1);
    quicksort(rowB, 0, count - 1);

    int diff = 0;
    int similarity = 0;
    for (int i = 0; i < count; i++) {
        similarity += rowA[i] * std::count(rowB, rowB + count, rowA[i]);

        int tempDiff = rowA[i] - rowB[i];
        if (tempDiff < 0) {
            tempDiff = -tempDiff;
        }

        diff += tempDiff;
    }


    std::cout << "Similarity: " << similarity << std::endl;
    std::cout << "Difference: " << diff << std::endl;

    inputFile.close();
}