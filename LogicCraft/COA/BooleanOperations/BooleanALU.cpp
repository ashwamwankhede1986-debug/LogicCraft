#include "BooleanALU.h"
#include <cstring>

uint64_t BooleanALU::execute(ALUOpcode opcode, uint64_t operandA, uint64_t operandB, ALUFlags& flags) {
    uint64_t result = 0;

    switch (opcode) {
        case OP_AND:
            result = operandA & operandB;
            break;
        case OP_OR:
            result = operandA | operandB;
            break;
        case OP_NOT:
            result = (~operandA) & 1ULL; // Invert bit 0
            break;
        case OP_XOR:
            result = operandA ^ operandB;
            break;
        case OP_NAND:
            result = (~(operandA & operandB)) & 1ULL;
            break;
        case OP_NOR:
            result = (~(operandA | operandB)) & 1ULL;
            break;
        default:
            result = 0;
            break;
    }

    // Update Processor Status Flags
    flags.zeroFlag = (result == 0);
    flags.signFlag = ((result >> 63) & 1) != 0;
    flags.carryFlag = false;

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
        default:      return "UNKNOWN";
    }
}

ALUOpcode BooleanALU::stringToOpcode(const char* name) {
    if (strcmp(name, "AND") == 0) return OP_AND;
    if (strcmp(name, "OR") == 0) return OP_OR;
    if (strcmp(name, "NOT") == 0) return OP_NOT;
    if (strcmp(name, "XOR") == 0) return OP_XOR;
    if (strcmp(name, "NAND") == 0) return OP_NAND;
    if (strcmp(name, "NOR") == 0) return OP_NOR;
    return OP_AND;
}
