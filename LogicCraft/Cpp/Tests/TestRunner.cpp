#include "../Gates/Gate.h"
#include "../DataStructures/ArrayOperations.h"
#include "../DataStructures/LinkedList.h"
#include "../CircuitEngine/TruthTable.h"
#include "../CircuitEngine/Circuit.h"
#include <iostream>
#include <cassert>

/**
 * @file TestRunner.cpp
 * @brief Automated test suite for C++ Module (CO1 Arrays & CO2 Linked Lists)
 */

void testArrayOperations() {
    std::cout << "[TEST] Running Array Operations (CO1) Test ...\n";
    GateArray arr;
    int inputs1[2] = {1, 1};
    Gate g1(1, "AND", "TestAND", 2, inputs1);
    assert(arr.insertGate(g1) == true);
    assert(arr.getCount() == 1);

    int inputs2[2] = {0, 1};
    Gate g2(2, "OR", "TestOR", 2, inputs2);
    assert(arr.insertGate(g2) == true);
    assert(arr.getCount() == 2);

    assert(arr.searchGateById(1) == 0);
    assert(arr.searchGateById(2) == 1);
    assert(arr.searchGateById(999) == -1);

    int newInputs[2] = {1, 0};
    assert(arr.updateGate(1, "UpdatedAND", newInputs, 2) == true);
    assert(arr.getGateAt(0)->getOutput() == 0); // 1 AND 0 = 0

    assert(arr.deleteGate(1) == true);
    assert(arr.getCount() == 1);
    assert(arr.searchGateById(1) == -1);
    assert(arr.searchGateById(2) == 0);

    std::cout << " -> Array Operations PASS!\n";
}

void testTruthTableGeneration() {
    std::cout << "[TEST] Running Truth Table (CO1) Test ...\n";
    int inputs[2] = {0, 0};
    Gate andGate(10, "AND", "TruthAND", 2, inputs);
    TruthTable tt;
    tt.generateForGate(andGate);
    assert(tt.getNumRows() == 4);
    // 0 AND 0 = 0
    assert(tt.getOutput(0) == 0);
    // 0 AND 1 = 0
    assert(tt.getOutput(1) == 0);
    // 1 AND 0 = 0
    assert(tt.getOutput(2) == 0);
    // 1 AND 1 = 1
    assert(tt.getOutput(3) == 1);

    std::cout << " -> Truth Table Generation PASS!\n";
}

void testLinkedListOperations() {
    std::cout << "[TEST] Running Linked List (CO2) Test ...\n";
    CircuitLinkedList list;
    int in1[2] = {1, 1};
    list.insertNode(101, "AND", "DynAND", 2, in1);
    int in2[2] = {0, 1};
    list.insertNode(102, "OR", "DynOR", 2, in2);
    int in3[1] = {1};
    list.insertNode(103, "NOT", "DynNOT", 1, in3);

    assert(list.getCount() == 3);
    assert(list.searchNode(101) != nullptr);
    assert(list.searchNode(102) != nullptr);
    assert(list.searchNode(103) != nullptr);

    GateNode* n1 = list.searchNode(101);
    assert(n1->outputValue == 1);

    GateNode* n3 = list.searchNode(103);
    assert(n3->outputValue == 0); // NOT 1 = 0

    int updatedIn[1] = {0};
    assert(list.updateNode(103, "InvertedNOT", updatedIn) == true);
    assert(list.searchNode(103)->outputValue == 1); // NOT 0 = 1

    assert(list.deleteNode(102) == true);
    assert(list.getCount() == 2);
    assert(list.searchNode(102) == nullptr);

    std::cout << " -> Linked List Operations PASS!\n";
}

int main() {
    std::cout << "======================================================\n";
    std::cout << "      LOGICCRAFT C++ UNIT TEST VERIFICATION SUITE     \n";
    std::cout << "======================================================\n";
    testArrayOperations();
    testTruthTableGeneration();
    testLinkedListOperations();
    std::cout << "======================================================\n";
    std::cout << " ALL C++ REVIEW-2 TESTS PASSED SUCCESSFULLY! (CO1 & CO2)\n";
    std::cout << "======================================================\n";
    return 0;
}
