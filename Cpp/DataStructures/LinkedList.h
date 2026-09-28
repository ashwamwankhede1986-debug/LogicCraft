#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <cstring>

/**
 * @file LinkedList.h
 * @brief Dynamic Circuit Component Structure using Linked List (CO2 - Linked List)
 *
 * ACADEMIC JUSTIFICATION:
 * "LogicCraft allows components to be dynamically added and deleted on the visual canvas,
 * therefore a linked list provides a flexible dynamic structure without fixed array boundaries."
 */

struct GateNode {
    int gateId;
    char gateType[16];   // e.g. "AND", "OR", "NOT", "XOR"
    char gateName[32];   // e.g. "Gate_A1"
    int inputCount;
    int inputs[4];       // Boolean inputs
    int outputValue;     // Evaluated output
    GateNode* next;      // Pointer to the next node in the dynamic circuit
};

class CircuitLinkedList {
private:
    GateNode* head;
    int count;

    int computeOutput(const char* type, int inCount, const int inps[]);

public:
    CircuitLinkedList();
    ~CircuitLinkedList();

    // Fundamental Linked List Operations (CO2)
    void insertNode(int id, const char* type, const char* name, int inputCount, const int initialInputs[]);
    bool deleteNode(int id);
    bool updateNode(int id, const char* newName, const int newInputs[]);
    GateNode* searchNode(int id) const;
    void traverseAndDisplay() const;

    // Evaluate all nodes along the chain
    void evaluateAll();

    // Clear entire list (memory cleanup)
    void clear();

    int getCount() const;
};

#endif // LINKED_LIST_H
