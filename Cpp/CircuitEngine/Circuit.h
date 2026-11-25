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

#include "../DataStructures/StackOperations.h"
#include "../DataStructures/QueueOperations.h"

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
    bool removeConnection(int srcGateId, int dstGateId, int dstInputIdx);
    
    // Propagate inputs and evaluate circuit
    void evaluateCircuit();

    void displayCircuit() const;

    GateArray& getGateRegistry();
    int getConnectionCount() const;
    const Connection* getConnectionAt(int index) const;
    const char* getName() const;
};

// Procedural Circuit Operations with LIFO Undo Stack (CO3)
bool executeAddGateWithUndo(Circuit* circuit, CircuitUndoStack* undoStack, const Gate& gate);
bool executeDeleteGateWithUndo(Circuit* circuit, CircuitUndoStack* undoStack, int gateId);
bool executeToggleInputWithUndo(Circuit* circuit, CircuitUndoStack* undoStack, int gateId, int pin, int newVal);
bool executeAddConnectionWithUndo(Circuit* circuit, CircuitUndoStack* undoStack, int srcId, int dstId, int dstPin);
bool executeUndoOperation(Circuit* circuit, CircuitUndoStack* undoStack);

// Procedural Circuit Simulation with FIFO Event Queue (CO3)
void executeSimulationWithQueue(Circuit* circuit, SimulationQueue* queue);

#endif // CIRCUIT_H
