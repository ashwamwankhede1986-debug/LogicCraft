#include "Gates/Gate.h"
#include "DataStructures/ArrayOperations.h"
#include "DataStructures/LinkedList.h"
#include "CircuitEngine/TruthTable.h"
#include "CircuitEngine/Circuit.h"
#include "Algorithms/AlgorithmsPlaceholder.h"

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

int main(int argc, char* argv[]) {
    printBanner();

    demonstrateCO1_Arrays();
    demonstrateCO2_LinkedList();
    demonstrateEndToEndCircuit();

    std::cout << "\n*****************************************************************\n";
    std::cout << "* [EXTENSION POINT AUDIT]: CO3-CO5 FUTURE WORK                  *\n";
    std::cout << "*  " << LogicCraftReviewExt::EvaluationQueueStub::status() << "\n";
    std::cout << "*  " << LogicCraftReviewExt::ComponentHashIndexStub::status() << "\n";
    std::cout << "*****************************************************************\n";
    std::cout << "*  C++ PROGRAMMING LAB REVIEW-2 DEMONSTRATION COMPLETE!          *\n";
    std::cout << "*****************************************************************\n\n";

    return 0;
}
