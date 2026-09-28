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

GateArray& Circuit::getGateRegistry() {
    return gateRegistry;
}
