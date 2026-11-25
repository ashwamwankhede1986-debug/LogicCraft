#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include "../BooleanOperations/BooleanALU.h"
#include <cstdint>
#include <string>
#include <vector>

/**
 * @file ControlUnit.h
 * @brief Comprehensive Control Unit Architecture for LogicCraft (COA CO4)
 *
 * Implements:
 * 1. Control Signal Generation: RegWrite, ALUSrc, MemRead, MemWrite, Branch, MemtoReg, RegDst, ALUOp
 * 2. Instruction Decoding: Opcode identification and ALU selection
 * 3. Hardwired Control Model: Combinational logic gate simulation
 * 4. Microprogrammed Control Model: ROM control memory, Micro-Sequencer & CAR
 * 5. Control Signal Truth Table and Architectural Comparison
 */

// Instruction Formats supported by LogicCraft Datapath
enum InstructionFormat {
    FORMAT_R_TYPE, // Register-to-register (ALU logic and arithmetic gates)
    FORMAT_I_TYPE, // Immediate, Load/Store, Branch
    FORMAT_J_TYPE  // Jump instructions
};

// Machine Opcode Identifiers (6-bit field representation)
enum CPUOpcode {
    OPC_R_TYPE = 0x00, // Funct determines ALU operation
    OPC_ADDI   = 0x08, // Add Immediate
    OPC_LW     = 0x23, // Load Word from Memory
    OPC_SW     = 0x2B, // Store Word to Memory
    OPC_BEQ    = 0x04, // Branch on Equal (Zero Flag == 1)
    OPC_BNE    = 0x05, // Branch on Not Equal (Zero Flag == 0)
    OPC_JMP    = 0x02  // Unconditional Jump
};

// Micro-architectural Control Signals Bus
struct ControlSignals {
    bool RegDst;          // 0 = rt (bits 20-16), 1 = rd (bits 15-11)
    bool ALUSrc;          // 0 = Register operand, 1 = Immediate operand
    bool MemtoReg;        // 0 = ALU Result, 1 = Memory Read Data
    bool RegWrite;        // 1 = Write back to register file enabled
    bool MemRead;         // 1 = Read from data memory enabled
    bool MemWrite;        // 1 = Write to data memory enabled
    bool Branch;          // 1 = Instruction is conditional branch
    bool Jump;            // 1 = Instruction is unconditional jump
    uint8_t ALUOp;        // 2-bit ALU operation class (00=Add/Mem, 01=Sub/Branch, 10=R-type, 11=Custom)
    ALUOpcode aluControl; // Exact decoded 4-bit ALU operation code passed to BooleanALU
};

// Internal 32-bit/64-bit Instruction Representation
struct InstructionWord {
    uint8_t opcode;        // 6-bit primary opcode
    uint8_t rs;            // Source register 1 index (0..7)
    uint8_t rt;            // Source register 2 index / target (0..7)
    uint8_t rd;            // Destination register index (0..7)
    uint8_t funct;         // Function field for R-type (maps to ALUOpcode)
    int16_t imm;           // 16-bit immediate / displacement value
    uint32_t target;       // 26-bit jump target address
    std::string mnemonic;  // Assembly text string, e.g. "AND R3, R1, R2"
};

// Micro-Instruction Word Format for Microprogrammed Control
struct MicroInstruction {
    uint16_t microAddress;      // Control ROM address
    std::string microMnemonic;  // Description of micro-operation
    ControlSignals signals;     // Asserted control signals for this micro-cycle
    uint16_t nextAddress;       // Default next micro-address
    uint8_t branchCondition;    // 0 = Next, 1 = On Zero Flag, 2 = Opcode Dispatch, 3 = Fetch Return
};

// Hardwired Control Unit (Combinational Logic Decoder)
class HardwiredControl {
public:
    static ControlSignals decode(const InstructionWord& instr);
    static void printArchitectureDiagram();
};

// Microprogrammed Control Unit (ROM Store & Micro-Sequencer)
class MicroprogrammedControl {
private:
    std::vector<MicroInstruction> controlMemory;
    uint16_t CAR; // Control Address Register

    void initializeMicrocodeROM();

public:
    MicroprogrammedControl();
    ControlSignals executeMicrocode(const InstructionWord& instr, bool zeroFlag = false);
    uint16_t getCAR() const { return CAR; }
    void printMicrocodeROM() const;
    static void printArchitectureDiagram();
};

// Master Control Unit Interface
class ControlUnit {
private:
    HardwiredControl hardwiredUnit;
    MicroprogrammedControl microprogrammedUnit;

public:
    ControlUnit();

    // Primary instruction decoding and signal generation
    ControlSignals generateSignals(const InstructionWord& instr, bool useMicroprogrammed = false);
    ControlSignals decode(const InstructionWord& instr, bool useMicroprogrammed = false) {
        return generateSignals(instr, useMicroprogrammed);
    }

    // ALU Control derivation from ALUOp and Funct
    static ALUOpcode deriveALUControl(uint8_t ALUOp, uint8_t funct);

    // Helper builders for LogicCraft instructions
    static InstructionWord createRType(const char* name, uint8_t rd, uint8_t rs, uint8_t rt);
    static InstructionWord createIType(CPUOpcode op, uint8_t rt, uint8_t rs, int16_t imm, const char* name);
    static InstructionWord createBranch(CPUOpcode op, uint8_t rs, uint8_t rt, int16_t offset, const char* name);

    // Demonstration and Documentation outputs
    static void printControlSignalTable();
    static void compareControlTechniques();
};

#endif // CONTROL_UNIT_H
