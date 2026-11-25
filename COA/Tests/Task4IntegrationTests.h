#ifndef TASK4_INTEGRATION_TESTS_H
#define TASK4_INTEGRATION_TESTS_H

#include <cstdint>

/**
 * @file Task4IntegrationTests.h
 * @brief LogicCraft COA Task 4 Integration Test Suite (CO4)
 *
 * Demonstrates the bidirectional mapping of Application-Level Circuit Operations
 * to Processor-Level Architecture:
 * - Complete 10-Operation ALU with Flags (ZF, SF, CF, OF)
 * - Control Unit (Hardwired & Microprogrammed models, Control Signal Table)
 * - Five-Stage Instruction Pipeline (IF, ID, EX, MEM, WB, Forwarding & Load-Use Stalls)
 * - 3 Core LogicCraft Functionalities executed at CPU Simulator & 64-bit Assembly levels
 */

void testALUComplete10OperationsAndFlags();
void testControlUnitSignalsAndTechniques();
void testFiveStagePipelineAndHazards();
void testProcessorSimulatorThreeCoreFunctionalities();
void runCOATask4IntegrationSuite();

#endif // TASK4_INTEGRATION_TESTS_H
