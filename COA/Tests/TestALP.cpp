#include "../ProcessorSimulation/ProcessorSimulator.h"
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

int main() {
    std::cout << "======================================================\n";
    std::cout << "        COA / ALP UNIT TEST VERIFICATION SUITE        \n";
    std::cout << "======================================================\n";
    testProcessorSimulationCO1();
    test64BitAssemblyCO2();
    std::cout << "======================================================\n";
    std::cout << " ALL COA REVIEW-2 TESTS PASSED SUCCESSFULLY! (CO1 & CO2)\n";
    std::cout << "======================================================\n";
    return 0;
}
