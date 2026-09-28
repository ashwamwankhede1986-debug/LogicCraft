#ifndef TRUTH_TABLE_H
#define TRUTH_TABLE_H

#include "../Gates/Gate.h"

/**
 * @file TruthTable.h
 * @brief Truth Table Generator using Arrays (CO1 - Arrays)
 *
 * Demonstrates:
 * - Generating all 2^N Boolean input combinations using arrays
 * - Storing rows in 2D array representation
 * - Storing outputs in array
 * - Evaluating gate/circuit behavior across all input permutations
 */

const int MAX_TRUTH_TABLE_VARS = 4;
const int MAX_TRUTH_TABLE_ROWS = 16; // 2^4 = 16

class TruthTable {
private:
    int numInputs;
    int numRows;
    int inputCombinations[MAX_TRUTH_TABLE_ROWS][MAX_TRUTH_TABLE_VARS]; // 2D array of inputs
    int outputs[MAX_TRUTH_TABLE_ROWS];                                 // 1D array of outputs
    char circuitName[64];

public:
    TruthTable();
    TruthTable(const char* name, int inputsCount);

    // Generate truth table using array bit-manipulation
    void generateForGate(Gate& gate);
    void generateFor2InputOperation(const char* opName);

    // Display formatted table
    void display() const;

    // Accessors
    int getNumRows() const;
    int getInput(int row, int col) const;
    int getOutput(int row) const;
};

#endif // TRUTH_TABLE_H
