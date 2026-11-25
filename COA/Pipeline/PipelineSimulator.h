#ifndef PIPELINE_SIMULATOR_H
#define PIPELINE_SIMULATOR_H

#include "../ControlUnit/ControlUnit.h"
#include "../BooleanOperations/BooleanALU.h"
#include <cstdint>
#include <string>
#include <vector>
#include <map>

/**
 * @file PipelineSimulator.h
 * @brief Five-Stage Instruction Pipeline Simulator for LogicCraft (COA CO4)
 *
 * Implements:
 * 1. Five discrete pipeline stages: IF, ID, EX, MEM, WB
 * 2. Pipeline Registers: IF/ID, ID/EX, EX/MEM, MEM/WB
 * 3. Forwarding Unit (Resolves RAW Data Hazards without stalls when possible)
 * 4. Hazard Detection Unit (Stalls 1 cycle on Load-Use data hazards)
 * 5. Branch Flush Handling (Control Hazards)
 * 6. Cycle-by-cycle Execution Timing Matrix & Statistics
 */

// Pipeline Stage Enumeration
enum PipelineStage {
    STAGE_NONE = 0,
    STAGE_IF,
    STAGE_ID,
    STAGE_EX,
    STAGE_MEM,
    STAGE_WB,
    STAGE_STALL
};

// 1. IF/ID Pipeline Register
struct IF_ID_Register {
    bool valid;
    InstructionWord instr;
    uint64_t pc;
    int instrIndex;

    void clear() {
        valid = false;
        instr = InstructionWord();
        pc = 0;
        instrIndex = -1;
    }
};

// 2. ID/EX Pipeline Register
struct ID_EX_Register {
    bool valid;
    bool isBubble;
    InstructionWord instr;
    uint64_t pc;
    int instrIndex;
    uint64_t readData1;
    uint64_t readData2;
    uint64_t signExtImm;
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
    ControlSignals ctrl;

    void clear() {
        valid = false;
        isBubble = false;
        instr = InstructionWord();
        pc = 0;
        instrIndex = -1;
        readData1 = 0;
        readData2 = 0;
        signExtImm = 0;
        rs = 0;
        rt = 0;
        rd = 0;
        ctrl = ControlSignals();
    }
};

// 3. EX/MEM Pipeline Register
struct EX_MEM_Register {
    bool valid;
    bool isBubble;
    InstructionWord instr;
    uint64_t pc;
    int instrIndex;
    uint64_t aluResult;
    uint64_t writeData;
    uint8_t destReg;
    ALUFlags flags;
    uint64_t branchTarget;
    bool branchTaken;
    ControlSignals ctrl;

    void clear() {
        valid = false;
        isBubble = false;
        instr = InstructionWord();
        pc = 0;
        instrIndex = -1;
        aluResult = 0;
        writeData = 0;
        destReg = 0;
        flags = ALUFlags();
        branchTarget = 0;
        branchTaken = false;
        ctrl = ControlSignals();
    }
};

// 4. MEM/WB Pipeline Register
struct MEM_WB_Register {
    bool valid;
    InstructionWord instr;
    uint64_t pc;
    int instrIndex;
    uint64_t aluResult;
    uint64_t memReadData;
    uint8_t destReg;
    ControlSignals ctrl;

    void clear() {
        valid = false;
        instr = InstructionWord();
        pc = 0;
        instrIndex = -1;
        aluResult = 0;
        memReadData = 0;
        destReg = 0;
        ctrl = ControlSignals();
    }
};

// Struct tracking stage state for cycle-by-cycle timing matrix
struct StageHistory {
    int cycle;
    int instrIndex;
    std::string mnemonic;
    PipelineStage stage;
    std::string annotation; // e.g., "Fwd from EX/MEM", "Bubble/Stall"
};

// Forwarding Control Selector Values
enum ForwardMux {
    FORWARD_NONE = 0,     // 00: Operand from ID/EX register
    FORWARD_FROM_WB = 1,  // 01: Operand forwarded from MEM/WB (or WB write data)
    FORWARD_FROM_MEM = 2  // 10: Operand forwarded from EX/MEM register
};

class PipelineSimulator {
private:
    // Architectural Resources
    uint64_t regFile[8];       // R0 (hardwired to 0), R1, R2, R3, R4, R5, R6, R7
    uint64_t dataMemory[256];   // 256 x 64-bit word Data Memory
    std::vector<InstructionWord> instructionMemory;
    uint64_t PC;

    // Pipeline Registers
    IF_ID_Register IF_ID;
    ID_EX_Register ID_EX;
    EX_MEM_Register EX_MEM;
    MEM_WB_Register MEM_WB;

    // Next State Buffers (for synchronous clock edge transitions)
    IF_ID_Register next_IF_ID;
    ID_EX_Register next_ID_EX;
    EX_MEM_Register next_EX_MEM;
    MEM_WB_Register next_MEM_WB;

    // Hazards & Telemetry
    int currentCycle;
    int totalStalls;
    int totalForwardEvents;
    int totalFlushes;
    bool stallRequested;
    bool branchFlushRequested;
    uint64_t branchFlushTarget;

    ForwardMux forwardA;
    ForwardMux forwardB;

    // History tracking for timing table
    std::vector<StageHistory> history;

    // Internal Stage Handlers
    void stageWB();
    void stageMEM();
    void stageEX();
    void stageID();
    void stageIF();

    void evaluateHazardDetection();
    void evaluateForwardingUnit();

public:
    PipelineSimulator();

    void reset();
    void loadProgram(const std::vector<InstructionWord>& program);
    void writeDataMem(uint32_t address, uint64_t value);
    uint64_t readDataMem(uint32_t address) const;

    void setRegister(uint8_t regNum, uint64_t value);
    uint64_t getRegister(uint8_t regNum) const;

    // Step a single clock cycle; returns true if instructions are still in flight
    bool stepCycle();

    // Run program to completion or until maxCycles
    void run(int maxCycles = 30);

    // Formatted outputs and telemetry reports
    void printPipelineTimingMatrix() const;
    void printRegisterFile() const;
    void printHazardAnalysis() const;
    void printPipelineArchitectureDiagram() const;

    // Telemetry Getters
    int getCurrentCycle() const { return currentCycle; }
    int getTotalStalls() const { return totalStalls; }
    int getTotalForwardEvents() const { return totalForwardEvents; }
    int getTotalFlushes() const { return totalFlushes; }
    double getIPC() const;
};

#endif // PIPELINE_SIMULATOR_H