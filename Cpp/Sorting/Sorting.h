#ifndef SORTING_H
#define SORTING_H

#include "../Searching/Searching.h"

/**
 * @file Sorting.h
 * @brief LogicCraft Procedural Sorting Algorithms (CO4 - Sorting)
 *
 * ACADEMIC RESTRICTION COMPLIANCE:
 * Pure procedural C++ implementation using C-style structs, arrays, pointers,
 * and standalone functions. No classes, inheritance, or OOP abstractions.
 *
 * Features:
 * - Bubble Sort and Selection Sort
 * - Multi-criteria sorting: by Component ID, by Component Name, and by Gate Type
 * - Comparison and swap instrumentation for empirical complexity evaluation
 * - Formatted tabular display of sorted component datasets
 */

enum SortKey {
    SORT_BY_ID = 1,
    SORT_BY_NAME = 2,
    SORT_BY_TYPE = 3
};

struct SortMetrics {
    int comparisons;
    int swaps;
};

// Procedural Bubble Sort
void bubbleSort(ComponentRecord items[], int size, SortKey key, SortMetrics* metricsOut);

// Procedural Selection Sort
void selectionSort(ComponentRecord items[], int size, SortKey key, SortMetrics* metricsOut);

// Procedural Insertion Sort (O(N^2) Time Complexity, optimal for small datasets)
void insertionSort(ComponentRecord items[], int size, SortKey key, SortMetrics* metricsOut);

// Procedural Quick Sort (O(N log N) average Time Complexity)
void quickSort(ComponentRecord items[], int size, SortKey key, SortMetrics* metricsOut);

// Procedural Merge Sort (O(N log N) worst-case Time Complexity, stable)
void mergeSort(ComponentRecord items[], int size, SortKey key, SortMetrics* metricsOut);

// Display array of component records
void displayComponentArray(const ComponentRecord items[], int size, const char* title);

// Helper to copy component arrays
void copyComponentArray(const ComponentRecord source[], ComponentRecord destination[], int size);

#endif // SORTING_H
