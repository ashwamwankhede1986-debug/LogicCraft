#ifndef GATE_H
#define GATE_H

#include <iostream>
#include <cstring>

/**
 * @file Gate.h
 * @brief LogicCraft Gate Representation for C++ Programming Lab (CO1 Arrays & Component Model)
 *
 * Encapsulates gate identity, logic type, input array, and evaluated output.
 */

const int MAX_INPUTS = 8;

class Gate {
private:
    int id;
    char type[16];   // "AND", "OR", "NOT", "XOR", etc.
    char name[32];   // User-defined label (e.g., "Carry_Gate")
    int inputCount;
    int inputs[MAX_INPUTS]; // Boolean values (0 or 1) stored in an array
    int output;             // Evaluated Boolean output (0 or 1)

public:
    Gate();
    Gate(int gateId, const char* gateType, const char* gateName, int inCount, const int initialInputs[]);

    // Logic Evaluation
    int evaluate();

    // Getters and Setters
    int getId() const;
    const char* getType() const;
    const char* getName() const;
    int getInputCount() const;
    int getInput(int index) const;
    int getOutput() const;

    void setId(int newId);
    void setType(const char* newType);
    void setName(const char* newName);
    void setInput(int index, int val);
    void setInputs(const int newInputs[], int count);

    // Display
    void display() const;
};

#endif // GATE_H
