#include "PerformanceComparison.h"
#include <iostream>
#include <iomanip>
#include <cstdio>

// Pattern types: 0 = Sorted, 1 = Reverse Sorted, 2 = Shuffled / Alternating
void generateSyntheticComponents(ComponentRecord dataset[], int size, int patternType) {
    if (!dataset || size <= 0) return;

    const char* gateTypes[] = {"AND", "OR", "NOT", "XOR", "NAND", "NOR"};
    int numTypes = 6;

    for (int i = 0; i < size; ++i) {
        int idVal = i + 1;
        if (patternType == 1) {
            // Reverse sorted
            idVal = size - i;
        } else if (patternType == 2) {
            // Pseudo-random pseudo-shuffled pattern
            idVal = ((i * 17 + 7) % size) + 1;
        }

        dataset[i].id = idVal;
        dataset[i].inputCount = 2;
        dataset[i].output = (idVal % 2);
        std::snprintf(dataset[i].name, sizeof(dataset[i].name), "Gate_%02d", idVal);
        std::snprintf(dataset[i].type, sizeof(dataset[i].type), "%s", gateTypes[i % numTypes]);
    }
}

void runSearchComparisonSuite() {
    std::cout << "\n=================================================================\n";
    std::cout << " [PERFORMANCE BENCHMARK 1]: LINEAR SEARCH vs BINARY SEARCH (CO4)\n";
    std::cout << "=================================================================\n";
    std::cout << "Theoretical Complexity: Linear Search O(N) vs Binary Search O(log2 N)\n";
    std::cout << "+-------+-----------------------+--------+---------------+----------------+\n";
    std::cout << "| Size  | Scenario              | Target | Linear Search | Binary Search  |\n";
    std::cout << "| (N)   |                       | ID     | (Comparisons) | (Comparisons)  |\n";
    std::cout << "+-------+-----------------------+--------+---------------+----------------+\n";

    int sizes[] = {8, 16, 32};
    for (int s = 0; s < 3; ++s) {
        int n = sizes[s];
        ComponentRecord dataset[32];
        generateSyntheticComponents(dataset, n, 0); // Sorted array for fair binary search

        // Best Case: Target at middle (binary best) or front (linear best)
        int linBestComp = 0, binBestComp = 0;
        linearSearchById(dataset, n, dataset[0].id, &linBestComp); // First element
        int midIdx = (n - 1) / 2;
        binarySearchById(dataset, n, dataset[midIdx].id, &binBestComp); // Middle element

        // Average Case: Element near 3/4 position
        int avgTarget = dataset[(3 * n) / 4].id;
        int linAvgComp = 0, binAvgComp = 0;
        linearSearchById(dataset, n, avgTarget, &linAvgComp);
        binarySearchById(dataset, n, avgTarget, &binAvgComp);

        // Worst Case: Element not present
        int worstTarget = 999;
        int linWorstComp = 0, binWorstComp = 0;
        linearSearchById(dataset, n, worstTarget, &linWorstComp);
        binarySearchById(dataset, n, worstTarget, &binWorstComp);

        std::cout << "| " << std::setw(5) << n << " | " << std::setw(21) << std::left << "Best Case Target" << std::right
                  << " | " << std::setw(6) << dataset[0].id
                  << " | " << std::setw(13) << linBestComp
                  << " | " << std::setw(14) << binBestComp << " |\n";

        std::cout << "| " << std::setw(5) << n << " | " << std::setw(21) << std::left << "Average Case Target" << std::right
                  << " | " << std::setw(6) << avgTarget
                  << " | " << std::setw(13) << linAvgComp
                  << " | " << std::setw(14) << binAvgComp << " |\n";

        std::cout << "| " << std::setw(5) << n << " | " << std::setw(21) << std::left << "Worst (Not Found)" << std::right
                  << " | " << std::setw(6) << worstTarget
                  << " | " << std::setw(13) << linWorstComp
                  << " | " << std::setw(14) << binWorstComp << " |\n";
        std::cout << "+-------+-----------------------+--------+---------------+----------------+\n";
    }
}

void runSortComparisonSuite() {
    std::cout << "\n=================================================================\n";
    std::cout << " [PERFORMANCE BENCHMARK 2]: BUBBLE SORT vs SELECTION SORT (CO4)  \n";
    std::cout << "=================================================================\n";
    std::cout << "Comparing Comparisons & Data Swaps across Different Input Topologies\n";
    std::cout << "+------+-----------------+-----------------------+-----------------------+\n";
    std::cout << "| Size | Distribution    | Bubble Sort           | Selection Sort        |\n";
    std::cout << "| (N)  |                 | Comps       Swaps     | Comps       Swaps     |\n";
    std::cout << "+------+-----------------+-----------------------+-----------------------+\n";

    int sizes[] = {8, 16, 32};
    const char* distributions[] = {"Already Sorted", "Reverse Sorted", "Random Order"};

    for (int s = 0; s < 3; ++s) {
        int n = sizes[s];
        for (int d = 0; d < 3; ++d) {
            ComponentRecord original[32];
            ComponentRecord arrBubble[32];
            ComponentRecord arrSelection[32];

            generateSyntheticComponents(original, n, d);
            copyComponentArray(original, arrBubble, n);
            copyComponentArray(original, arrSelection, n);

            SortMetrics mBubble = {0, 0};
            SortMetrics mSelection = {0, 0};

            bubbleSort(arrBubble, n, SORT_BY_ID, &mBubble);
            selectionSort(arrSelection, n, SORT_BY_ID, &mSelection);

            std::cout << "| " << std::setw(4) << n << " | "
                      << std::setw(15) << std::left << distributions[d] << std::right << " | "
                      << std::setw(11) << mBubble.comparisons << " " << std::setw(9) << mBubble.swaps << " | "
                      << std::setw(11) << mSelection.comparisons << " " << std::setw(9) << mSelection.swaps << " |\n";
        }
        std::cout << "+------+-----------------+-----------------------+-----------------------+\n";
    }
}

void runAlgorithmPerformanceBenchmarks() {
    runSearchComparisonSuite();
    runSortComparisonSuite();
}
