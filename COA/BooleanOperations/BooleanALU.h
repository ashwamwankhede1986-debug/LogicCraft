#ifndef BOOLEAN_ALU_H
#define BOOLEAN_ALU_H

#include <cstdint>

/**
 * @file BooleanALU.h
 * @brief Arithmetic Logic Unit (ALU) Hardware Model for LogicCraft (CO1 & CO4)
 *
 * Implements micro-operations for 64-bit Boolean Logic and Arithmetic execution:
 * - Logical Operations: AND, OR, NOT, XOR, NAND, NOR, XNOR
 * - Arithmetic Operations: ADD, SUB, COMPARE
 * - Flag Generation: Zero Flag (ZF), Sign Flag (SF), Carry/Borrow Flag (CF), Overflow Flag (OF)
 */

enum ALUOpcode {
    OP_AND  = 0x01,
    OP_OR   = 0x02,
    OP_NOT  = 0x03,
    OP_XOR  = 0x04,
    OP_NAND = 0x05,
    OP_NOR  = 0x06,
    OP_XNOR = 0x07,
    OP_ADD  = 0x08,
    OP_SUB  = 0x09,
    OP_CMP  = 0x0A,
    OP_FAST_ADD = 0x0B,
    OP_BOOTH_MUL = 0x0C
};

struct ALUFlags {
    bool zeroFlag;     // ZF: Set to 1 if result is 0
    bool signFlag;     // SF: Set to 1 if MSB (bit 63) is 1
    bool carryFlag;    // CF: Set if arithmetic overflow / borrow occurs
    bool overflowFlag; // OF: Set if signed two's-complement overflow occurs
};

class BooleanALU {
public:
    static uint64_t execute(ALUOpcode opcode, uint64_t operandA, uint64_t operandB, ALUFlags& flags);
    static const char* getOpcodeName(ALUOpcode opcode);
    static ALUOpcode stringToOpcode(const char* name);
    static void printALUDiagram();

    // COA Academic Implementations
    static uint64_t carryLookaheadAdd(uint64_t operandA, uint64_t operandB, bool carryIn, ALUFlags& flags, int bitWidth = 64);
    static int64_t boothMultiply(int64_t multiplicand, int64_t multiplier, ALUFlags& flags, int bitWidth = 32);
};

#endif // BOOLEAN_ALU_H
