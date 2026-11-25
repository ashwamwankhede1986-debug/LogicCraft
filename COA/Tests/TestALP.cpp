#include "../ProcessorSimulation/ProcessorSimulator.h"
#include "Task4IntegrationTests.h"
#include <iostream>
#include <cstdint>
#include <cassert>

/**
 * @file TestALP.cpp
 * @brief Unit tests for COA Module (CO1 Processor Simulation & CO2 64-bit Assembly)
 */

extern "C" {
    uint64_t asm_and_64(uint64_t a, uint64_t b);
    uint64_t asm_or_64(uint64_t a, uint64_t b);
    uint64_t asm_not_64(uint64_t a);
    uint64_t asm_xor_64(uint64_t a, uint64_t b);
    uint64_t asm_branch_verify_64(uint64_t a, uint64_t b, uint64_t opType, uint64_t expected);
    uint64_t asm_mul_successive_addition_64(uint64_t multiplicand, uint64_t multiplier);
    uint64_t asm_compute_truth_table_rows_64(uint64_t numInputs);
    uint64_t asm_parse_gate_type_64(const char* gateName);
    uint64_t asm_validate_command_64(const char* cmdString);
    uint64_t asm_eval_boolean_expr_str_64(const char* exprStr);
}

void testProcessorSimulationCO1() {
    std::cout << "[TEST] Running Processor Simulation (CO1) Test ...\n";
    ProcessorSimulator sim;

    // Test AND: 1 & 0 = 0
    sim.loadR1(1);
    sim.loadR2(0);
    sim.executeALU(OP_AND);
    assert(sim.getR3() == 0);
    assert(sim.getZeroFlag() == true);

    // Test OR: 0 | 1 = 1
    sim.loadR1(0);
    sim.loadR2(1);
    sim.executeALU(OP_OR);
    assert(sim.getR3() == 1);
    assert(sim.getZeroFlag() == false);

    std::cout << " -> Processor Simulation (CO1) PASS!\n";
}

void test64BitAssemblyCO2() {
    std::cout << "[TEST] Running 64-bit Assembly (CO2) Test ...\n";

    // 1. 64-bit AND
    assert(asm_and_64(1, 1) == 1);
    assert(asm_and_64(1, 0) == 0);
    assert(asm_and_64(0, 1) == 0);
    assert(asm_and_64(0, 0) == 0);
    assert(asm_and_64(0xFFFFFFFFFFFFFFFFULL, 0x0000000000000001ULL) == 1);

    // 2. 64-bit OR
    assert(asm_or_64(0, 0) == 0);
    assert(asm_or_64(1, 0) == 1);
    assert(asm_or_64(0, 1) == 1);
    assert(asm_or_64(1, 1) == 1);

    // 3. 64-bit NOT
    assert(asm_not_64(0) == 1);
    assert(asm_not_64(1) == 0);

    // 4. 64-bit XOR
    assert(asm_xor_64(1, 0) == 1);
    assert(asm_xor_64(1, 1) == 0);

    // 5. 64-bit Comparison and Conditional Branching (JE / JNE)
    uint64_t codeMatch = asm_branch_verify_64(1, 0, 0, 0); // 1 AND 0 == 0 -> Match!
    assert(codeMatch == 100);

    uint64_t codeMismatch = asm_branch_verify_64(1, 1, 0, 0); // 1 AND 1 == 1, expected 0 -> Mismatch!
    assert(codeMismatch == 404);

    std::cout << " -> 64-Bit Assembly (CO2) PASS!\n";
}

void testArithmeticSuccessiveAdditionCO3() {
    std::cout << "[TEST] Running Successive Addition Arithmetic (CO3) Test ...\n";

    // 1. Normal Multiplications
    assert(asm_mul_successive_addition_64(15, 8) == 120);
    assert(asm_mul_successive_addition_64(25, 4) == 100);
    assert(asm_mul_successive_addition_64(1000, 250) == 250000);

    // 2. Boundary Cases
    assert(asm_mul_successive_addition_64(15, 0) == 0);  // Zero multiplier
    assert(asm_mul_successive_addition_64(0, 8) == 0);   // Zero multiplicand
    assert(asm_mul_successive_addition_64(0, 0) == 0);   // Both zero
    assert(asm_mul_successive_addition_64(42, 1) == 42); // Identity 1

    // 3. Truth Table Doubling (2^N)
    assert(asm_compute_truth_table_rows_64(0) == 1);
    assert(asm_compute_truth_table_rows_64(1) == 2);
    assert(asm_compute_truth_table_rows_64(2) == 4);
    assert(asm_compute_truth_table_rows_64(3) == 8);
    assert(asm_compute_truth_table_rows_64(4) == 16);
    assert(asm_compute_truth_table_rows_64(8) == 256);

    std::cout << " -> Successive Addition Arithmetic (CO3) PASS!\n";
}

void testStringProcessingCO3() {
    std::cout << "[TEST] Running 64-bit Assembly String Processing (CO3) Test ...\n";

    // 1. Gate Type String Parsing
    assert(asm_parse_gate_type_64("AND") == 1);
    assert(asm_parse_gate_type_64("OR") == 2);
    assert(asm_parse_gate_type_64("NOT") == 3);
    assert(asm_parse_gate_type_64("XOR") == 4);
    assert(asm_parse_gate_type_64("NAND") == 5);
    assert(asm_parse_gate_type_64("NOR") == 6);
    assert(asm_parse_gate_type_64("XNOR") == 7);
    assert(asm_parse_gate_type_64("and") == 1);
    assert(asm_parse_gate_type_64("xor") == 4);

    // Boundary String Cases
    assert(asm_parse_gate_type_64("") == 0);
    assert(asm_parse_gate_type_64(nullptr) == 0);
    assert(asm_parse_gate_type_64("INVALID") == 0);

    // 2. Circuit Command Validation
    assert(asm_validate_command_64("ADD_GATE") == 1);
    assert(asm_validate_command_64("DEL_GATE") == 2);
    assert(asm_validate_command_64("CONNECT") == 3);
    assert(asm_validate_command_64("SIMULATE") == 4);
    assert(asm_validate_command_64("TOGGLE") == 5);
    assert(asm_validate_command_64("UNKNOWN") == 0);
    assert(asm_validate_command_64("") == 0);
    assert(asm_validate_command_64(nullptr) == 0);

    // 3. Boolean Expression String Evaluation
    assert(asm_eval_boolean_expr_str_64("1 & 0") == 0);
    assert(asm_eval_boolean_expr_str_64("1 & 1") == 1);
    assert(asm_eval_boolean_expr_str_64("0 | 0") == 0);
    assert(asm_eval_boolean_expr_str_64("1 | 0") == 1);
    assert(asm_eval_boolean_expr_str_64("1 ^ 1") == 0);
    assert(asm_eval_boolean_expr_str_64("1 ^ 0") == 1);
    assert(asm_eval_boolean_expr_str_64("~0") == 1);
    assert(asm_eval_boolean_expr_str_64("~1") == 0);

    // Boundary Expression Cases
    assert(asm_eval_boolean_expr_str_64("bad") == 255);
    assert(asm_eval_boolean_expr_str_64("") == 255);
    assert(asm_eval_boolean_expr_str_64(nullptr) == 255);

    std::cout << " -> 64-bit Assembly String Processing (CO3) PASS!\n";
}

int main() {
    std::cout << "======================================================\n";
    std::cout << "        COA / ALP UNIT TEST VERIFICATION SUITE        \n";
    std::cout << "======================================================\n";
    testProcessorSimulationCO1();
    test64BitAssemblyCO2();
    testArithmeticSuccessiveAdditionCO3();
    testStringProcessingCO3();
    runCOATask4IntegrationSuite();
    std::cout << "======================================================\n";
    std::cout << " ALL COA TESTS PASSED SUCCESSFULLY! (CO1, CO2, CO3, CO4)\n";
    std::cout << "======================================================\n";
    return 0;
}
