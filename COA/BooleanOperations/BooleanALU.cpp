#include "BooleanALU.h"
#include <cstring>
#include <iostream>

uint64_t BooleanALU::execute(ALUOpcode opcode, uint64_t operandA, uint64_t operandB, ALUFlags& flags) {
    uint64_t result = 0;
    flags.zeroFlag = false;
    flags.signFlag = false;
    flags.carryFlag = false;
    flags.overflowFlag = false;

    // Check if operands are 1-bit boolean or multi-bit words
    bool is1Bit = (operandA <= 1ULL) && (operandB <= 1ULL);

    switch (opcode) {
        case OP_AND:
            result = operandA & operandB;
            break;

        case OP_OR:
            result = operandA | operandB;
            break;

        case OP_NOT:
            result = is1Bit ? ((~operandA) & 1ULL) : (~operandA);
            break;

        case OP_XOR:
            result = operandA ^ operandB;
            break;

        case OP_NAND:
            result = is1Bit ? ((~(operandA & operandB)) & 1ULL) : (~(operandA & operandB));
            break;

        case OP_NOR:
            result = is1Bit ? ((~(operandA | operandB)) & 1ULL) : (~(operandA | operandB));
            break;

        case OP_XNOR:
            result = is1Bit ? ((~(operandA ^ operandB)) & 1ULL) : (~(operandA ^ operandB));
            break;

        case OP_ADD: {
            result = operandA + operandB;
            // Carry Flag: set if unsigned sum wrapped around
            flags.carryFlag = (result < operandA);
            // Overflow Flag: set if signed operands had same sign and result has opposite sign
            flags.overflowFlag = (((operandA ^ result) & (operandB ^ result)) >> 63) & 1ULL;
            break;
        }

        case OP_SUB: {
            result = operandA - operandB;
            // Carry/Borrow Flag: set if operandA < operandB
            flags.carryFlag = (operandA < operandB);
            // Overflow Flag for signed subtraction
            flags.overflowFlag = (((operandA ^ operandB) & (operandA ^ result)) >> 63) & 1ULL;
            break;
        }

        case OP_CMP: {
            // Compare performs operandA - operandB, setting flags without altering destination
            result = operandA - operandB;
            flags.carryFlag = (operandA < operandB);
            flags.overflowFlag = (((operandA ^ operandB) & (operandA ^ result)) >> 63) & 1ULL;
            flags.zeroFlag = (operandA == operandB);
            flags.signFlag = ((int64_t)operandA < (int64_t)operandB);
            return (operandA == operandB) ? 0 : ((operandA > operandB) ? 1 : 0xFFFFFFFFFFFFFFFFULL);
        }

        case OP_FAST_ADD: {
            result = carryLookaheadAdd(operandA, operandB, false, flags, 64);
            return result;
        }

        case OP_BOOTH_MUL: {
            int64_t prod = boothMultiply((int64_t)operandA, (int64_t)operandB, flags, 32);
            return (uint64_t)prod;
        }

        default:
            result = 0;
            break;
    }

    // Update Common Processor Status Flags
    flags.zeroFlag = (result == 0ULL);
    flags.signFlag = ((result >> 63) & 1ULL) != 0;

    return result;
}

const char* BooleanALU::getOpcodeName(ALUOpcode opcode) {
    switch (opcode) {
        case OP_AND:  return "AND";
        case OP_OR:   return "OR";
        case OP_NOT:  return "NOT";
        case OP_XOR:  return "XOR";
        case OP_NAND: return "NAND";
        case OP_NOR:  return "NOR";
        case OP_XNOR: return "XNOR";
        case OP_ADD:  return "ADD";
        case OP_SUB:  return "SUB";
        case OP_CMP:  return "CMP";
        case OP_FAST_ADD: return "FAST_ADD (CLA)";
        case OP_BOOTH_MUL: return "BOOTH_MUL";
        default:      return "UNKNOWN";
    }
}

ALUOpcode BooleanALU::stringToOpcode(const char* name) {
    if (!name) return OP_AND;
    if (strcmp(name, "AND") == 0 || strcmp(name, "and") == 0) return OP_AND;
    if (strcmp(name, "OR") == 0  || strcmp(name, "or") == 0)  return OP_OR;
    if (strcmp(name, "NOT") == 0 || strcmp(name, "not") == 0) return OP_NOT;
    if (strcmp(name, "XOR") == 0 || strcmp(name, "xor") == 0) return OP_XOR;
    if (strcmp(name, "NAND") == 0 || strcmp(name, "nand") == 0) return OP_NAND;
    if (strcmp(name, "NOR") == 0  || strcmp(name, "nor") == 0)  return OP_NOR;
    if (strcmp(name, "XNOR") == 0 || strcmp(name, "xnor") == 0) return OP_XNOR;
    if (strcmp(name, "ADD") == 0  || strcmp(name, "add") == 0)  return OP_ADD;
    if (strcmp(name, "SUB") == 0  || strcmp(name, "sub") == 0)  return OP_SUB;
    if (strcmp(name, "CMP") == 0  || strcmp(name, "cmp") == 0 || strcmp(name, "COMPARE") == 0) return OP_CMP;
    if (strcmp(name, "FAST_ADD") == 0 || strcmp(name, "fast_add") == 0 || strcmp(name, "CLA") == 0) return OP_FAST_ADD;
    if (strcmp(name, "BOOTH") == 0 || strcmp(name, "booth") == 0 || strcmp(name, "BOOTH_MUL") == 0) return OP_BOOTH_MUL;
    return OP_AND;
}

void BooleanALU::printALUDiagram() {
    std::cout << "\n";
    std::cout << "        +-----------------------------------------------+\n";
    std::cout << "        |          64-BIT EXPANDED ALU ARCHITECTURE     |\n";
    std::cout << "        +-----------------------------------------------+\n";
    std::cout << "                     |                     |             \n";
    std::cout << "                     v                     v             \n";
    std::cout << "             [Operand A: 64-bit]   [Operand B: 64-bit]   \n";
    std::cout << "                     |                     |             \n";
    std::cout << "                     +----------+----------+             \n";
    std::cout << "                                |                        \n";
    std::cout << "       [ALU Selection] --------> \\                      \n";
    std::cout << "       OP_AND  (0x01)             \\                     \n";
    std::cout << "       OP_OR   (0x02)              \\                    \n";
    std::cout << "       OP_NOT  (0x03)               +----------------+   \n";
    std::cout << "       OP_XOR  (0x04)               |  ALU CORE      |   \n";
    std::cout << "       OP_NAND (0x05)               |  - Logic Block |   \n";
    std::cout << "       OP_NOR  (0x06)               |  - Adder/Sub   |   \n";
    std::cout << "       OP_XNOR (0x07)               |  - Comparator  |   \n";
    std::cout << "       OP_ADD  (0x08)               +----------------+   \n";
    std::cout << "       OP_SUB  (0x09)              /         |           \n";
    std::cout << "       OP_CMP  (0x0A)             /          |           \n";
    std::cout << "                                 /           v           \n";
    std::cout << "                     +----------+     [Result: 64-bit]   \n";
    std::cout << "                     |                                   \n";
    std::cout << "                     v                                   \n";
    std::cout << "          +-------------------------+                    \n";
    std::cout << "          | STATUS FLAGS REGISTER   |                    \n";
    std::cout << "          | ZF: Zero Flag           |                    \n";
    std::cout << "          | SF: Sign Flag           |                    \n";
    std::cout << "          | CF: Carry/Borrow Flag   |                    \n";
    std::cout << "          | OF: Overflow Flag       |                    \n";
    std::cout << "          +-------------------------+                    \n";
    std::cout << "\n";
}

/**
 * @brief Carry Lookahead Fast Adder (CLA)
 * Implements hardware Generate (G_i = A_i & B_i) and Propagate (P_i = A_i ^ B_i)
 * with parallel fast lookahead carry logic: C_{i+1} = G_i | (P_i & C_i).
 */
uint64_t BooleanALU::carryLookaheadAdd(uint64_t operandA, uint64_t operandB, bool carryIn, ALUFlags& flags, int bitWidth) {
    if (bitWidth <= 0 || bitWidth > 64) bitWidth = 64;

    uint64_t mask = (bitWidth == 64) ? ~0ULL : ((1ULL << bitWidth) - 1ULL);
    uint64_t signBit = 1ULL << (bitWidth - 1);
    uint64_t A = operandA & mask;
    uint64_t B = operandB & mask;

    uint64_t G = A & B;      // Generate signals for each bit
    uint64_t P = A ^ B;      // Propagate signals for each bit

    uint64_t S = 0;
    bool c = carryIn;
    bool prevCarry = false;

    for (int i = 0; i < bitWidth; ++i) {
        prevCarry = c;
        bool bitS = ((P >> i) & 1ULL) ^ c;
        if (bitS) S |= (1ULL << i);

        // Carry Lookahead computation for next stage
        bool g_i = (G >> i) & 1ULL;
        bool p_i = (P >> i) & 1ULL;
        c = g_i || (p_i && c);
    }

    S &= mask;

    // Status flags
    flags.carryFlag = c;
    // Overflow flag for signed two's complement addition: carry-in to MSB != carry-out from MSB
    flags.overflowFlag = (c ^ prevCarry);
    flags.zeroFlag = (S == 0ULL);
    flags.signFlag = (S & signBit) != 0;

    return S;
}

/**
 * @brief Booth's Signed Multiplication Algorithm
 * Uses Booth recoding and arithmetic right shifts (ARS) with 2's complement operands.
 */
int64_t BooleanALU::boothMultiply(int64_t multiplicand, int64_t multiplier, ALUFlags& flags, int bitWidth) {
    if (bitWidth <= 0 || bitWidth > 32) bitWidth = 32;

    int64_t mask = (1ULL << bitWidth) - 1ULL;
    int64_t signBit = 1ULL << (bitWidth - 1);

    // Sign extend M and Q into bitWidth
    int64_t M = multiplicand & mask;
    if (M & signBit) M |= (~mask); // sign extend

    int64_t Q = multiplier & mask;
    int64_t A = 0;
    int q_minus_1 = 0;

    for (int i = 0; i < bitWidth; ++i) {
        int q0 = Q & 1;

        if (q0 == 1 && q_minus_1 == 0) {
            A = (A - M) & mask;
        } else if (q0 == 0 && q_minus_1 == 1) {
            A = (A + M) & mask;
        }

        // Arithmetic right shift [A, Q, q_minus_1]
        q_minus_1 = Q & 1;
        int a_lsb = A & 1;
        bool a_sign = (A & signBit) != 0;

        Q = ((uint64_t)(Q & mask) >> 1) | ((uint64_t)a_lsb << (bitWidth - 1));
        A = ((uint64_t)(A & mask) >> 1);
        if (a_sign) {
            A |= signBit;
        }
    }

    // Combine [A, Q] into 2*bitWidth result
    int totalBits = 2 * bitWidth;
    uint64_t rawProduct = (((uint64_t)A & mask) << bitWidth) | ((uint64_t)Q & mask);
    int64_t finalProduct = 0;
    if (A & signBit) {
        // Negative in 2*bitWidth bits
        uint64_t fullMask = (totalBits >= 64) ? ~0ULL : ((1ULL << totalBits) - 1ULL);
        finalProduct = (int64_t)(rawProduct | (~fullMask));
    } else {
        finalProduct = (int64_t)rawProduct;
    }

    flags.zeroFlag = (finalProduct == 0);
    flags.signFlag = (finalProduct < 0);
    flags.carryFlag = false;
    flags.overflowFlag = false;

    return finalProduct;
}
