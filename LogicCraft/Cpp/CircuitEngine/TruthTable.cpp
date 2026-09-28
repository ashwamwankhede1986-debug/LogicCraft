#include "TruthTable.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>

TruthTable::TruthTable() : numInputs(2), numRows(4) {
    strcpy(circuitName, "Default Truth Table");
    memset(inputCombinations, 0, sizeof(inputCombinations));
    memset(outputs, 0, sizeof(outputs));
}

TruthTable::TruthTable(const char* name, int inputsCount) : numInputs(inputsCount) {
    strncpy(circuitName, name, sizeof(circuitName) - 1);
    circuitName[sizeof(circuitName) - 1] = '\0';
    
    if (numInputs > MAX_TRUTH_TABLE_VARS) numInputs = MAX_TRUTH_TABLE_VARS;
    numRows = 1 << numInputs; // 2^N rows
    memset(inputCombinations, 0, sizeof(inputCombinations));
    memset(outputs, 0, sizeof(outputs));
}

void TruthTable::generateForGate(Gate& gate) {
    numInputs = gate.getInputCount();
    if (numInputs > MAX_TRUTH_TABLE_VARS) numInputs = MAX_TRUTH_TABLE_VARS;
    numRows = 1 << numInputs;

    strncpy(circuitName, gate.getName(), sizeof(circuitName) - 1);
    circuitName[sizeof(circuitName) - 1] = '\0';

    // Populate rows using binary representation stored in array
    for (int r = 0; r < numRows; ++r) {
        for (int c = 0; c < numInputs; ++c) {
            // (r >> (numInputs - 1 - c)) & 1 extracts binary bit
            inputCombinations[r][c] = (r >> (numInputs - 1 - c)) & 1;
        }

        // Apply input combination to gate
        gate.setInputs(inputCombinations[r], numInputs);
        outputs[r] = gate.evaluate();
    }
}

void TruthTable::generateFor2InputOperation(const char* opName) {
    Gate tempGate(999, opName, opName, 2, nullptr);
    generateForGate(tempGate);
}

void TruthTable::display() const {
    std::cout << "\n+-------------------------------------------------------+\n";
    std::cout << "|   TRUTH TABLE FOR: " << std::left << std::setw(34) << circuitName << "|\n";
    std::cout << "+-------------------------------------------------------+\n| ";
    
    // Column Headers
    for (int i = 0; i < numInputs; ++i) {
        char varName = 'A' + i;
        std::cout << " " << varName << " |";
    }
    std::cout << "  OUTPUT  |\n";

    // Divider
    std::cout << "+";
    for (int i = 0; i < numInputs; ++i) {
        std::cout << "---+";
    }
    std::cout << "----------+\n";

    // Rows
    for (int r = 0; r < numRows; ++r) {
        std::cout << "| ";
        for (int c = 0; c < numInputs; ++c) {
            std::cout << " " << inputCombinations[r][c] << " |";
        }
        std::cout << "    " << outputs[r] << "     |\n";
    }
    std::cout << "+";
    for (int i = 0; i < numInputs; ++i) {
        std::cout << "---+";
    }
    std::cout << "----------+\n\n";
}

int TruthTable::getNumRows() const { return numRows; }
int TruthTable::getInput(int row, int col) const {
    if (row >= 0 && row < numRows && col >= 0 && col < numInputs) {
        return inputCombinations[row][col];
    }
    return 0;
}
int TruthTable::getOutput(int row) const {
    if (row >= 0 && row < numRows) return outputs[row];
    return 0;
}
