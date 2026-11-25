#include "Searching/Searching.h"
#include "Sorting/Sorting.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cctype>

static bool readRecords(std::vector<ComponentRecord>& records) {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, type, name, inputCount, output;
        if (!std::getline(ss, id, '|') || !std::getline(ss, type, '|') ||
            !std::getline(ss, name, '|') || !std::getline(ss, inputCount, '|') ||
            !std::getline(ss, output)) return false;
        ComponentRecord rec{};
        rec.id = std::atoi(id.c_str());
        std::snprintf(rec.type, sizeof(rec.type), "%s", type.c_str());
        std::snprintf(rec.name, sizeof(rec.name), "%s", name.c_str());
        rec.inputCount = std::atoi(inputCount.c_str());
        rec.output = std::atoi(output.c_str());
        records.push_back(rec);
    }
    return true;
}
static void printRecord(const ComponentRecord& r) {
    std::cout << r.id << '|' << r.type << '|' << r.name << '|' << r.inputCount << '|' << r.output << '\n';
}
int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage: DSABridge search <id|name> <linear|binary> <target> OR sort <id|name|type> <bubble|selection|insertion|quick|merge> <asc|desc>\n";
        return 2;
    }
    std::vector<ComponentRecord> records;
    if (!readRecords(records)) { std::cerr << "Invalid input record. Expected id|type|name|inputs|output\n"; return 2; }
    if (records.size() > 5000) { std::cerr << "Maximum record count is 5000\n"; return 2; }
    const std::string op = argv[1], key = argv[2], algorithm = argv[3], target = argv[4];
    if (op == "search") {
        int idx = -1, comps = 0;
        if (key == "id") {
            std::string digits; for (char ch : target) if (std::isdigit(static_cast<unsigned char>(ch))) digits.push_back(ch);
            int numericTarget = std::atoi(digits.c_str());
            if (algorithm == "binary") {
                std::sort(records.begin(), records.end(), [](const ComponentRecord& a, const ComponentRecord& b){ return a.id < b.id; });
                idx = binarySearchById(records.data(), (int)records.size(), numericTarget, &comps);
            } else idx = linearSearchById(records.data(), (int)records.size(), numericTarget, &comps);
        } else if (key == "name") {
            if (algorithm == "binary") {
                std::sort(records.begin(), records.end(), [](const ComponentRecord& a, const ComponentRecord& b){ return std::string(a.name) < std::string(b.name); });
                idx = binarySearchByName(records.data(), (int)records.size(), target.c_str(), &comps);
            } else idx = linearSearchByName(records.data(), (int)records.size(), target.c_str(), &comps);
        } else { std::cerr << "Invalid search key\n"; return 2; }
        std::cout << "SEARCH_RESULT|" << idx << "|" << comps << '\n';
        if (idx >= 0) printRecord(records[idx]);
        return 0;
    }
    if (op == "sort") {
        SortKey sortKey = key == "name" ? SORT_BY_NAME : key == "type" ? SORT_BY_TYPE : SORT_BY_ID;
        SortMetrics metrics{};
        if (algorithm == "bubble") bubbleSort(records.data(), (int)records.size(), sortKey, &metrics);
        else if (algorithm == "selection") selectionSort(records.data(), (int)records.size(), sortKey, &metrics);
        else if (algorithm == "insertion") insertionSort(records.data(), (int)records.size(), sortKey, &metrics);
        else if (algorithm == "quick") quickSort(records.data(), (int)records.size(), sortKey, &metrics);
        else if (algorithm == "merge") mergeSort(records.data(), (int)records.size(), sortKey, &metrics);
        else { std::cerr << "Invalid sorting algorithm\n"; return 2; }
        if (target == "desc") std::reverse(records.begin(), records.end());
        std::cout << "SORT_METRICS|" << metrics.comparisons << "|" << metrics.swaps << '\n';
        for (const auto& r : records) printRecord(r);
        return 0;
    }
    std::cerr << "Unknown operation\n";
    return 2;
}
