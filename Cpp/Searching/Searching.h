#ifndef SEARCHING_H
#define SEARCHING_H

#include <iostream>
#include <cstring>

/**
 * @file Searching.h
 * @brief LogicCraft Procedural Searching Algorithms (CO4 - Searching)
 *
 * ACADEMIC RESTRICTION COMPLIANCE:
 * Pure procedural C++ implementation using C-style structs, arrays, pointers,
 * and standalone functions. No classes, inheritance, or OOP abstractions.
 *
 * Features:
 * - Linear Search by Component ID and Component Name
 * - Binary Search by Component ID and Component Name
 * - Comparison counters for empirical performance analysis
 * - Input validation and boundary / not-found handling
 */

struct ComponentRecord {
    int id;
    char type[16];   // e.g. "AND", "OR", "NOT", "XOR", "NAND", "NOR"
    char name[32];   // e.g. "Primary_AND", "Carry_Gate"
    int inputCount;
    int output;
};

// Input validation functions
bool isValidComponentId(int id);
bool isValidComponentName(const char* name);

// Linear Search (O(N) Time Complexity)
int linearSearchById(const ComponentRecord items[], int size, int targetId, int* comparisonsOut);
int linearSearchByName(const ComponentRecord items[], int size, const char* targetName, int* comparisonsOut);

// Binary Search (O(log N) Time Complexity - requires array to be sorted by search key)
int binarySearchById(const ComponentRecord items[], int size, int targetId, int* comparisonsOut);
int binarySearchByName(const ComponentRecord items[], int size, const char* targetName, int* comparisonsOut);

// Utility: Display search outcome and metrics
void displaySearchResult(int foundIndex, const ComponentRecord items[], int comparisons, const char* searchMethod);

#endif // SEARCHING_H
