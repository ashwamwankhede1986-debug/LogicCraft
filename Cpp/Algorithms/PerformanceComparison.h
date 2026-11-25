#ifndef PERFORMANCE_COMPARISON_H
#define PERFORMANCE_COMPARISON_H

#include "../Searching/Searching.h"
#include "../Sorting/Sorting.h"

/**
 * @file PerformanceComparison.h
 * @brief Empirical Performance Benchmarking Suite (CO4 & CO5 - Algorithms Comparison)
 *
 * ACADEMIC RESTRICTION COMPLIANCE:
 * Pure procedural C++ implementation using C-style structs, arrays, pointers,
 * and standalone functions. No classes, inheritance, or OOP abstractions.
 *
 * Features:
 * - Systematic comparison of Linear Search vs Binary Search
 * - Comparison of Bubble Sort vs Selection Sort across dataset sizes (8, 16, 32)
 * - Evaluation under Best Case, Average Case, and Worst Case input distributions
 * - Formatted tabular metric outputs for viva demonstration
 */

struct SearchBenchmarkRow {
    int datasetSize;
    const char* scenario; // "Best Case", "Average Case", "Worst Case (Not Found)"
    int targetId;
    int linearComparisons;
    int binaryComparisons;
};

struct SortBenchmarkRow {
    int datasetSize;
    const char* distribution; // "Already Sorted", "Reverse Sorted", "Random Order"
    int bubbleComparisons;
    int bubbleSwaps;
    int selectionComparisons;
    int selectionSwaps;
};

// Procedural functions to populate synthetic benchmark datasets
void generateSyntheticComponents(ComponentRecord dataset[], int size, int patternType);

// Run Search Performance Comparison
void runSearchComparisonSuite();

// Run Sort Performance Comparison
void runSortComparisonSuite();

// Master Benchmark Runner
void runAlgorithmPerformanceBenchmarks();

#endif // PERFORMANCE_COMPARISON_H
