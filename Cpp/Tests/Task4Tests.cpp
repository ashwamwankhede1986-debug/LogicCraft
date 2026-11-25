#include "Task4Tests.h"
#include "../Searching/Searching.h"
#include "../Sorting/Sorting.h"
#include "../Hashing/HashTable.h"
#include "../Algorithms/PerformanceComparison.h"
#include "../CircuitEngine/CircuitValidator.h"

#include <iostream>
#include <cassert>
#include <cstring>

void testSearchingAlgorithms() {
    std::cout << "[TEST 1/6] Running Searching Algorithms (CO4: Linear & Binary Search) ...\n";

    ComponentRecord dataset[5] = {
        {10, "AND", "Gate_Alpha", 2, 0},
        {20, "OR",  "Gate_Beta",  2, 1},
        {30, "XOR", "Gate_Gamma", 2, 1},
        {40, "NOT", "Gate_Delta", 1, 0},
        {50, "NAND","Gate_Epsilon",2, 1}
    };

    // 1. Linear Search by ID
    int comps = 0;
    int idx = linearSearchById(dataset, 5, 30, &comps);
    assert(idx == 2);
    assert(comps == 3);

    // 2. Linear Search by Name
    idx = linearSearchByName(dataset, 5, "Gate_Delta", &comps);
    assert(idx == 3);
    assert(comps == 4);

    // 3. Linear Search Not Found & Boundary
    idx = linearSearchById(dataset, 5, 999, &comps);
    assert(idx == -1);
    assert(comps == 5);

    idx = linearSearchById(dataset, 5, -5, &comps); // Invalid negative ID
    assert(idx == -1);

    // 4. Binary Search by ID
    idx = binarySearchById(dataset, 5, 10, &comps);
    assert(idx == 0);
    idx = binarySearchById(dataset, 5, 50, &comps);
    assert(idx == 4);
    idx = binarySearchById(dataset, 5, 30, &comps);
    assert(idx == 2);
    assert(comps == 1); // Exact middle found in 1 comparison!

    idx = binarySearchById(dataset, 5, 999, &comps);
    assert(idx == -1);

    std::cout << " -> Searching Algorithms PASS!\n";
}

void testSortingAlgorithms() {
    std::cout << "[TEST 2/6] Running Sorting Algorithms (CO4: Bubble & Selection Sort) ...\n";

    ComponentRecord unsorted[4] = {
        {40, "NOT", "Zeta_Gate",  1, 0},
        {10, "AND", "Alpha_Gate", 2, 1},
        {30, "XOR", "Omega_Gate", 2, 0},
        {20, "OR",  "Beta_Gate",  2, 1}
    };

    // 1. Bubble Sort by ID
    ComponentRecord arr1[4];
    copyComponentArray(unsorted, arr1, 4);
    SortMetrics m1 = {0, 0};
    bubbleSort(arr1, 4, SORT_BY_ID, &m1);
    assert(arr1[0].id == 10);
    assert(arr1[1].id == 20);
    assert(arr1[2].id == 30);
    assert(arr1[3].id == 40);
    assert(m1.swaps > 0);

    // 2. Selection Sort by Name
    ComponentRecord arr2[4];
    copyComponentArray(unsorted, arr2, 4);
    SortMetrics m2 = {0, 0};
    selectionSort(arr2, 4, SORT_BY_NAME, &m2);
    assert(std::strcmp(arr2[0].name, "Alpha_Gate") == 0);
    assert(std::strcmp(arr2[1].name, "Beta_Gate") == 0);
    assert(std::strcmp(arr2[2].name, "Omega_Gate") == 0);
    assert(std::strcmp(arr2[3].name, "Zeta_Gate") == 0);

    // 3. Early termination on already sorted array in Bubble Sort
    SortMetrics mSorted = {0, 0};
    bubbleSort(arr1, 4, SORT_BY_ID, &mSorted);
    assert(mSorted.swaps == 0);
    assert(mSorted.comparisons == 3); // Single pass N-1 comparisons

    std::cout << " -> Sorting Algorithms PASS!\n";
}

void testHashTableLinearProbing() {
    std::cout << "[TEST 3/6] Running Hash Table & Linear Probing (CO5: Hashing) ...\n";

    ComponentHashTable ht;
    initHashTable(&ht);
    assert(isHashTableEmpty(&ht) == true);

    // Intentionally create collision: capacity is 17
    // ID 3 hashes to 3 % 17 = 3
    // ID 20 hashes to 20 % 17 = 3 (Collision with ID 3!)
    ComponentRecord r1 = {3,  "AND", "Gate_Three",  2, 1};
    ComponentRecord r2 = {20, "OR",  "Gate_Twenty", 2, 0};
    ComponentRecord r3 = {37, "NOT", "Gate_ThirtySeven", 1, 1}; // 37 % 17 = 3 (Triple collision!)

    int probe1 = 0, probe2 = 0, probe3 = 0;
    assert(insertHashTable(&ht, &r1, &probe1) == true);
    assert(probe1 == 0); // Directly in bucket 3

    assert(insertHashTable(&ht, &r2, &probe2) == true);
    assert(probe2 == 1); // Linear probed to bucket 4!
    assert(ht.totalCollisions == 1);

    assert(insertHashTable(&ht, &r3, &probe3) == true);
    assert(probe3 == 2); // Linear probed to bucket 5!
    assert(ht.totalCollisions == 2);

    // Verify lookup through probe chain
    ComponentRecord found;
    int searchProbes = 0;
    int slot = searchHashTableById(&ht, 20, &found, &searchProbes);
    assert(slot == 4);
    assert(found.id == 20);
    assert(std::strcmp(found.name, "Gate_Twenty") == 0);

    slot = searchHashTableById(&ht, 37, &found, &searchProbes);
    assert(slot == 5);
    assert(found.id == 37);

    // Test Deletion with Tombstone (deleting middle element ID 20 should NOT break search for ID 37)
    assert(deleteHashTableById(&ht, 20) == true);
    assert(ht.table[4].state == SLOT_DELETED);

    // Search for ID 37 must still succeed across the deleted tombstone!
    slot = searchHashTableById(&ht, 37, &found, &searchProbes);
    assert(slot == 5);
    assert(found.id == 37);

    // Test Boundary: Fill table completely
    clearHashTable(&ht);
    for (int i = 1; i <= HASH_TABLE_CAPACITY; ++i) {
        ComponentRecord r = {i, "AND", "FillGate", 2, 0};
        assert(insertHashTable(&ht, &r, nullptr) == true);
    }
    assert(isHashTableFull(&ht) == true);

    // Boundary: 18th insert on full table must safely reject without infinite loop
    ComponentRecord overflowRecord = {999, "XOR", "OverflowGate", 2, 1};
    assert(insertHashTable(&ht, &overflowRecord, nullptr) == false);

    std::cout << " -> Hash Table & Linear Probing PASS!\n";
}

void testAlgorithmPerformanceSuite() {
    std::cout << "[TEST 4/6] Running Performance Benchmarks (Empirical Metrics) ...\n";
    ComponentRecord dataset[16];
    generateSyntheticComponents(dataset, 16, 0);

    int linComps = 0, binComps = 0;
    linearSearchById(dataset, 16, dataset[15].id, &linComps);
    binarySearchById(dataset, 16, dataset[15].id, &binComps);
    assert(linComps == 16);
    assert(binComps <= 5); // log2(16) + 1 = 5

    std::cout << " -> Performance Benchmarks PASS!\n";
}

void testCircuitValidatorDecisions() {
    std::cout << "[TEST 5/6] Running Smart Circuit Validation & Recommendation (CO4 Decision-Making) ...\n";

    // Scenario 1: Valid circuit
    Circuit validCircuit("Valid Circuit");
    Gate g1(1, "AND", "Valid_AND", 2, (int[]){1, 0});
    validCircuit.addGate(g1);
    ValidationReport r1 = validateCircuit(&validCircuit);
    assert(r1.status == CIRCUIT_VALID || r1.status == CIRCUIT_WARNING);
    assert(r1.canEvaluate == true);

    // Scenario 2: Circuit with invalid self-loop and dangling connection
    Circuit invalidCircuit("Invalid Circuit");
    Gate gBad(10, "AND", "Bad_AND", 2, (int[]){1, 1});
    invalidCircuit.addGate(gBad);
    invalidCircuit.addConnection(10, 10, 0); // Self loop
    ValidationReport r2 = validateCircuit(&invalidCircuit);
    assert(r2.status == CIRCUIT_INVALID);
    assert(r2.canEvaluate == false);
    assert(r2.errorCount >= 1);

    std::cout << " -> Smart Circuit Validation & Decision-Making PASS!\n";
}

void testIntegratedPipeline() {
    std::cout << "[TEST 6/6] Running Full Pipeline Integration (Stack + Queue + Hashing + Validation) ...\n";

    Circuit pipelineCircuit("Master Integrated Pipeline");
    CircuitUndoStack undoStack;
    initUndoStack(&undoStack);
    SimulationQueue simQueue;
    initSimulationQueue(&simQueue);
    ComponentHashTable ht;
    initHashTable(&ht);

    Gate gA(1, "AND", "Input_Stage_AND", 2, (int[]){1, 1});
    Gate gB(2, "NOT", "Output_Stage_NOT", 1, (int[]){0});

    executeAddGateWithUndo(&pipelineCircuit, &undoStack, gA);
    executeAddGateWithUndo(&pipelineCircuit, &undoStack, gB);
    executeAddConnectionWithUndo(&pipelineCircuit, &undoStack, 1, 2, 0);

    // Hash table registry insertion
    ComponentRecord recA = {1, "AND", "Input_Stage_AND", 2, 1};
    ComponentRecord recB = {2, "NOT", "Output_Stage_NOT", 1, 0};
    insertHashTable(&ht, &recA, nullptr);
    insertHashTable(&ht, &recB, nullptr);

    // Validate
    ValidationReport vr = validateCircuit(&pipelineCircuit);
    assert(vr.canEvaluate == true);

    // Simulate via Queue
    executeSimulationWithQueue(&pipelineCircuit, &simQueue);
    assert(pipelineCircuit.getGateRegistry().getGateAt(1)->getOutput() == 0); // 1 AND 1 = 1, NOT 1 = 0

    // Revert via Undo Stack
    assert(executeUndoOperation(&pipelineCircuit, &undoStack) == true); // Reverts wire
    assert(pipelineCircuit.getConnectionCount() == 0);

    std::cout << " -> Full Pipeline Integration PASS!\n";
}

void runAllTask4Tests() {
    std::cout << "\n======================================================\n";
    std::cout << "      LOGICCRAFT TASK 4 VERIFICATION TEST SUITE       \n";
    std::cout << "======================================================\n";
    testSearchingAlgorithms();
    testSortingAlgorithms();
    testHashTableLinearProbing();
    testAlgorithmPerformanceSuite();
    testCircuitValidatorDecisions();
    testIntegratedPipeline();
    std::cout << "======================================================\n";
    std::cout << " ALL TASK 4 C++ TESTS PASSED SUCCESSFULLY! (CO4 & CO5)\n";
    std::cout << "======================================================\n";
}
