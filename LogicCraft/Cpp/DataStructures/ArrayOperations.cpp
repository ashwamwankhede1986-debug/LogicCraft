#include "ArrayOperations.h"
#include <iostream>
#include <cstring>

GateArray::GateArray() : count(0) {}

bool GateArray::insertGate(const Gate& g) {
    if (count >= MAX_CIRCUIT_GATES) {
        std::cerr << "[Array Error] Capacity exceeded! Cannot insert gate ID " << g.getId() << "\n";
        return false;
    }
    // Check for duplicate ID
    if (searchGateById(g.getId()) != -1) {
        std::cerr << "[Array Error] Gate with ID " << g.getId() << " already exists!\n";
        return false;
    }
    gates[count] = g;
    count++;
    std::cout << "[Array Insert] Successfully inserted gate ID " << g.getId() 
              << " (" << g.getType() << ") at array index " << (count - 1) << ".\n";
    return true;
}

bool GateArray::deleteGate(int gateId) {
    int index = searchGateById(gateId);
    if (index == -1) {
        std::cerr << "[Array Delete] Gate ID " << gateId << " not found in array.\n";
        return false;
    }

    // Shift left to preserve contiguous array storage
    for (int i = index; i < count - 1; ++i) {
        gates[i] = gates[i + 1];
    }
    count--;
    std::cout << "[Array Delete] Successfully deleted gate ID " << gateId 
              << " from array. Remaining elements: " << count << ".\n";
    return true;
}

bool GateArray::updateGate(int gateId, const char* newName, const int newInputs[], int newCount) {
    int index = searchGateById(gateId);
    if (index == -1) {
        std::cerr << "[Array Update] Gate ID " << gateId << " not found.\n";
        return false;
    }

    if (newName != nullptr && strlen(newName) > 0) {
        gates[index].setName(newName);
    }
    if (newInputs != nullptr && newCount > 0) {
        gates[index].setInputs(newInputs, newCount);
    }
    std::cout << "[Array Update] Successfully updated gate ID " << gateId 
              << ". New state: ";
    gates[index].display();
    return true;
}

int GateArray::searchGateById(int gateId) const {
    for (int i = 0; i < count; ++i) {
        if (gates[i].getId() == gateId) {
            return i;
        }
    }
    return -1;
}

int GateArray::searchGateByName(const char* name) const {
    if (name == nullptr) return -1;
    for (int i = 0; i < count; ++i) {
        if (strcmp(gates[i].getName(), name) == 0) {
            return i;
        }
    }
    return -1;
}

void GateArray::displayAll() const {
    std::cout << "\n======================================================\n";
    std::cout << "        LOGICCRAFT ARRAY COMPONENT REGISTRY (CO1)     \n";
    std::cout << "======================================================\n";
    std::cout << "Total Gates Stored in Contiguous Array: " << count << " / " << MAX_CIRCUIT_GATES << "\n";
    if (count == 0) {
        std::cout << "  (Array is currently empty)\n";
    }
    for (int i = 0; i < count; ++i) {
        std::cout << "Index [" << i << "] -> ";
        gates[i].display();
    }
    std::cout << "======================================================\n\n";
}

int GateArray::getCount() const { return count; }

const Gate* GateArray::getGateAt(int index) const {
    if (index >= 0 && index < count) return &gates[index];
    return nullptr;
}

Gate* GateArray::getGateAt(int index) {
    if (index >= 0 && index < count) return &gates[index];
    return nullptr;
}
