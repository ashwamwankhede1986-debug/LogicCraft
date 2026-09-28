#ifndef ARRAY_OPERATIONS_H
#define ARRAY_OPERATIONS_H

#include "../Gates/Gate.h"

/**
 * @file ArrayOperations.h
 * @brief LogicCraft Array-Based Component Registry (CO1 - Arrays)
 *
 * Implements fundamental array-based operations:
 * - Insert gate record into array
 * - Delete gate record from array
 * - Update gate attributes in array
 * - Search gate record by ID or Name
 * - Display all records stored in array
 */

const int MAX_CIRCUIT_GATES = 32;

class GateArray {
private:
    Gate gates[MAX_CIRCUIT_GATES]; // Fixed-size array storing Gate records
    int count;                     // Current number of elements in array

public:
    GateArray();

    // Fundamental Array Operations
    bool insertGate(const Gate& g);
    bool deleteGate(int gateId);
    bool updateGate(int gateId, const char* newName, const int newInputs[], int newCount);
    int searchGateById(int gateId) const;
    int searchGateByName(const char* name) const;
    void displayAll() const;

    // Accessors
    int getCount() const;
    const Gate* getGateAt(int index) const;
    Gate* getGateAt(int index);
};

#endif // ARRAY_OPERATIONS_H
