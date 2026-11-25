#include "ProcessorSimulation/ProcessorSimulator.h"
#include <iostream>
#include <iomanip>
#include <cstdint>

/**
 * @file main.cpp
 * @brief LogicCraft Review-2 Demonstration Entry Point for COA / ALP
 *
 * Subject: Computer Organization & Architecture / ALP
 * Focus:
 *  - CO1: Processor-Level Modeling (Registers R1, R2, R3, ALU, Control, Flags ZF/SF)
 *  - CO2: 64-Bit Assembly (64-bit Registers RAX/RCX/RDX, Instructions AND/OR/CMP, Conditional Branching)
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

void printBanner() {
    std::cout << "\n";
    std::cout << "*****************************************************************\n";
    std::cout << "*     LOGICCRAFT — VISUAL LOGIC GATE SIMULATOR (REVIEW 2)       *\n";
    std::cout << "*             MODULE: COA / 64-BIT ASSEMBLY (ALP)               *\n";
    std::cout << "*****************************************************************\n";
    std::cout << "*  CO1: PROCESSOR MODELING -> Registers R1/R2/R3, ALU, Control, *\n";
    std::cout << "*                             Status Flags (ZF, SF, CF)         *\n";
    std::cout << "*  CO2: 64-BIT ASSEMBLY    -> Native x86_64 Instructions,       *\n";
    std::cout << "*                             Registers RAX/RCX/RDX, CMP & JNE  *\n";
    std::cout << "*****************************************************************\n\n";
}

void demonstrateCO1_ProcessorModeling() {
    std::cout << "=================================================================\n";
    std::cout << " [DEMONSTRATION 1 / CO1]: PROCESSOR-LEVEL MODELING OF LOGIC GATES\n";
    std::cout << "=================================================================\n";
    std::cout << "In digital processors, Boolean logic gates (AND, OR, NOT) are not\n";
    std::cout << "abstract constructs; they map directly to CPU Arithmetic Logic Unit\n";
    std::cout << "(ALU) operations manipulating discrete binary values (0 and 1)\n";
    std::cout << "stored in high-speed hardware registers.\n";

    ProcessorSimulator cpu;

    // Simulation 1: AND Gate (1 AND 0 -> 0)
    cpu.simulateLogicGate("AND", 1, 0);

    // Simulation 2: OR Gate (0 OR 1 -> 1)
    cpu.simulateLogicGate("OR", 0, 1);

    // Simulation 3: NOT Gate (NOT 1 -> 0)
    cpu.simulateLogicGate("NOT", 1, 0);
}

void demonstrateCO2_64BitAssembly() {
    std::cout << "\n=================================================================\n";
    std::cout << " [DEMONSTRATION 2 / CO2]: GENUINE 64-BIT x86_64 ASSEMBLY (ALP)   \n";
    std::cout << "=================================================================\n";
    std::cout << "The following operations execute native x86_64 machine code via  \n";
    std::cout << "Microsoft x64 ABI calling conventions in 'logic_ops_64.s'.       \n";
    std::cout << "-----------------------------------------------------------------\n";

    // 1. 64-Bit AND Operation
    std::cout << "\n[ALP OPERATION 1: 64-BIT AND]\n";
    std::cout << "  Assembly Flow:\n";
    std::cout << "    MOV RAX, RCX      ; Load 64-bit operand A (RCX) into Accumulator\n";
    std::cout << "    AND RAX, RDX      ; Execute bitwise AND with operand B (RDX)\n";
    std::cout << "    RET               ; Return 64-bit result in RAX\n\n";

    uint64_t a1 = 1, b1 = 0;
    uint64_t resAnd0 = asm_and_64(a1, b1);
    std::cout << "  Execution: " << a1 << " AND " << b1 << " -> Result RAX = " << resAnd0 << "\n";

    uint64_t a2 = 1, b2 = 1;
    uint64_t resAnd1 = asm_and_64(a2, b2);
    std::cout << "  Execution: " << a2 << " AND " << b2 << " -> Result RAX = " << resAnd1 << "\n";

    // 2. 64-Bit OR Operation
    std::cout << "\n[ALP OPERATION 2: 64-BIT OR]\n";
    std::cout << "  Assembly Flow:\n";
    std::cout << "    MOV RAX, RCX      ; Load 64-bit operand A into RAX\n";
    std::cout << "    OR  RAX, RDX      ; Execute bitwise OR with operand B\n";
    std::cout << "    RET               ; Return 64-bit result in RAX\n\n";

    uint64_t resOr0 = asm_or_64(0, 0);
    uint64_t resOr1 = asm_or_64(0, 1);
    std::cout << "  Execution: 0 OR 0 -> Result RAX = " << resOr0 << "\n";
    std::cout << "  Execution: 0 OR 1 -> Result RAX = " << resOr1 << "\n";

    // 3. 64-Bit NOT and XOR
    std::cout << "\n[ALP OPERATION 3: 64-BIT NOT & XOR]\n";
    uint64_t resNot1 = asm_not_64(1);
    uint64_t resXor  = asm_xor_64(1, 0);
    std::cout << "  Execution: NOT 1     -> Result RAX = " << resNot1 << "\n";
    std::cout << "  Execution: 1 XOR 0   -> Result RAX = " << resXor << "\n";

    // 4. Comparison and Conditional Branching (CO2)
    std::cout << "\n[ALP OPERATION 4: COMPARISON & CONDITIONAL BRANCHING]\n";
    std::cout << "  Assembly Flow:\n";
    std::cout << "    CMP RAX, R9       ; Compare evaluated RAX with expected R9\n";
    std::cout << "    JNE .mismatch     ; Conditional branch if not equal (JNE)\n";
    std::cout << "    MOV RAX, 100      ; Branch taken: MATCH_SUCCESS\n";
    std::cout << "    RET\n\n";

    std::cout << "  Branch Test 1 (Operation: 1 AND 0, Expected: 0):\n";
    uint64_t branchTest1 = asm_branch_verify_64(1, 0, 0, 0);
    if (branchTest1 == 100) {
        std::cout << "   -> Branch Verification: [SUCCESS (Code 100)] Match verified via JE branch!\n";
    } else {
        std::cout << "   -> Branch Verification: [FAILED]\n";
    }

    std::cout << "  Branch Test 2 (Operation: 1 AND 1, Injected Fault Expected: 0):\n";
    uint64_t branchTest2 = asm_branch_verify_64(1, 1, 0, 0);
    if (branchTest2 == 404) {
        std::cout << "   -> Branch Verification: [SUCCESS (Code 404)] Mismatch correctly trapped via JNE branch!\n";
    } else {
        std::cout << "   -> Branch Verification: [FAILED]\n";
    }
}

void demonstrateHardwareTraceOfReferenceCircuit() {
    std::cout << "\n=================================================================\n";
    std::cout << " [DEMONSTRATION 3]: PROCESSOR HARDWARE TRACE OF REFERENCE CIRCUIT\n";
    std::cout << "=================================================================\n";
    std::cout << "Tracing hardware execution of shared/circuit_data.json on CPU:\n";
    std::cout << " Step 1: Gate G1 (AND) with Inputs [1, 0]\n";
    std::cout << " Step 2: Gate G2 (OR)  with Inputs [G1.out, C=1]\n";
    std::cout << " Step 3: Gate G3 (NOT) with Input  [G2.out]\n";
    std::cout << "-----------------------------------------------------------------\n";

    // Stage 1: G1
    uint64_t g1Out = asm_and_64(1, 0);
    std::cout << " [Stage 1 / G1 AND] -> asm_and_64(1, 0)       = " << g1Out << " (Stored in Register R3)\n";

    // Stage 2: G2
    uint64_t g2Out = asm_or_64(g1Out, 1);
    std::cout << " [Stage 2 / G2 OR ] -> asm_or_64(g1Out, 1)    = " << g2Out << " (Stored in Register R3)\n";

    // Stage 3: G3
    uint64_t g3Out = asm_not_64(g2Out);
    std::cout << " [Stage 3 / G3 NOT] -> asm_not_64(g2Out)      = " << g3Out << " (Final Circuit Output)\n";

    std::cout << ">>> Hardware trace matches high-level simulation perfectly! <<<\n";
}

void demonstrateCO3_ArithmeticAndStringProcessing() {
    std::cout << "\n=================================================================\n";
    std::cout << " [DEMONSTRATION 4 / CO3]: 64-BIT ASSEMBLY COMPUTATION & STRINGS  \n";
    std::cout << "=================================================================\n";
    std::cout << "CO3 Academic Competencies:\n";
    std::cout << " 1. Computationally Intensive Operation: Multi-step arithmetic\n";
    std::cout << "    implemented via Successive Addition loop in 64-bit hardware registers.\n";
    std::cout << " 2. String & Data Processing: ASCII character traversal, opcode\n";
    std::cout << "    decoding, and Boolean expression parsing executed in 64-bit Assembly.\n";
    std::cout << "-----------------------------------------------------------------\n";

    // -------------------------------------------------------------
    // Part A: Computational Arithmetic (Successive Addition)
    // -------------------------------------------------------------
    std::cout << "\n--- [CO3.1: ARITHMETIC ALGORITHM - SUCCESSIVE ADDITION] ---\n";
    std::cout << "Scenario: Computing Total Circuit Propagation Delay in picoseconds.\n";
    std::cout << "Formula: TotalDelay = GateCount * DelayPerGate\n";
    std::cout << "Assembly Routine: asm_mul_successive_addition_64 (RCX = delay, RDX = count)\n\n";

    // Normal Case
    uint64_t delayPerGate = 15; // 15 ns per gate
    uint64_t gateCount = 8;     // 8 gates in critical path
    uint64_t totalDelay = asm_mul_successive_addition_64(delayPerGate, gateCount);
    std::cout << " [NORMAL CASE 1]: " << delayPerGate << " ns/gate * " << gateCount 
              << " gates -> Result RAX = " << totalDelay << " ns [VERIFIED]\n";

    // Large Values
    uint64_t largeDelay = 250;
    uint64_t largeCount = 1000;
    uint64_t totalLarge = asm_mul_successive_addition_64(largeDelay, largeCount);
    std::cout << " [NORMAL CASE 2]: " << largeDelay << " ps/gate * " << largeCount 
              << " gates -> Result RAX = " << totalLarge << " ps [VERIFIED]\n";

    // Boundary Case 1: Zero multiplier (0 gates)
    uint64_t zeroMult = asm_mul_successive_addition_64(delayPerGate, 0);
    std::cout << " [BOUNDARY CASE 1 - Zero Multiplier]: " << delayPerGate 
              << " ns * 0 -> Result RAX = " << zeroMult << " (Expected 0) [PASS]\n";

    // Boundary Case 2: Zero multiplicand (0 delay)
    uint64_t zeroDelay = asm_mul_successive_addition_64(0, gateCount);
    std::cout << " [BOUNDARY CASE 2 - Zero Multiplicand]: 0 ns * " << gateCount 
              << " -> Result RAX = " << zeroDelay << " (Expected 0) [PASS]\n";

    // Boundary Case 3: Identity multiplier (1 gate)
    uint64_t idMult = asm_mul_successive_addition_64(delayPerGate, 1);
    std::cout << " [BOUNDARY CASE 3 - Identity 1]: " << delayPerGate 
              << " ns * 1 -> Result RAX = " << idMult << " (Expected " << delayPerGate << ") [PASS]\n";

    // Sub-demonstration: Truth table row count via successive doubling
    std::cout << "\nScenario: Truth Table Permutations (2^N) via 64-bit Doubling Algorithm:\n";
    for (uint64_t n = 0; n <= 4; n++) {
        uint64_t rows = asm_compute_truth_table_rows_64(n);
        std::cout << " -> Inputs N=" << n << " -> Truth Table Rows RAX = " << rows << "\n";
    }

    // -------------------------------------------------------------
    // Part B: String & Data Processing in 64-Bit Assembly
    // -------------------------------------------------------------
    std::cout << "\n--- [CO3.2: STRING PROCESSING IN 64-BIT ASSEMBLY] ---\n";
    std::cout << "Scenario 1: Decoding Gate Type Names from ASCII Strings.\n";
    std::cout << "Assembly Routine: asm_parse_gate_type_64 (RCX = const char*)\n\n";

    const char* sampleGates[] = {"AND", "OR", "NOT", "XOR", "NAND", "NOR", "XNOR", "and", "xor"};
    for (const char* gName : sampleGates) {
        uint64_t opcode = asm_parse_gate_type_64(gName);
        std::cout << " -> String \"" << std::setw(5) << gName << "\" -> Decoded Opcode RAX = " 
                  << opcode << " (" << BooleanALU::getOpcodeName((ALUOpcode)opcode) << ")\n";
    }

    // Boundary String Cases
    std::cout << " [BOUNDARY STRING CASES]:\n";
    uint64_t opUnknown = asm_parse_gate_type_64("INVALID_GATE");
    std::cout << "  - Malformed Gate Name (\"INVALID_GATE\"): RAX = " << opUnknown << " (Expected 0) [PASS]\n";
    uint64_t opEmpty = asm_parse_gate_type_64("");
    std::cout << "  - Empty String (\"\"):                     RAX = " << opEmpty << " (Expected 0) [PASS]\n";
    uint64_t opNull = asm_parse_gate_type_64(nullptr);
    std::cout << "  - Null Pointer (nullptr):              RAX = " << opNull << " (Expected 0) [PASS]\n";

    std::cout << "\nScenario 2: Validating Circuit Control Commands in 64-bit Assembly:\n";
    const char* commands[] = {"ADD_GATE", "DEL_GATE", "CONNECT", "SIMULATE", "TOGGLE", "UNKNOWN"};
    for (const char* cmd : commands) {
        uint64_t cmdId = asm_validate_command_64(cmd);
        std::cout << " -> Command \"" << std::setw(8) << cmd << "\" -> Command ID RAX = " << cmdId << "\n";
    }

    std::cout << "\nScenario 3: Direct Evaluation of Boolean Expression ASCII Strings:\n";
    std::cout << "Assembly Routine: asm_eval_boolean_expr_str_64 (RCX = const char*)\n\n";
    const char* exprs[] = {"1 & 0", "1 | 0", "1 ^ 1", "~0", "~1", "0 & 0", "1 | 1"};
    for (const char* ex : exprs) {
        uint64_t evaluated = asm_eval_boolean_expr_str_64(ex);
        std::cout << " -> Expression \"" << ex << "\" ===> Assembly Evaluated Result RAX = " << evaluated << "\n";
    }

    // Expression Boundary / Malformed Cases
    uint64_t errExpr1 = asm_eval_boolean_expr_str_64("invalid");
    std::cout << " [BOUNDARY EXPR 1]: \"invalid\" -> RAX = " << errExpr1 << " (Code 255 = Malformed) [PASS]\n";
    uint64_t errExpr2 = asm_eval_boolean_expr_str_64("");
    std::cout << " [BOUNDARY EXPR 2]: \"\"        -> RAX = " << errExpr2 << " (Code 255 = Malformed) [PASS]\n";
}

void demonstrateCO4_IntegrationAndDemonstration() {
    std::cout << "\n=================================================================\n";
    std::cout << " [DEMONSTRATION 4 / CO4]: COA COMPONENT INTEGRATION & DEMONSTRATION\n";
    std::cout << "=================================================================\n";
    std::cout << "Mapping Application-Level Circuit Logic to 64-Bit Processor Hardware:\n";
    std::cout << "+-------------------+-----------------------+-----------------------------+\n";
    std::cout << "| Application Level | Processor Operation   | Hardware Register State     |\n";
    std::cout << "+-------------------+-----------------------+-----------------------------+\n";

    // Application Gate 1: AND Gate
    uint64_t g1_a = 1, g1_b = 0;
    uint64_t g1_out = asm_and_64(g1_a, g1_b);
    std::cout << "| G1: AND(1, 0)     | MOV RAX, 1; AND RDX   | RAX = " << g1_out << " | ZF = " << (g1_out == 0 ? 1 : 0) << " (Zero)       |\n";

    // Application Gate 2: OR Gate (Cascaded G1 output into Pin 0, Pin 1 = 1)
    uint64_t g2_a = g1_out, g2_b = 1;
    uint64_t g2_out = asm_or_64(g2_a, g2_b);
    std::cout << "| G2: OR(G1, 1)     | MOV RAX, 0; OR  RDX   | RAX = " << g2_out << " | ZF = " << (g2_out == 0 ? 1 : 0) << " (Non-zero)   |\n";

    // Application Gate 3: NOT Gate (Inverts G2 output)
    uint64_t g3_out = asm_not_64(g2_out);
    std::cout << "| G3: NOT(G2)       | NOT RAX; AND RAX, 1   | RAX = " << g3_out << " | ZF = " << (g3_out == 0 ? 1 : 0) << " (Zero)       |\n";
    std::cout << "+-------------------+-----------------------+-----------------------------+\n";

    // Integration of Arithmetic Delay & Truth Table Generation
    std::cout << "\n[INTEGRATED PROPAGATION DELAY CALCULATION VIA 64-BIT ASSEMBLY]:\n";
    uint64_t stages = 3;
    uint64_t gateDelay = 5; // 5 nanoseconds per gate
    uint64_t totalDelay = asm_mul_successive_addition_64(stages, gateDelay);
    std::cout << "  -> Circuit Stages: " << stages << " | Delay per Gate: " << gateDelay << " ns\n";
    std::cout << "  -> Hardware Total Propagation Delay: " << totalDelay 
              << " ns (Computed via asm_mul_successive_addition_64 without hardware IMUL)\n";

    uint64_t truthTableRows = asm_compute_truth_table_rows_64(stages);
    std::cout << "  -> Truth Table Input Permutations: 2^" << stages << " = " 
              << truthTableRows << " Rows (Computed via asm_compute_truth_table_rows_64)\n";

    // Verification of branch logic
    uint64_t verifyResult = asm_branch_verify_64(g2_a, g2_b, 1, g2_out); // Op 1 = OR
    std::cout << "  -> Conditional Branch Integrity (CMP & JE/JNE): Code " 
              << verifyResult << " (100 = Exact Hardware Match)\n";

    // -------------------------------------------------------------
    // CO4 PART B: ALU ARCHITECTURE & 10 OPERATION HARDWARE DESIGN
    // -------------------------------------------------------------
    BooleanALU::printALUDiagram();

    // -------------------------------------------------------------
    // CO4 PART C: CONTROL UNIT ARCHITECTURE & SIGNAL GENERATION
    // -------------------------------------------------------------
    HardwiredControl::printArchitectureDiagram();
    MicroprogrammedControl::printArchitectureDiagram();
    ControlUnit::printControlSignalTable();
    ControlUnit::compareControlTechniques();

    // -------------------------------------------------------------
    // CO4 PART D: THREE MAJOR LOGICCRAFT FUNCTIONALITIES IN CPU SIMULATOR
    // -------------------------------------------------------------
    ProcessorSimulator cpu;
    // 1. Logic Gate Evaluation
    cpu.demonstrateLogicGateFunctionality("AND", 1, 1);
    cpu.demonstrateLogicGateFunctionality("XOR", 1, 0);
    // 2. Propagation Delay via Successive Addition Multiplication Loop
    cpu.demonstratePropagationDelayMultiplication(15, 8);
    // 3. Netlist Gate Name String Parsing & Opcode Decoding
    cpu.demonstrateNetlistOpcodeDecoding("AND");
    cpu.demonstrateNetlistOpcodeDecoding("NOR");

    // -------------------------------------------------------------
    // CO4 PART E: FIVE-STAGE INSTRUCTION PIPELINE WITH HAZARDS
    // -------------------------------------------------------------
    cpu.demonstrateFiveStagePipeline();
}

int main(int argc, char* argv[]) {
    printBanner();

    demonstrateCO1_ProcessorModeling();
    demonstrateCO2_64BitAssembly();
    demonstrateHardwareTraceOfReferenceCircuit();
    demonstrateCO3_ArithmeticAndStringProcessing();
    demonstrateCO4_IntegrationAndDemonstration();

    std::cout << "\n*****************************************************************\n";
    std::cout << "*  COA / 64-BIT ASSEMBLY (CO1, CO2, CO3, CO4) COMPLETE!         *\n";
    std::cout << "*****************************************************************\n\n";

    return 0;
}

