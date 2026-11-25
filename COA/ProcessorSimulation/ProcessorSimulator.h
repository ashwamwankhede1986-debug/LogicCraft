#ifndef PROCESSOR_SIMULATOR_H
#define PROCESSOR_SIMULATOR_H

#include "../BooleanOperations/BooleanALU.h"
#include "../ControlUnit/ControlUnit.h"
#include "../Pipeline/PipelineSimulator.h"
#include <cstdint>
#include <string>
#include <vector>

/**
 * @file ProcessorSimulator.h
 * @brief Processor-Level Hardware Architecture Simulator (CO1, CO2, CO3, CO4)
 *
 * Demonstrates:
 * - General Purpose Register File: R0 (hardwired 0), R1, R2, R3, R4, R5, R6, R7
 * - Program Counter (PC) and Instruction Register (IR)
 * - Condition Code / Status Flags Register (ZF, SF, CF, OF)
 * - 256-Word Data Memory & Instruction Memory
 * - Hardwired & Microprogrammed Control Unit Integration
 * - Step-by-step register-transfer level (RTL) trace of LogicCraft operations
 * - Execution of the 3 core LogicCraft processor-level functionalities:
 *    1. Logic Gate Evaluation (AND, OR, NOT, NAND, NOR, XOR, XNOR)
 *    2. Propagation Delay Calculation via Successive Addition Loop
 *    3. Netlist Gate Type ASCII Opcode Decoding
 * - Five-Stage Instruction Pipeline (IF, ID, EX, MEM, WB) execution & timing matrix
 */

class ProcessorSimulator {
private:
    // General Purpose Register File R0..R7
    uint64_t registers[8];
    uint64_t PC;     // Program Counter
    uint32_t IR;     // Instruction Register
    ALUFlags flags;  // Condition Code / Status Flags Register

    // Memory Subsystem
    uint64_t dataMemory[256];

    // Control Unit & Pipeline
    ControlUnit controlUnit;
    ControlSignals lastControlSignals;
    PipelineSimulator pipeline;

public:
    ProcessorSimulator();

    void reset();

    // Register Transfer Operations (Backward compatible with CO1)
    void loadR1(uint64_t value);
    void loadR2(uint64_t value);
    void setRegister(uint8_t regNum, uint64_t value);
    uint64_t getRegister(uint8_t regNum) const;

    // Memory Access
    void writeMem(uint32_t addr, uint64_t value);
    uint64_t readMem(uint32_t addr) const;

    // ALU Instruction Execution with Control Unit Signal Generation
    void executeALU(ALUOpcode op);

    // Formatted Register Dump & Control Signal Display
    void displayState(const char* stepName = nullptr) const;
    void displayControlSignals(const ControlSignals& sigs) const;

    // Original Logic Gate simulation (CO1)
    void simulateLogicGate(const char* gateType, uint64_t operandA, uint64_t operandB = 0);

    // =========================================================================
    // THREE MAJOR LOGICCRAFT FUNCTIONALITIES EXECUTED AT PROCESSOR LEVEL (CO4)
    // =========================================================================

    // Functionality 1: Logic Gate Evaluation via ALU & 64-bit Assembly
    void demonstrateLogicGateFunctionality(const char* gateName, uint64_t opA, uint64_t opB = 0);

    // Functionality 2: Circuit Propagation Delay via Successive Addition Multiplication Loop
    uint64_t demonstratePropagationDelayMultiplication(uint64_t delayPerGate, uint64_t gateCount);

    // Functionality 3: Netlist Gate Name String Parsing & Opcode Decoding
    uint64_t demonstrateNetlistOpcodeDecoding(const char* gateNameStr);

    // Five-Stage Instruction Pipeline Demonstration
    void demonstrateFiveStagePipeline();

    // Control Unit Comparison Demonstration (Hardwired vs Microprogrammed)
    void demonstrateControlTechniques();

    // Getters
    uint64_t getR1() const;
    uint64_t getR2() const;
    uint64_t getR3() const;
    uint64_t getPC() const;
    uint32_t getIR() const;
    bool getZeroFlag() const;
    bool getSignFlag() const;
    bool getCarryFlag() const;
    bool getOverflowFlag() const;
    const ControlSignals& getLastControlSignals() const;
    PipelineSimulator& getPipeline();
};

#endif // PROCESSOR_SIMULATOR_H