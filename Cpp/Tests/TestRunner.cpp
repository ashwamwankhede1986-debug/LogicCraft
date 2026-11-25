#include "../Gates/Gate.h"
#include "../DataStructures/ArrayOperations.h"
#include "../DataStructures/LinkedList.h"
#include "../CircuitEngine/TruthTable.h"
#include "../CircuitEngine/Circuit.h"
#include "Task4Tests.h"
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

void testStackOperations() {
    std::cout << "[TEST] Running Stack Operations (CO3 LIFO) Test ...\n";
    CircuitUndoStack stack;
    initUndoStack(&stack);

    // Boundary 1: Empty structure
    assert(isStackEmpty(&stack) == true);
    assert(isStackFull(&stack) == false);
    assert(getStackSize(&stack) == 0);

    // Boundary 2: Underflow protection
    CircuitAction dummy;
    assert(popAction(&stack, &dummy) == false);
    assert(peekAction(&stack, &dummy) == false);

    // Normal LIFO operations
    CircuitAction a1 = createAddGateAction(1, "AND", "G1");
    CircuitAction a2 = createUpdateInputAction(1, 0, 0, 1);
    CircuitAction a3 = createDeleteGateAction(2, "OR", "G2");

    assert(pushAction(&stack, &a1) == true);
    assert(pushAction(&stack, &a2) == true);
    assert(pushAction(&stack, &a3) == true);
    assert(getStackSize(&stack) == 3);

    // Peek verifies top element
    CircuitAction peekTop;
    assert(peekAction(&stack, &peekTop) == true);
    assert(peekTop.type == ACTION_DELETE_GATE);
    assert(peekTop.gateId == 2);

    // Pop verifies LIFO order (a3 then a2 then a1)
    CircuitAction popped;
    assert(popAction(&stack, &popped) == true);
    assert(popped.type == ACTION_DELETE_GATE);

    assert(popAction(&stack, &popped) == true);
    assert(popped.type == ACTION_UPDATE_INPUT);

    assert(popAction(&stack, &popped) == true);
    assert(popped.type == ACTION_ADD_GATE);

    assert(isStackEmpty(&stack) == true);

    // Boundary 3: Overflow protection
    for (int i = 0; i < MAX_STACK_CAPACITY; i++) {
        CircuitAction a = createAddGateAction(i + 1, "AND", "Stress");
        assert(pushAction(&stack, &a) == true);
    }
    assert(isStackFull(&stack) == true);
    assert(getStackSize(&stack) == MAX_STACK_CAPACITY);
    CircuitAction overflow = createAddGateAction(999, "NOT", "Overflow");
    assert(pushAction(&stack, &overflow) == false); // Overflow blocked cleanly

    clearUndoStack(&stack);
    assert(isStackEmpty(&stack) == true);
    std::cout << " -> Stack Operations (LIFO & Boundaries) PASS!\n";
}

void testQueueOperations() {
    std::cout << "[TEST] Running Queue Operations (CO3 FIFO) Test ...\n";
    SimulationQueue queue;
    initSimulationQueue(&queue);

    // Boundary 1: Empty structure
    assert(isQueueEmpty(&queue) == true);
    assert(isQueueFull(&queue) == false);
    assert(getQueueSize(&queue) == 0);

    // Boundary 2: Underflow protection
    SimulationEvent dummy;
    assert(dequeueEvent(&queue, &dummy) == false);
    assert(peekQueue(&queue, &dummy) == false);

    // Normal FIFO operations
    SimulationEvent e1 = createSimulationEvent(101, 1, 0, 1, 1, "Event 1");
    SimulationEvent e2 = createSimulationEvent(102, 2, 1, 0, 1, "Event 2");
    SimulationEvent e3 = createSimulationEvent(103, 3, 0, 1, 2, "Event 3");

    assert(enqueueEvent(&queue, &e1) == true);
    assert(enqueueEvent(&queue, &e2) == true);
    assert(enqueueEvent(&queue, &e3) == true);
    assert(getQueueSize(&queue) == 3);

    // Peek verifies front element
    SimulationEvent peekFront;
    assert(peekQueue(&queue, &peekFront) == true);
    assert(peekFront.eventId == 101);

    // Dequeue verifies FIFO order (e1 then e2 then e3)
    SimulationEvent dequeued;
    assert(dequeueEvent(&queue, &dequeued) == true);
    assert(dequeued.eventId == 101);

    assert(dequeueEvent(&queue, &dequeued) == true);
    assert(dequeued.eventId == 102);

    assert(dequeueEvent(&queue, &dequeued) == true);
    assert(dequeued.eventId == 103);

    assert(isQueueEmpty(&queue) == true);

    // Boundary 3: Overflow protection with circular buffer
    for (int i = 0; i < MAX_QUEUE_CAPACITY; i++) {
        SimulationEvent ev = createSimulationEvent(i + 1, 1, 0, 1, 1);
        assert(enqueueEvent(&queue, &ev) == true);
    }
    assert(isQueueFull(&queue) == true);
    SimulationEvent overflow = createSimulationEvent(999, 1, 0, 1, 1);
    assert(enqueueEvent(&queue, &overflow) == false); // Overflow blocked cleanly

    clearSimulationQueue(&queue);
    assert(isQueueEmpty(&queue) == true);
    std::cout << " -> Queue Operations (FIFO & Boundaries) PASS!\n";
}

void testCircuitUndoIntegration() {
    std::cout << "[TEST] Running Circuit Undo & Queue Integration Test ...\n";
    Circuit c("Test Integration Circuit");
    CircuitUndoStack stack;
    initUndoStack(&stack);

    Gate g1(1, "AND", "G1", 2, (int[]){1, 0});
    assert(executeAddGateWithUndo(&c, &stack, g1) == true);
    assert(c.getGateRegistry().getCount() == 1);
    assert(getStackSize(&stack) == 1);

    // Execute Undo
    assert(executeUndoOperation(&c, &stack) == true);
    assert(c.getGateRegistry().getCount() == 0); // Gate removed
    assert(getStackSize(&stack) == 0);

    std::cout << " -> Circuit Undo & Queue Integration PASS!\n";
}

int main() {
    std::cout << "======================================================\n";
    std::cout << "      LOGICCRAFT C++ UNIT TEST VERIFICATION SUITE     \n";
    std::cout << "======================================================\n";
    testArrayOperations();
    testTruthTableGeneration();
    testLinkedListOperations();
    testStackOperations();
    testQueueOperations();
    testCircuitUndoIntegration();
    runAllTask4Tests();
    std::cout << "======================================================\n";
    std::cout << " ALL C++ TESTS PASSED SUCCESSFULLY! (CO1, CO2, CO3, CO4, CO5)\n";
    std::cout << "======================================================\n";
    return 0;
}
