#include "Gates/Gate.h"
#include "DataStructures/ArrayOperations.h"
#include "DataStructures/LinkedList.h"
#include "CircuitEngine/TruthTable.h"
#include "CircuitEngine/Circuit.h"
#include "Algorithms/AlgorithmsPlaceholder.h"
#include "Searching/Searching.h"
#include "Sorting/Sorting.h"
#include "Hashing/HashTable.h"
#include "Algorithms/PerformanceComparison.h"
#include "CircuitEngine/CircuitValidator.h"

#include <iostream>
#include <string>

/**
 * @file main.cpp
 * @brief LogicCraft Review-2 Demonstration Entry Point for C++ / Programming Lab
 *
 * Subject: Programming Lab
 * Focus:
 *  - CO1: Arrays (Boolean arrays, Gate array operations, Truth Table rows)
 *  - CO2: Linked List (Dynamic circuit component node management)
 */

void printBanner() {
    std::cout << "\n";
    std::cout << "*****************************************************************\n";
    std::cout << "*     LOGICCRAFT — VISUAL LOGIC GATE SIMULATOR (REVIEW 2)       *\n";
    std::cout << "*             MODULE: C++ / PROGRAMMING LAB                     *\n";
    std::cout << "*****************************************************************\n";
    std::cout << "*  CO1: ARRAYS       -> Array Storage, CRUD Operations, Truth   *\n";
    std::cout << "*                       Table Generation                        *\n";
    std::cout << "*  CO2: LINKED LISTS -> Dynamic Circuit Nodes, Pointer Traversal*\n";
    std::cout << "*****************************************************************\n\n";
}

void demonstrateCO1_Arrays() {
    std::cout << "\n=================================================================\n";
    std::cout << " [DEMONSTRATION 1 / CO1]: ARRAY-BASED DATA STRUCTURES            \n";
    std::cout << "=================================================================\n";
    std::cout << "1. Creating GateArray to store Boolean circuit elements in memory.\n";

    GateArray registry;

    // 1. Insert Operations
    std::cout << "\n--- [CO1.1: INSERT OPERATION ON ARRAY] ---\n";
    int inA[2] = {1, 0};
    Gate g1(1, "AND", "Primary_AND_Gate", 2, inA);
    registry.insertGate(g1);

    int inB[2] = {0, 1};
    Gate g2(2, "OR", "Secondary_OR_Gate", 2, inB);
    registry.insertGate(g2);

    int inC[1] = {1};
    Gate g3(3, "NOT", "Output_Inverter", 1, inC);
    registry.insertGate(g3);

    // 2. Display All
    std::cout << "\n--- [CO1.2: DISPLAY OPERATION ON ARRAY] ---\n";
    registry.displayAll();

    // 3. Search Operations
    std::cout << "--- [CO1.3: SEARCH OPERATION ON ARRAY] ---\n";
    int searchId = 2;
    int idx = registry.searchGateById(searchId);
    if (idx != -1) {
        std::cout << "Found Gate ID " << searchId << " at array index [" << idx << "]:\n  -> ";
        registry.getGateAt(idx)->display();
    } else {
        std::cout << "Gate ID " << searchId << " not found in array.\n";
    }

    // 4. Update Operation
    std::cout << "\n--- [CO1.4: UPDATE OPERATION ON ARRAY] ---\n";
    std::cout << "Updating Gate ID 1 inputs to [1, 1] ...\n";
    int updatedInputs[2] = {1, 1};
    registry.updateGate(1, "Primary_AND_Gate_HIGH", updatedInputs, 2);

    // 5. Delete Operation
    std::cout << "\n--- [CO1.5: DELETE OPERATION ON ARRAY] ---\n";
    std::cout << "Deleting Gate ID 2 (OR Gate) from array ...\n";
    registry.deleteGate(2);

    std::cout << "\nDisplaying array after deletion (contiguous shift verified):\n";
    registry.displayAll();

    // 6. Truth Table Generation using Arrays
    std::cout << "=================================================================\n";
    std::cout << " [DEMONSTRATION 2 / CO1]: TRUTH TABLE GENERATION USING ARRAYS    \n";
    std::cout << "=================================================================\n";
    std::cout << "LogicCraft evaluates all 2^N Boolean permutations stored in arrays.\n";

    TruthTable ttAnd("AND Gate Truth Table", 2);
    ttAnd.generateFor2InputOperation("AND");
    ttAnd.display();

    TruthTable ttOr("OR Gate Truth Table", 2);
    ttOr.generateFor2InputOperation("OR");
    ttOr.display();

    TruthTable ttNot("NOT Gate Truth Table", 1);
    ttNot.generateFor2InputOperation("NOT");
    ttNot.display();
}

void demonstrateCO2_LinkedList() {
    std::cout << "\n=================================================================\n";
    std::cout << " [DEMONSTRATION 3 / CO2]: DYNAMIC CIRCUIT USING LINKED LIST      \n";
    std::cout << "=================================================================\n";
    std::cout << "ACADEMIC VIVA JUSTIFICATION:\n";
    std::cout << "\"LogicCraft allows components to be dynamically added and deleted\n";
    std::cout << "on the visual canvas at runtime. Unlike fixed arrays, a dynamic\n";
    std::cout << "singly-linked list allocates gate nodes dynamically on the heap,\n";
    std::cout << "permitting arbitrary circuit size and O(1) pointer-based removal.\"\n";
    std::cout << "-----------------------------------------------------------------\n";

    CircuitLinkedList dynamicCircuit;

    // Step A: Add AND, OR, NOT
    std::cout << "\nStep A: Dynamically adding components (AND, OR, NOT) to Linked List ...\n";
    int andInp[2] = {1, 1};
    dynamicCircuit.insertNode(101, "AND", "Canvas_AND_Gate", 2, andInp);

    int orInp[2] = {0, 0};
    dynamicCircuit.insertNode(102, "OR", "Canvas_OR_Gate", 2, orInp);

    int notInp[1] = {1};
    dynamicCircuit.insertNode(103, "NOT", "Canvas_NOT_Gate", 1, notInp);

    // Step B: Display list
    std::cout << "\nStep B: Traversing and displaying Dynamic Linked List:\n";
    dynamicCircuit.traverseAndDisplay();

    // Step C: Delete one gate
    std::cout << "Step C: Deleting Gate ID 102 (OR Gate) from Linked List ...\n";
    dynamicCircuit.deleteNode(102);

    // Step D: Update one gate
    std::cout << "\nStep D: Updating Gate ID 103 (NOT Gate input changed from 1 to 0) ...\n";
    int newNotInp[1] = {0};
    dynamicCircuit.updateNode(103, "Inverted_Buffer", newNotInp);

    // Step E: Search one gate
    std::cout << "\nStep E: Searching for Gate ID 101 in Linked List ...\n";
    GateNode* searched = dynamicCircuit.searchNode(101);
    if (searched != nullptr) {
        std::cout << " [SUCCESS] Found Node at Address: " << searched 
                  << " | Type: " << searched->gateType 
                  << " | Name: '" << searched->gateName << "'\n";
    }

    // Step F: Display updated list
    std::cout << "\nStep F: Traversing and displaying Updated Dynamic Linked List:\n";
    dynamicCircuit.traverseAndDisplay();
}

void demonstrateEndToEndCircuit() {
    std::cout << "\n=================================================================\n";
    std::cout << " [DEMONSTRATION 4]: INTEGRATED 3-GATE REFERENCE CIRCUIT          \n";
    std::cout << "=================================================================\n";
    std::cout << "Simulating Reference Circuit: (A AND B) -> G2[0], G2 = (G1.out OR C), G3 = NOT(G2.out)\n";

    Circuit refCircuit("LogicCraft Review-2 Reference Circuit");

    int inG1[2] = {1, 0}; // 1 AND 0 = 0
    Gate g1(1, "AND", "G1_AND", 2, inG1);
    refCircuit.addGate(g1);

    int inG2[2] = {0, 1}; // G1.out (0) OR C (1) = 1
    Gate g2(2, "OR", "G2_OR", 2, inG2);
    refCircuit.addGate(g2);

    int inG3[1] = {0}; // NOT(G2.out) = NOT(1) = 0
    Gate g3(3, "NOT", "G3_NOT", 1, inG3);
    refCircuit.addGate(g3);

    // Add wires
    refCircuit.addConnection(1, 2, 0); // G1 output -> G2 input 0
    refCircuit.addConnection(2, 3, 0); // G2 output -> G3 input 0

    refCircuit.displayCircuit();
    refCircuit.evaluateCircuit();

    std::cout << "\nFinal Evaluated Outputs:\n";
    std::cout << " -> G1 (1 AND 0)               = " << refCircuit.getGateRegistry().getGateAt(0)->getOutput() << "\n";
    std::cout << " -> G2 (G1.out OR 1) = (0 OR 1) = " << refCircuit.getGateRegistry().getGateAt(1)->getOutput() << "\n";
    std::cout << " -> G3 (NOT G2.out)  = NOT(1)   = " << refCircuit.getGateRegistry().getGateAt(2)->getOutput() << "\n";
}

void demonstrateCO3_StackAndQueue() {
    std::cout << "\n=================================================================\n";
    std::cout << " [DEMONSTRATION 5 / CO3]: STACK AND QUEUE DATA STRUCTURES        \n";
    std::cout << "=================================================================\n";
    std::cout << "ACADEMIC JUSTIFICATION (CO3):\n";
    std::cout << " 1. Stack (LIFO): Manages circuit modification history for Undo.\n";
    std::cout << "    Last action applied to canvas is the first one reverted.\n";
    std::cout << " 2. Queue (FIFO): Manages event propagation order for simulation.\n";
    std::cout << "    Signal changes are evaluated chronologically in arrival order.\n";
    std::cout << "-----------------------------------------------------------------\n";

    // -------------------------------------------------------------
    // Part A: Procedural LIFO Undo Stack Demonstration
    // -------------------------------------------------------------
    std::cout << "\n--- [CO3.1: PROCEDURAL LIFO UNDO STACK] ---\n";
    CircuitUndoStack undoStack;
    initUndoStack(&undoStack);

    std::cout << "Step 1: Inspecting Initial Empty Stack ...\n";
    std::cout << " -> isStackEmpty: " << (isStackEmpty(&undoStack) ? "TRUE (Verified Empty)" : "FALSE") << "\n";
    
    // Boundary Underflow Test
    CircuitAction dummyAction;
    std::cout << " -> Attempting popAction() on empty stack (Boundary Underflow Test): ";
    if (!popAction(&undoStack, &dummyAction)) {
        std::cout << " [PROTECTED: Underflow cleanly prevented!]\n";
    }

    // Circuit integration with Stack
    Circuit activeCircuit("Undo Stack Integrated Circuit");
    Gate g1(1, "AND", "G1_AND", 2, (int[]){1, 0});
    Gate g2(2, "OR", "G2_OR", 2, (int[]){0, 1});
    activeCircuit.addGate(g1);
    activeCircuit.addGate(g2);

    std::cout << "\nStep 2: Performing Operations with Automatic Undo Stack Recording ...\n";
    
    // Operation 1: Add Gate G3 (XOR)
    Gate g3(3, "XOR", "G3_XOR", 2, (int[]){0, 0});
    executeAddGateWithUndo(&activeCircuit, &undoStack, g3);
    std::cout << " [Recorded] Pushed Action: Add Gate G3 (XOR)\n";

    // Operation 2: Add Connection G1 -> G3
    executeAddConnectionWithUndo(&activeCircuit, &undoStack, 1, 3, 0);
    std::cout << " [Recorded] Pushed Action: Add Wire G1 -> G3 (Pin 0)\n";

    // Operation 3: Toggle Input Gate G1 Pin 1 (0 -> 1)
    executeToggleInputWithUndo(&activeCircuit, &undoStack, 1, 1, 1);

    std::cout << "\nStep 3: Displaying Undo Stack State (LIFO Inspection) ...\n";
    displayUndoStack(&undoStack);

    std::cout << "Step 4: Executing Sequential Undos (LIFO Verification) ...\n";
    // Undo 1: Reverts Input Toggle
    executeUndoOperation(&activeCircuit, &undoStack);
    
    // Undo 2: Reverts Wire Connection
    executeUndoOperation(&activeCircuit, &undoStack);

    // Undo 3: Reverts Gate Addition
    executeUndoOperation(&activeCircuit, &undoStack);

    std::cout << "\nStack state after 3 undos:\n";
    displayUndoStack(&undoStack);

    // Stack Boundary Test: Push to Full Capacity
    std::cout << "Step 5: Testing Stack Boundary / Overflow (Max Capacity = " << MAX_STACK_CAPACITY << ") ...\n";
    CircuitUndoStack stressStack;
    initUndoStack(&stressStack);
    for (int i = 0; i < MAX_STACK_CAPACITY; i++) {
        CircuitAction a = createAddGateAction(i + 1, "AND", "Gate");
        pushAction(&stressStack, &a);
    }
    std::cout << " -> Filled stack to size: " << getStackSize(&stressStack) << " (isStackFull = " 
              << (isStackFull(&stressStack) ? "TRUE" : "FALSE") << ")\n";
    CircuitAction overflowAction = createAddGateAction(999, "NOT", "OverflowGate");
    std::cout << " -> Attempting push to full stack (Overflow Boundary Test): ";
    if (!pushAction(&stressStack, &overflowAction)) {
        std::cout << " [PROTECTED: Stack Overflow handled safely!]\n";
    }

    // -------------------------------------------------------------
    // Part B: Procedural FIFO Simulation Event Queue Demonstration
    // -------------------------------------------------------------
    std::cout << "\n--- [CO3.2: PROCEDURAL FIFO SIMULATION EVENT QUEUE] ---\n";
    SimulationQueue simQueue;
    initSimulationQueue(&simQueue);

    std::cout << "Step 1: Inspecting Initial Empty Queue ...\n";
    std::cout << " -> isQueueEmpty: " << (isQueueEmpty(&simQueue) ? "TRUE (Verified Empty)" : "FALSE") << "\n";

    // Boundary Underflow Test
    SimulationEvent dummyEvent;
    std::cout << " -> Attempting dequeueEvent() on empty queue (Boundary Underflow Test): ";
    if (!dequeueEvent(&simQueue, &dummyEvent)) {
        std::cout << " [PROTECTED: Underflow cleanly prevented!]\n";
    }

    std::cout << "\nStep 2: Executing Dynamic Signal Propagation with FIFO Event Queue ...\n";
    Circuit simCircuit("Queue Propagation Demo Circuit");
    int in1[2] = {1, 1}; // AND outputs 1
    Gate qG1(1, "AND", "Sensor_AND", 2, in1);
    int in2[2] = {0, 0};
    Gate qG2(2, "OR", "Alert_OR", 2, in2);
    int in3[1] = {0};
    Gate qG3(3, "NOT", "Invert_NOT", 1, in3);

    simCircuit.addGate(qG1);
    simCircuit.addGate(qG2);
    simCircuit.addGate(qG3);
    simCircuit.addConnection(1, 2, 0); // qG1[OUT] -> qG2[Pin 0]
    simCircuit.addConnection(2, 3, 0); // qG2[OUT] -> qG3[Pin 0]

    // Run queued simulation
    executeSimulationWithQueue(&simCircuit, &simQueue);

    // Queue Boundary Test: Enqueue to Full Capacity
    std::cout << "Step 3: Testing Queue Boundary / Overflow (Max Capacity = " << MAX_QUEUE_CAPACITY << ") ...\n";
    SimulationQueue stressQueue;
    initSimulationQueue(&stressQueue);
    for (int i = 0; i < MAX_QUEUE_CAPACITY; i++) {
        SimulationEvent ev = createSimulationEvent(i + 1, 1, 0, 1, 1, "Stress Event");
        enqueueEvent(&stressQueue, &ev);
    }
    std::cout << " -> Filled queue to count: " << getQueueSize(&stressQueue) << " (isQueueFull = " 
              << (isQueueFull(&stressQueue) ? "TRUE" : "FALSE") << ")\n";
    SimulationEvent overflowEvent = createSimulationEvent(999, 1, 0, 1, 1, "Overflow Event");
    std::cout << " -> Attempting enqueue to full queue (Overflow Boundary Test): ";
    if (!enqueueEvent(&stressQueue, &overflowEvent)) {
        std::cout << " [PROTECTED: Queue Overflow handled safely!]\n";
    }
}

void demonstrateCO4_SearchingAndSorting() {
    std::cout << "\n=================================================================\n";
    std::cout << " [DEMONSTRATION 4 / CO4]: PROCEDURAL SEARCHING & SORTING         \n";
    std::cout << "=================================================================\n";

    // 1. Searching
    std::cout << "\n--- [CO4.1: LINEAR SEARCH & BINARY SEARCH] ---\n";
    ComponentRecord dataset[6] = {
        {10, "AND", "Front_AND",  2, 0},
        {20, "OR",  "Mid_OR",     2, 1},
        {30, "XOR", "Sum_XOR",    2, 1},
        {40, "NOT", "Invert_NOT", 1, 0},
        {50, "NAND","NAND_Latch", 2, 1},
        {60, "NOR", "NOR_Gate",   2, 0}
    };
    displayComponentArray(dataset, 6, "Sample Circuit Components for Search");

    int linComps = 0, binComps = 0;
    int targetId = 40;
    std::cout << "\nSearching for Component ID " << targetId << " ...\n";
    int idxLin = linearSearchById(dataset, 6, targetId, &linComps);
    displaySearchResult(idxLin, dataset, linComps, "Linear Search");

    int idxBin = binarySearchById(dataset, 6, targetId, &binComps);
    displaySearchResult(idxBin, dataset, binComps, "Binary Search");

    std::cout << "\nSearching for Component by Name \"Sum_XOR\" ...\n";
    int idxName = linearSearchByName(dataset, 6, "Sum_XOR", &linComps);
    displaySearchResult(idxName, dataset, linComps, "Linear Search by Name");

    // 2. Sorting
    std::cout << "\n--- [CO4.2: BUBBLE SORT & SELECTION SORT] ---\n";
    ComponentRecord unsorted[5] = {
        {45, "NOT", "Inverter", 1, 0},
        {12, "AND", "Carry_AND", 2, 1},
        {33, "XOR", "Parity_XOR", 2, 0},
        {18, "OR",  "Gate_OR", 2, 1},
        {25, "NAND","Latch_NAND", 2, 1}
    };
    displayComponentArray(unsorted, 5, "Unsorted Initial Component Array");

    ComponentRecord bubbleArr[5];
    copyComponentArray(unsorted, bubbleArr, 5);
    SortMetrics mBubble = {0, 0};
    bubbleSort(bubbleArr, 5, SORT_BY_ID, &mBubble);
    std::cout << " -> Bubble Sort by ID Completed: " << mBubble.comparisons 
              << " Comparisons, " << mBubble.swaps << " Swaps.\n";
    displayComponentArray(bubbleArr, 5, "Sorted by ID via Bubble Sort");

    ComponentRecord selArr[5];
    copyComponentArray(unsorted, selArr, 5);
    SortMetrics mSel = {0, 0};
    selectionSort(selArr, 5, SORT_BY_NAME, &mSel);
    std::cout << " -> Selection Sort by Name Completed: " << mSel.comparisons 
              << " Comparisons, " << mSel.swaps << " Swaps.\n";
    displayComponentArray(selArr, 5, "Sorted by Name via Selection Sort");

    // Empirical Benchmarks
    runAlgorithmPerformanceBenchmarks();
}

void demonstrateCO5_Hashing() {
    std::cout << "\n=================================================================\n";
    std::cout << " [DEMONSTRATION 5 / CO5]: HASH TABLE WITH LINEAR PROBING         \n";
    std::cout << "=================================================================\n";
    std::cout << "Fast O(1) Component Lookup with Collision Resolution.\n";

    ComponentHashTable ht;
    initHashTable(&ht);

    // Insert elements, deliberately including keys that hash to the same bucket
    ComponentRecord r1 = {1,  "AND", "Front_AND",   2, 0};
    ComponentRecord r2 = {18, "OR",  "Mid_OR",      2, 1}; // 18 % 17 = 1 (Collision with ID 1!)
    ComponentRecord r3 = {35, "XOR", "Branch_XOR",  2, 1}; // 35 % 17 = 1 (Triple collision!)
    ComponentRecord r4 = {5,  "NOT", "Final_NOT",   1, 0};
    ComponentRecord r5 = {9,  "NAND","Latch_NAND",  2, 1};

    int probes = 0;
    insertHashTable(&ht, &r1, &probes);
    std::cout << " -> Inserted ID " << r1.id << " (Base Hash: " << hashFunctionId(r1.id) 
              << ") | Probes: " << probes << "\n";

    insertHashTable(&ht, &r2, &probes);
    std::cout << " -> Inserted ID " << r2.id << " (Base Hash: " << hashFunctionId(r2.id) 
              << ") | Probes: " << probes << " [COLLISION DETECTED & RESOLVED VIA LINEAR PROBING]\n";

    insertHashTable(&ht, &r3, &probes);
    std::cout << " -> Inserted ID " << r3.id << " (Base Hash: " << hashFunctionId(r3.id) 
              << ") | Probes: " << probes << " [COLLISION DETECTED & RESOLVED VIA LINEAR PROBING]\n";

    insertHashTable(&ht, &r4, &probes);
    insertHashTable(&ht, &r5, &probes);

    displayHashTable(&ht);

    // Fast O(1) Lookup
    std::cout << "\nPerforming Fast Lookup for Component ID 35 in Hash Table ...\n";
    ComponentRecord foundRec;
    int searchProbes = 0;
    int bucket = searchHashTableById(&ht, 35, &foundRec, &searchProbes);
    if (bucket >= 0) {
        std::cout << " [LOOKUP SUCCESS]: Found ID 35 in Bucket [" << bucket << "] after " 
                  << searchProbes << " probe(s). Gate Name: " << foundRec.name << "\n";
    }

    // Deletion with tombstone demonstration
    std::cout << "\nDeleting Component ID 18 (middle of collision chain) ...\n";
    deleteHashTableById(&ht, 18);
    displayHashTable(&ht);

    std::cout << "Searching again for Component ID 35 across deleted slot ...\n";
    bucket = searchHashTableById(&ht, 35, &foundRec, &searchProbes);
    if (bucket >= 0) {
        std::cout << " [CHAIN PRESERVED]: Found ID 35 in Bucket [" << bucket << "] safely using tombstone probing.\n";
    }
}

int main(int argc, char* argv[]) {
    printBanner();

    demonstrateCO1_Arrays();
    demonstrateCO2_LinkedList();
    demonstrateEndToEndCircuit();
    demonstrateCO3_StackAndQueue();
    demonstrateCO4_SearchingAndSorting();
    demonstrateCO5_Hashing();
    runCircuitValidationScenarios();

    std::cout << "\n*****************************************************************\n";
    std::cout << "*  C++ PROGRAMMING LAB (CO1, CO2, CO3, CO4, CO5) COMPLETE!      *\n";
    std::cout << "*****************************************************************\n\n";

    return 0;
}

