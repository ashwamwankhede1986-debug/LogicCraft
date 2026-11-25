#include "Circuit.h"
#include <iostream>
#include <cstring>

Circuit::Circuit(const char* circuitName) : connectionCount(0) {
    strncpy(name, circuitName, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
}

bool Circuit::addGate(const Gate& gate) {
    return gateRegistry.insertGate(gate);
}

bool Circuit::removeGate(int gateId) {
    // Also remove connections tied to this gate
    for (int i = 0; i < connectionCount; ) {
        if (connections[i].sourceGateId == gateId || connections[i].destinationGateId == gateId) {
            for (int j = i; j < connectionCount - 1; ++j) {
                connections[j] = connections[j + 1];
            }
            connectionCount--;
        } else {
            i++;
        }
    }
    return gateRegistry.deleteGate(gateId);
}

bool Circuit::addConnection(int srcGateId, int dstGateId, int dstInputIdx) {
    if (connectionCount >= MAX_CONNECTIONS) {
        std::cerr << "[Circuit Error] Maximum connections reached!\n";
        return false;
    }
    if (gateRegistry.searchGateById(srcGateId) == -1) {
        std::cerr << "[Circuit Error] Source gate ID " << srcGateId << " does not exist!\n";
        return false;
    }
    if (gateRegistry.searchGateById(dstGateId) == -1) {
        std::cerr << "[Circuit Error] Destination gate ID " << dstGateId << " does not exist!\n";
        return false;
    }

    connections[connectionCount].sourceGateId = srcGateId;
    connections[connectionCount].destinationGateId = dstGateId;
    connections[connectionCount].destinationInputIndex = dstInputIdx;
    connectionCount++;

    std::cout << "[Circuit Wire] Connected Gate " << srcGateId << " -> Gate " 
              << dstGateId << " (Input Pin " << dstInputIdx << ").\n";
    return true;
}

void Circuit::evaluateCircuit() {
    std::cout << "\n>>> Evaluating Circuit '" << name << "' ...\n";
    
    // First pass: evaluate independent gates
    for (int i = 0; i < gateRegistry.getCount(); ++i) {
        Gate* g = gateRegistry.getGateAt(i);
        if (g) g->evaluate();
    }

    // Second pass: propagate connections and re-evaluate
    for (int c = 0; c < connectionCount; ++c) {
        int srcIdx = gateRegistry.searchGateById(connections[c].sourceGateId);
        int dstIdx = gateRegistry.searchGateById(connections[c].destinationGateId);

        if (srcIdx != -1 && dstIdx != -1) {
            Gate* srcGate = gateRegistry.getGateAt(srcIdx);
            Gate* dstGate = gateRegistry.getGateAt(dstIdx);
            
            int signal = srcGate->getOutput();
            dstGate->setInput(connections[c].destinationInputIndex, signal);
            dstGate->evaluate();
        }
    }
    std::cout << ">>> Circuit evaluation complete.\n";
}

void Circuit::displayCircuit() const {
    std::cout << "\n======================================================\n";
    std::cout << "              CIRCUIT: " << name << "\n";
    std::cout << "======================================================\n";
    gateRegistry.displayAll();

    std::cout << "--- Connections / Wires (" << connectionCount << ") ---\n";
    if (connectionCount == 0) {
        std::cout << "  (No wire connections configured)\n";
    }
    for (int i = 0; i < connectionCount; ++i) {
        std::cout << "  Wire [" << i << "]: Gate " << connections[i].sourceGateId 
                  << " [OUT] ===> Gate " << connections[i].destinationGateId 
                  << " [Pin " << connections[i].destinationInputIndex << "]\n";
    }
    std::cout << "======================================================\n\n";
}

bool Circuit::removeConnection(int srcGateId, int dstGateId, int dstInputIdx) {
    for (int i = 0; i < connectionCount; i++) {
        if (connections[i].sourceGateId == srcGateId &&
            connections[i].destinationGateId == dstGateId &&
            connections[i].destinationInputIndex == dstInputIdx) {
            for (int j = i; j < connectionCount - 1; j++) {
                connections[j] = connections[j + 1];
            }
            connectionCount--;
            std::cout << "[Circuit Wire] Removed wire: Gate " << srcGateId 
                      << " -> Gate " << dstGateId << " (Pin " << dstInputIdx << ").\n";
            return true;
        }
    }
    return false;
}

GateArray& Circuit::getGateRegistry() {
    return gateRegistry;
}

int Circuit::getConnectionCount() const {
    return connectionCount;
}

const Connection* Circuit::getConnectionAt(int index) const {
    if (index >= 0 && index < connectionCount) {
        return &connections[index];
    }
    return nullptr;
}

const char* Circuit::getName() const {
    return name;
}

// ------------------------------------------------------------------------------
// Procedural LIFO Undo Stack Circuit Operations (CO3)
// ------------------------------------------------------------------------------

bool executeAddGateWithUndo(Circuit* circuit, CircuitUndoStack* undoStack, const Gate& gate) {
    if (!circuit) return false;
    bool success = circuit->addGate(gate);
    if (success && undoStack) {
        CircuitAction action = createAddGateAction(gate.getId(), gate.getType(), gate.getName());
        pushAction(undoStack, &action);
    }
    return success;
}

bool executeDeleteGateWithUndo(Circuit* circuit, CircuitUndoStack* undoStack, int gateId) {
    if (!circuit) return false;
    int idx = circuit->getGateRegistry().searchGateById(gateId);
    if (idx == -1) return false;

    const Gate* g = circuit->getGateRegistry().getGateAt(idx);
    CircuitAction action = createDeleteGateAction(g->getId(), g->getType(), g->getName());
    
    bool success = circuit->removeGate(gateId);
    if (success && undoStack) {
        pushAction(undoStack, &action);
    }
    return success;
}

bool executeToggleInputWithUndo(Circuit* circuit, CircuitUndoStack* undoStack, int gateId, int pin, int newVal) {
    if (!circuit) return false;
    int idx = circuit->getGateRegistry().searchGateById(gateId);
    if (idx == -1) return false;

    Gate* g = circuit->getGateRegistry().getGateAt(idx);
    int oldVal = g->getInput(pin);
    g->setInput(pin, newVal);
    g->evaluate();

    if (undoStack) {
        CircuitAction action = createUpdateInputAction(gateId, pin, oldVal, newVal);
        pushAction(undoStack, &action);
    }
    std::cout << "[Circuit Action] Toggled Gate " << gateId << " Pin " << pin 
              << ": " << oldVal << " -> " << newVal << " (Pushed to Undo Stack).\n";
    return true;
}

bool executeAddConnectionWithUndo(Circuit* circuit, CircuitUndoStack* undoStack, int srcId, int dstId, int dstPin) {
    if (!circuit) return false;
    bool success = circuit->addConnection(srcId, dstId, dstPin);
    if (success && undoStack) {
        CircuitAction action = createAddConnectionAction(srcId, dstId, dstPin);
        pushAction(undoStack, &action);
    }
    return success;
}

bool executeUndoOperation(Circuit* circuit, CircuitUndoStack* undoStack) {
    if (!circuit || !undoStack) return false;

    CircuitAction action;
    if (!popAction(undoStack, &action)) {
        return false; // Underflow handled cleanly
    }

    std::cout << "\n>>> [LIFO UNDO EXECUTING] Inverting Action: " << action.description << " ...\n";
    switch (action.type) {
        case ACTION_ADD_GATE: {
            // Inverse of adding a gate is removing it
            circuit->removeGate(action.gateId);
            std::cout << "  -> Inverse Applied: Removed Gate ID " << action.gateId << " from circuit.\n";
            break;
        }
        case ACTION_DELETE_GATE: {
            // Inverse of deleting a gate is restoring it
            int defaultInputs[2] = {0, 0};
            Gate restoredGate(action.gateId, action.gateType, action.gateName, 2, defaultInputs);
            circuit->addGate(restoredGate);
            std::cout << "  -> Inverse Applied: Re-inserted Gate ID " << action.gateId << " into circuit.\n";
            break;
        }
        case ACTION_UPDATE_INPUT: {
            // Inverse of updating input is restoring the previous input value
            int idx = circuit->getGateRegistry().searchGateById(action.gateId);
            if (idx != -1) {
                Gate* g = circuit->getGateRegistry().getGateAt(idx);
                g->setInput(action.pinIndex, action.previousValue);
                g->evaluate();
                std::cout << "  -> Inverse Applied: Restored Gate " << action.gateId 
                          << " Pin " << action.pinIndex << " to previous value " << action.previousValue << ".\n";
            }
            break;
        }
        case ACTION_ADD_CONNECTION: {
            // Inverse of adding connection is removing it
            circuit->removeConnection(action.srcGateId, action.dstGateId, action.dstPin);
            std::cout << "  -> Inverse Applied: Removed Wire Gate " << action.srcGateId 
                      << " -> Gate " << action.dstGateId << " (Pin " << action.dstPin << ").\n";
            break;
        }
    }
    std::cout << ">>> [UNDO COMPLETE] Circuit state successfully restored to prior state.\n";
    return true;
}

// ------------------------------------------------------------------------------
// Procedural FIFO Simulation Event Queue (CO3)
// ------------------------------------------------------------------------------

void executeSimulationWithQueue(Circuit* circuit, SimulationQueue* queue) {
    if (!circuit || !queue) return;

    std::cout << "\n======================================================\n";
    std::cout << "  SIMULATION ENGINE: FIFO EVENT QUEUE EVALUATION      \n";
    std::cout << "======================================================\n";
    std::cout << "Chronological propagation of Boolean logic signals.\n";

    // Step 1: Initial evaluation of all gates and enqueueing initial output transitions
    int eventCounter = 1;
    for (int i = 0; i < circuit->getGateRegistry().getCount(); i++) {
        Gate* g = circuit->getGateRegistry().getGateAt(i);
        g->evaluate();

        // Find downstream connections for this gate's output
        for (int c = 0; c < circuit->getConnectionCount(); c++) {
            const Connection* conn = circuit->getConnectionAt(c);
            if (conn && conn->sourceGateId == g->getId()) {
                char desc[80];
                std::snprintf(desc, sizeof(desc), "Propagate G%d.out (%d) -> G%d[Pin %d]", 
                              g->getId(), g->getOutput(), conn->destinationGateId, conn->destinationInputIndex);
                SimulationEvent ev = createSimulationEvent(eventCounter++, conn->destinationGateId, 
                                                           conn->destinationInputIndex, g->getOutput(), 1, desc);
                enqueueEvent(queue, &ev);
            }
        }
    }

    displaySimulationQueue(queue);

    // Step 2: FIFO processing of events until queue is empty (signals settle)
    int processedCount = 0;
    while (!isQueueEmpty(queue)) {
        SimulationEvent currentEvent;
        dequeueEvent(queue, &currentEvent);
        processedCount++;

        std::cout << " [FIFO Step " << processedCount << "] Processing Event #" 
                  << currentEvent.eventId << ": Assigning Gate " << currentEvent.targetGateId 
                  << " Pin " << currentEvent.targetPinIndex << " = " << currentEvent.signalValue << "\n";

        int dstIdx = circuit->getGateRegistry().searchGateById(currentEvent.targetGateId);
        if (dstIdx != -1) {
            Gate* targetGate = circuit->getGateRegistry().getGateAt(dstIdx);
            int prevOutput = targetGate->getOutput();
            targetGate->setInput(currentEvent.targetPinIndex, currentEvent.signalValue);
            int newOutput = targetGate->evaluate();

            std::cout << "   -> Evaluated Gate ID " << targetGate->getId() << " (" 
                      << targetGate->getType() << "): Output = " << newOutput;
            if (prevOutput != newOutput) {
                std::cout << " (TRANSITION: " << prevOutput << " -> " << newOutput << ")\n";
                // Enqueue downstream events
                for (int c = 0; c < circuit->getConnectionCount(); c++) {
                    const Connection* conn = circuit->getConnectionAt(c);
                    if (conn && conn->sourceGateId == targetGate->getId()) {
                        char desc[80];
                        std::snprintf(desc, sizeof(desc), "Ripple G%d.out (%d) -> G%d[Pin %d]", 
                                      targetGate->getId(), newOutput, conn->destinationGateId, conn->destinationInputIndex);
                        SimulationEvent downstreamEv = createSimulationEvent(
                            eventCounter++, conn->destinationGateId, 
                            conn->destinationInputIndex, newOutput, 
                            currentEvent.simulationStep + 1, desc
                        );
                        enqueueEvent(queue, &downstreamEv);
                        std::cout << "      [Downstream Enqueued] " << desc << "\n";
                    }
                }
            } else {
                std::cout << " (Steady State: No output change)\n";
            }
        }
    }

    std::cout << "\n>>> FIFO Queue Signal Propagation Complete. Total Events Processed: " 
              << processedCount << " <<<\n";
    std::cout << "======================================================\n\n";
}

