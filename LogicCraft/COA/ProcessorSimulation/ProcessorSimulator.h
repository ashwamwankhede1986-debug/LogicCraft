#ifndef PROCESSOR_SIMULATOR_H
#define PROCESSOR_SIMULATOR_H

#include "../BooleanOperations/BooleanALU.h"
#include <cstdint>

/**
 * @file ProcessorSimulator.h
 * @brief Processor-Level Hardware Architecture Simulator (CO1 - Processor Modeling)
 *
 * Demonstrates:
 * - General Purpose Registers: R1, R2, R3
 * - Program Counter (PC) and Instruction Register (IR)
 * - Status Flags Register (ZF, SF, CF)
 * - Step-by-step register-transfer level (RTL) trace of LogicCraft operations
 */

class ProcessorSimulator {
private:
    uint64_t R1;     // Register 1: Operand A
    uint64_t R2;     // Register 2: Operand B
    uint64_t R3;     // Register 3: Result / Accumulator
    uint64_t PC;     // Program Counter
    uint32_t IR;     // Instruction Register
    ALUFlags flags;  // Condition Code / Flags Register

public:
    ProcessorSimulator();

    void reset();

    // Register Transfer Operations
    void loadR1(uint64_t value);
    void loadR2(uint64_t value);
    
    // ALU Instruction Execution (CO1)
    void executeALU(ALUOpcode op);

    // Formatted Register Dump
    void displayState(const char* stepName = nullptr) const;

    // Run full annotated RTL simulation of a Logic Gate
    void simulateLogicGate(const char* gateType, uint64_t operandA, uint64_t operandB = 0);

    // Getters
    uint64_t getR1() const;
    uint64_t getR2() const;
    uint64_t getR3() const;
    bool getZeroFlag() const;
};

#endif // PROCESSOR_SIMULATOR_H
