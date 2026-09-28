#include "ProcessorSimulator.h"
#include <iostream>
#include <iomanip>

ProcessorSimulator::ProcessorSimulator() {
    reset();
}

void ProcessorSimulator::reset() {
    R1 = 0;
    R2 = 0;
    R3 = 0;
    PC = 0x00400000;
    IR = 0;
    flags.zeroFlag = false;
    flags.signFlag = false;
    flags.carryFlag = false;
}

void ProcessorSimulator::loadR1(uint64_t value) {
    R1 = value;
    std::cout << " [RTL Trace] MOV R1, " << value << "  (Loaded Operand A into Register R1)\n";
}

void ProcessorSimulator::loadR2(uint64_t value) {
    R2 = value;
    std::cout << " [RTL Trace] MOV R2, " << value << "  (Loaded Operand B into Register R2)\n";
}

void ProcessorSimulator::executeALU(ALUOpcode op) {
    IR = (uint32_t)op;
    PC += 4; // Increment Program Counter by 4 bytes (instruction size)

    std::cout << " [RTL Trace] EXEC ALU_" << BooleanALU::getOpcodeName(op) 
              << " R3, R1, R2  (ALU computed operation, stored into Register R3)\n";

    R3 = BooleanALU::execute(op, R1, R2, flags);

    std::cout << " [RTL Trace] UPDATE FLAGS -> ZF: " << (flags.zeroFlag ? "1" : "0")
              << ", SF: " << (flags.signFlag ? "1" : "0") << "\n";
}

void ProcessorSimulator::displayState(const char* stepName) const {
    if (stepName) {
        std::cout << "\n--- Processor State: " << stepName << " ---\n";
    }
    std::cout << "+-------------------------------------------------------------+\n";
    std::cout << "|  PC: 0x" << std::hex << std::setw(8) << std::setfill('0') << PC << std::dec << "  |  IR: 0x" << std::hex << std::setw(4) << std::setfill('0') << IR << std::dec << "  |  OP: " << std::left << std::setw(6) << std::setfill(' ') << BooleanALU::getOpcodeName((ALUOpcode)IR) << " |\n";
    std::cout << "+-------------------------------------------------------------+\n";
    std::cout << "|  R1 (Operand A): " << std::setw(2) << R1 
              << "  |  R2 (Operand B): " << std::setw(2) << R2 
              << "  |  R3 (Result): " << std::setw(2) << R3 << "   |\n";
    std::cout << "|  FLAGS -> [ZF (Zero): " << (flags.zeroFlag ? "1" : "0") 
              << "] [SF (Sign): " << (flags.signFlag ? "1" : "0") 
              << "] [CF (Carry): " << (flags.carryFlag ? "1" : "0") << "]           |\n";
    std::cout << "+-------------------------------------------------------------+\n";
}

void ProcessorSimulator::simulateLogicGate(const char* gateType, uint64_t operandA, uint64_t operandB) {
    std::cout << "\n===============================================================\n";
    std::cout << " SIMULATING LOGIC GATE AT PROCESSOR LEVEL: " << gateType << "\n";
    std::cout << " Inputs: A = " << operandA << ", B = " << operandB << "\n";
    std::cout << "===============================================================\n";

    reset();
    displayState("Initial Reset");

    // Micro-operation 1: Fetch & Load operand A
    loadR1(operandA);
    // Micro-operation 2: Fetch & Load operand B
    if (BooleanALU::stringToOpcode(gateType) != OP_NOT) {
        loadR2(operandB);
    }

    // Micro-operation 3: ALU Execution
    ALUOpcode op = BooleanALU::stringToOpcode(gateType);
    executeALU(op);

    // Final Dump
    displayState("Execution Completed");
    std::cout << "Summary: R1=" << R1 << ", R2=" << R2 
              << ", OP=" << BooleanALU::getOpcodeName(op) 
              << ", R3=" << R3 
              << ", ZF=" << (flags.zeroFlag ? 1 : 0) << "\n";
}

uint64_t ProcessorSimulator::getR1() const { return R1; }
uint64_t ProcessorSimulator::getR2() const { return R2; }
uint64_t ProcessorSimulator::getR3() const { return R3; }
bool ProcessorSimulator::getZeroFlag() const { return flags.zeroFlag; }
