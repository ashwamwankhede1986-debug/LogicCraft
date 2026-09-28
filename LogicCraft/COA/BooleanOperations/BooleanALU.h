#ifndef BOOLEAN_ALU_H
#define BOOLEAN_ALU_H

#include <cstdint>

/**
 * @file BooleanALU.h
 * @brief Arithmetic Logic Unit (ALU) Hardware Model for LogicCraft (CO1 - Processor Modeling)
 *
 * Implements micro-operations for Boolean Logic execution:
 * - Opcode decoding (AND, OR, NOT, XOR, NAND, NOR)
 * - Flag generation (Zero Flag ZF, Carry/Borrow Flag, Sign Flag)
 */

enum ALUOpcode {
    OP_AND  = 0x01,
    OP_OR   = 0x02,
    OP_NOT  = 0x03,
    OP_XOR  = 0x04,
    OP_NAND = 0x05,
    OP_NOR  = 0x06
};

struct ALUFlags {
    bool zeroFlag;     // ZF: Set to 1 if result is 0
    bool signFlag;     // SF: Set to 1 if MSB is 1
    bool carryFlag;    // CF: Set if overflow/carry
};

class BooleanALU {
public:
    static uint64_t execute(ALUOpcode opcode, uint64_t operandA, uint64_t operandB, ALUFlags& flags);
    static const char* getOpcodeName(ALUOpcode opcode);
    static ALUOpcode stringToOpcode(const char* name);
};

#endif // BOOLEAN_ALU_H
