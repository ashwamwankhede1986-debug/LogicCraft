#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "../Gates/Gate.h"
#include "../DataStructures/ArrayOperations.h"

/**
 * @file Circuit.h
 * @brief Array-Based LogicCraft Circuit Engine (CO1 - Arrays)
 *
 * Demonstrates:
 * - Managing circuit components using array storage
 * - Managing wire connections between gates
 * - Propagating evaluated outputs across array-indexed gates
 */

struct Connection {
    int sourceGateId;
    int destinationGateId;
    int destinationInputIndex;
};

const int MAX_CONNECTIONS = 32;

class Circuit {
private:
    char name[64];
    GateArray gateRegistry; // Array of gates
    Connection connections[MAX_CONNECTIONS]; // Array of connections
    int connectionCount;

public:
    Circuit(const char* circuitName = "Default Circuit");

    bool addGate(const Gate& gate);
    bool removeGate(int gateId);
    bool addConnection(int srcGateId, int dstGateId, int dstInputIdx);
    
    // Propagate inputs and evaluate circuit
    void evaluateCircuit();

    void displayCircuit() const;

    GateArray& getGateRegistry();
};

#endif // CIRCUIT_H
