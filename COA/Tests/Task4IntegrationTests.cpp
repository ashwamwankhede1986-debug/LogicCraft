#include "Task4IntegrationTests.h"
#include "../ProcessorSimulation/ProcessorSimulator.h"
#include "../BooleanOperations/BooleanALU.h"
#include "../ControlUnit/ControlUnit.h"
#include "../Pipeline/PipelineSimulator.h"

#include <iostream>
#include <iomanip>
#include <cassert>

extern "C" {
    uint64_t asm_and_64(uint64_t a, uint64_t b);
    uint64_t asm_or_64(uint64_t a, uint64_t b);
    uint64_t asm_not_64(uint64_t a);
    uint64_t asm_xor_64(uint64_t a, uint64_t b);
    uint64_t asm_nand_64(uint64_t a, uint64_t b);
    uint64_t asm_nor_64(uint64_t a, uint64_t b);
    uint64_t asm_xnor_64(uint64_t a, uint64_t b);
    uint64_t asm_add_64(uint64_t a, uint64_t b);
    uint64_t asm_sub_64(uint64_t a, uint64_t b);
    int64_t  asm_cmp_64(uint64_t a, uint64_t b);
    uint64_t asm_branch_verify_64(uint64_t a, uint64_t b, uint64_t opType, uint64_t expected);
    uint64_t asm_mul_successive_addition_64(uint64_t multiplicand, uint64_t multiplier);
    uint64_t asm_compute_truth_table_rows_64(uint64_t numInputs);
    uint64_t asm_parse_gate_type_64(const char* gateName);
    uint64_t asm_validate_command_64(const char* cmdString);
    uint64_t asm_eval_boolean_expr_str_64(const char* exprStr);
}

// Struct modeling application-to-processor operation mapping
struct CircuitStageMapping {
    const char* gateName;
    const char* gateType;
    uint64_t inA;
    uint64_t inB;
    uint64_t expectedOutput;
    const char* hardwareInstruction;
    const char* registerMapping;
};

void testApplicationToProcessorMapping() {
    std::cout << "[COA TEST 1/8] Testing Application-to-Processor Mapping (CO4 Integration) ...\n";

    CircuitStageMapping stages[3] = {
        {"G1_AND", "AND", 1, 1, 1, "AND RAX, RDX", "RCX=1, RDX=1 -> RAX=1 (ZF=0)"},
        {"G2_OR",  "OR",  1, 0, 1, "OR  RAX, RDX", "RCX=1, RDX=0 -> RAX=1 (ZF=0)"},
        {"G3_NOT", "NOT", 1, 0, 0, "NOT RAX; AND RAX, 1", "RCX=1 -> RAX=0 (ZF=1)"}
    };

    uint64_t currentSignal = 0;
    for (int i = 0; i < 3; ++i) {
        uint64_t inA = (i == 0) ? stages[i].inA : currentSignal;
        uint64_t inB = stages[i].inB;
        uint64_t result = 0;

        uint64_t opcode = asm_parse_gate_type_64(stages[i].gateType);
        assert(opcode > 0);

        if (opcode == 1) { // AND
            result = asm_and_64(inA, inB);
        } else if (opcode == 2) { // OR
            result = asm_or_64(inA, inB);
        } else if (opcode == 3) { // NOT
            result = asm_not_64(inA);
        }

        assert(result == stages[i].expectedOutput);
        currentSignal = result;

        // Verify with branch verification conditional jump for 2-input gates
        if (opcode <= 2) {
            uint64_t branchCode = asm_branch_verify_64(inA, inB, opcode - 1, stages[i].expectedOutput);
            assert(branchCode == 100); // 100 = verified match
        }
    }

    std::cout << " -> Application-to-Processor Mapping PASS!\n";
}

void testAssemblyArithmeticDelayAndTableIntegration() {
    std::cout << "[COA TEST 2/8] Testing Assembly Arithmetic Propagation Delay & Truth Table (CO4) ...\n";

    uint64_t stages = 3;
    uint64_t delayPerGate = 5;
    uint64_t totalDelay = asm_mul_successive_addition_64(stages, delayPerGate);
    assert(totalDelay == 15);

    uint64_t rows = asm_compute_truth_table_rows_64(stages);
    assert(rows == 8);

    assert(asm_mul_successive_addition_64(0, 5) == 0);
    assert(asm_mul_successive_addition_64(5, 0) == 0);
    assert(asm_compute_truth_table_rows_64(0) == 1);

    std::cout << " -> Assembly Arithmetic & Truth Table PASS!\n";
}

void testAssemblyStringAndCommandProcessing() {
    std::cout << "[COA TEST 3/8] Testing Assembly String Parsing & Boolean Evaluation (CO4) ...\n";

    assert(asm_parse_gate_type_64("AND") == 1);
    assert(asm_parse_gate_type_64("OR") == 2);
    assert(asm_parse_gate_type_64("NOT") == 3);
    assert(asm_parse_gate_type_64("XOR") == 4);
    assert(asm_parse_gate_type_64("NAND") == 5);
    assert(asm_parse_gate_type_64("NOR") == 6);
    assert(asm_parse_gate_type_64("XNOR") == 7);

    assert(asm_validate_command_64("ADD_GATE") == 1);
    assert(asm_validate_command_64("DEL_GATE") == 2);
    assert(asm_validate_command_64("CONNECT") == 3);
    assert(asm_validate_command_64("SIMULATE") == 4);

    assert(asm_eval_boolean_expr_str_64("1 & 1") == 1);
    assert(asm_eval_boolean_expr_str_64("1 & 0") == 0);
    assert(asm_eval_boolean_expr_str_64("0 | 1") == 1);
    assert(asm_eval_boolean_expr_str_64("1 ^ 0") == 1);
    assert(asm_eval_boolean_expr_str_64("~0") == 1);
    assert(asm_eval_boolean_expr_str_64("~1") == 0);

    std::cout << " -> Assembly String & Command Processing PASS!\n";
}

void testAssemblyErrorHandlingAndBoundaries() {
    std::cout << "[COA TEST 4/8] Testing Assembly Boundary Protection & Error Handling (CO4) ...\n";

    assert(asm_parse_gate_type_64("INVALID_OPCODE") == 0);
    assert(asm_parse_gate_type_64("") == 0);
    assert(asm_parse_gate_type_64(nullptr) == 0);

    assert(asm_validate_command_64("RANDOM_CMD") == 0);
    assert(asm_validate_command_64("") == 0);
    assert(asm_validate_command_64(nullptr) == 0);

    assert(asm_eval_boolean_expr_str_64("gibberish") == 255);
    assert(asm_eval_boolean_expr_str_64("") == 255);
    assert(asm_eval_boolean_expr_str_64(nullptr) == 255);

    uint64_t mismatchCode = asm_branch_verify_64(1, 1, 0, 0);
    assert(mismatchCode == 404);

    std::cout << " -> Assembly Boundary Protection & Error Handling PASS!\n";
}

// =============================================================================
// CO4 EXTENSION TESTS: ALU 10 OPERATIONS & FLAGS
// =============================================================================
void testALUComplete10OperationsAndFlags() {
    std::cout << "[COA TEST 5/8] Testing ALU 10 Operations & Condition Flags (ZF, SF, CF, OF) ...\n";
    ALUFlags flags;

    // 1. OP_AND
    assert(BooleanALU::execute(OP_AND, 1, 1, flags) == 1);
    assert(!flags.zeroFlag);
    assert(BooleanALU::execute(OP_AND, 1, 0, flags) == 0);
    assert(flags.zeroFlag);

    // 2. OP_OR
    assert(BooleanALU::execute(OP_OR, 0, 0, flags) == 0);
    assert(flags.zeroFlag);
    assert(BooleanALU::execute(OP_OR, 1, 0, flags) == 1);
    assert(!flags.zeroFlag);

    // 3. OP_NOT
    assert(BooleanALU::execute(OP_NOT, 0, 0, flags) == 1);
    assert(!flags.zeroFlag);
    assert(BooleanALU::execute(OP_NOT, 1, 0, flags) == 0);
    assert(flags.zeroFlag);

    // 4. OP_XOR
    assert(BooleanALU::execute(OP_XOR, 1, 1, flags) == 0);
    assert(flags.zeroFlag);
    assert(BooleanALU::execute(OP_XOR, 1, 0, flags) == 1);
    assert(!flags.zeroFlag);

    // 5. OP_NAND
    assert(BooleanALU::execute(OP_NAND, 1, 1, flags) == 0);
    assert(flags.zeroFlag);
    assert(BooleanALU::execute(OP_NAND, 1, 0, flags) == 1);
    assert(!flags.zeroFlag);

    // 6. OP_NOR
    assert(BooleanALU::execute(OP_NOR, 0, 0, flags) == 1);
    assert(!flags.zeroFlag);
    assert(BooleanALU::execute(OP_NOR, 1, 0, flags) == 0);
    assert(flags.zeroFlag);

    // 7. OP_XNOR
    assert(BooleanALU::execute(OP_XNOR, 1, 1, flags) == 1);
    assert(!flags.zeroFlag);
    assert(BooleanALU::execute(OP_XNOR, 1, 0, flags) == 0);
    assert(flags.zeroFlag);

    // 8. OP_ADD
    assert(BooleanALU::execute(OP_ADD, 15, 25, flags) == 40);
    assert(!flags.zeroFlag && !flags.signFlag && !flags.carryFlag && !flags.overflowFlag);
    // Carry Flag Test (Unsigned overflow: 0xFFFFFFFFFFFFFFFF + 1 = 0, CF=1, ZF=1)
    assert(BooleanALU::execute(OP_ADD, 0xFFFFFFFFFFFFFFFFULL, 1, flags) == 0);
    assert(flags.carryFlag && flags.zeroFlag);

    // 9. OP_SUB
    assert(BooleanALU::execute(OP_SUB, 50, 20, flags) == 30);
    assert(!flags.zeroFlag && !flags.signFlag);
    assert(BooleanALU::execute(OP_SUB, 20, 20, flags) == 0);
    assert(flags.zeroFlag);
    // Negative Result: SF=1
    BooleanALU::execute(OP_SUB, 20, 50, flags);
    assert(flags.signFlag);

    // 10. OP_CMP
    assert(BooleanALU::execute(OP_CMP, 25, 25, flags) == 0);
    assert(flags.zeroFlag);
    assert(BooleanALU::execute(OP_CMP, 50, 25, flags) == 1);
    assert(!flags.zeroFlag && !flags.signFlag);
    assert((int64_t)BooleanALU::execute(OP_CMP, 10, 25, flags) == -1);
    assert(flags.signFlag);

    // Verify Native 64-bit Assembly counterparts
    assert(asm_nand_64(1, 1) == 0);
    assert(asm_nand_64(1, 0) == 1);
    assert(asm_nor_64(0, 0) == 1);
    assert(asm_nor_64(0, 1) == 0);
    assert(asm_xnor_64(1, 1) == 1);
    assert(asm_xnor_64(1, 0) == 0);
    assert(asm_add_64(15, 25) == 40);
    assert(asm_sub_64(50, 20) == 30);
    assert(asm_cmp_64(20, 20) == 0);
    assert(asm_cmp_64(30, 20) == 1);
    assert(asm_cmp_64(10, 20) == -1);

    std::cout << " -> ALU 10 Operations & Condition Flags PASS!\n";
}

// =============================================================================
// CO4 EXTENSION TESTS: CONTROL UNIT SIGNALS & HARDWIRED/MICROPROGRAMMED
// =============================================================================
void testControlUnitSignalsAndTechniques() {
    std::cout << "[COA TEST 6/8] Testing Control Unit Signals & Control Models ...\n";
    ControlUnit cu;

    // Test R-Type Instruction: AND R3, R1, R2
    InstructionWord rInstr = ControlUnit::createRType("AND", 3, 1, 2);
    ControlSignals rSig = cu.decode(rInstr);
    assert(rSig.RegWrite == true);
    assert(rSig.ALUSrc == false);
    assert(rSig.MemRead == false);
    assert(rSig.MemWrite == false);
    assert(rSig.Branch == false);
    assert(rSig.MemtoReg == false);
    assert(rSig.RegDst == true);
    assert(rSig.ALUOp == 0x02);
    assert(rSig.aluControl == OP_AND);

    // Test LW Instruction
    InstructionWord lwInstr = ControlUnit::createIType(OPC_LW, 2, 1, 8, "LW R2, 8(R1)");
    ControlSignals lwSig = cu.decode(lwInstr);
    assert(lwSig.RegWrite == true);
    assert(lwSig.ALUSrc == true);
    assert(lwSig.MemRead == true);
    assert(lwSig.MemWrite == false);
    assert(lwSig.Branch == false);
    assert(lwSig.MemtoReg == true);
    assert(lwSig.RegDst == false);
    assert(lwSig.aluControl == OP_ADD);

    // Test SW Instruction
    InstructionWord swInstr = ControlUnit::createIType(OPC_SW, 2, 1, 8, "SW R2, 8(R1)");
    ControlSignals swSig = cu.decode(swInstr);
    assert(swSig.RegWrite == false);
    assert(swSig.ALUSrc == true);
    assert(swSig.MemRead == false);
    assert(swSig.MemWrite == true);
    assert(swSig.Branch == false);
    assert(swSig.RegDst == false);

    // Test BEQ Instruction
    InstructionWord beqInstr = ControlUnit::createBranch(OPC_BEQ, 1, 2, 4, "BEQ R1, R2, 4");
    ControlSignals beqSig = cu.decode(beqInstr);
    assert(beqSig.RegWrite == false);
    assert(beqSig.ALUSrc == false);
    assert(beqSig.MemRead == false);
    assert(beqSig.MemWrite == false);
    assert(beqSig.Branch == true);
    assert(beqSig.aluControl == OP_SUB);

    // Verify Microprogrammed Control functional equivalence with Hardwired Control
    MicroprogrammedControl uProg;
    ControlSignals uSig = uProg.executeMicrocode(rInstr);
    assert(uSig.RegWrite == rSig.RegWrite);
    assert(uSig.ALUSrc == rSig.ALUSrc);
    assert(uSig.MemRead == rSig.MemRead);
    assert(uSig.MemWrite == rSig.MemWrite);
    assert(uSig.aluControl == rSig.aluControl);

    std::cout << " -> Control Unit Signals & Control Models PASS!\n";
}

// =============================================================================
// CO4 EXTENSION TESTS: FIVE-STAGE PIPELINE & HAZARD RESOLUTION
// =============================================================================
void testFiveStagePipelineAndHazards() {
    std::cout << "[COA TEST 7/8] Testing Five-Stage Pipeline, RAW Forwarding & Load-Use Stalls ...\n";
    PipelineSimulator pipe;

    // Test Case 1: RAW Data Hazard resolved via Forwarding
    std::vector<InstructionWord> prog1;
    InstructionWord i0; i0.opcode = OPC_ADDI; i0.rs = 0; i0.rt = 1; i0.imm = 10; i0.mnemonic = "ADDI R1, R0, 10";
    InstructionWord i1; i1.opcode = OPC_R_TYPE; i1.rs = 1; i1.rt = 1; i1.rd = 2; i1.funct = OP_ADD; i1.mnemonic = "ADD R2, R1, R1";
    InstructionWord i2; i2.opcode = OPC_R_TYPE; i2.rs = 2; i2.rt = 1; i2.rd = 3; i2.funct = OP_ADD; i2.mnemonic = "ADD R3, R2, R1";
    prog1.push_back(i0);
    prog1.push_back(i1);
    prog1.push_back(i2);

    pipe.loadProgram(prog1);
    pipe.run(20);

    assert(pipe.getRegister(1) == 10);
    assert(pipe.getRegister(2) == 20);
    assert(pipe.getRegister(3) == 30);
    assert(pipe.getTotalForwardEvents() >= 2);

    // Test Case 2: Load-Use Hazard requiring 1-cycle Stall Bubble
    PipelineSimulator pipe2;
    std::vector<InstructionWord> prog2;
    InstructionWord j0; j0.opcode = OPC_ADDI; j0.rs = 0; j0.rt = 1; j0.imm = 42; j0.mnemonic = "ADDI R1, R0, 42";
    InstructionWord j1; j1.opcode = OPC_SW;   j1.rs = 0; j1.rt = 1; j1.imm = 0;  j1.mnemonic = "SW R1, 0(R0)";
    InstructionWord j2; j2.opcode = OPC_LW;   j2.rs = 0; j2.rt = 2; j2.imm = 0;  j2.mnemonic = "LW R2, 0(R0)";
    InstructionWord j3; j3.opcode = OPC_R_TYPE; j3.rs = 2; j3.rt = 1; j3.rd = 3; j3.funct = OP_ADD; j3.mnemonic = "ADD R3, R2, R1";
    prog2.push_back(j0);
    prog2.push_back(j1);
    prog2.push_back(j2);
    prog2.push_back(j3);

    pipe2.loadProgram(prog2);
    pipe2.run(25);

    assert(pipe2.getRegister(1) == 42);
    assert(pipe2.getRegister(2) == 42);
    assert(pipe2.getRegister(3) == 84);
    assert(pipe2.getTotalStalls() >= 1); // Confirms hardware bubble injection

    std::cout << " -> Five-Stage Pipeline & Hazard Resolution PASS!\n";
}

// =============================================================================
// CO4 EXTENSION TESTS: THREE LOGICCRAFT FUNCTIONALITIES AT PROCESSOR LEVEL
// =============================================================================
void testProcessorSimulatorThreeCoreFunctionalities() {
    std::cout << "[COA TEST 8/8] Testing 3 LogicCraft Core Functionalities in CPU Simulator ...\n";
    ProcessorSimulator sim;

    // 1. Logic Gate Evaluation (AND, XOR, NAND)
    sim.demonstrateLogicGateFunctionality("AND", 1, 1);
    assert(sim.getR3() == 1);
    assert(!sim.getZeroFlag());

    sim.demonstrateLogicGateFunctionality("XOR", 1, 1);
    assert(sim.getR3() == 0);
    assert(sim.getZeroFlag());

    sim.demonstrateLogicGateFunctionality("NAND", 1, 1);
    assert(sim.getR3() == 0);
    assert(sim.getZeroFlag());

    // 2. Propagation Delay via Successive Addition Multiplication
    uint64_t delay = sim.demonstratePropagationDelayMultiplication(15, 8);
    assert(delay == 120);
    assert(sim.demonstratePropagationDelayMultiplication(25, 4) == 100);

    // 3. Netlist Gate Type ASCII Opcode Decoding
    assert(sim.demonstrateNetlistOpcodeDecoding("AND") == 1);
    assert(sim.demonstrateNetlistOpcodeDecoding("OR") == 2);
    assert(sim.demonstrateNetlistOpcodeDecoding("NOT") == 3);
    assert(sim.demonstrateNetlistOpcodeDecoding("XOR") == 4);
    assert(sim.demonstrateNetlistOpcodeDecoding("NAND") == 5);
    assert(sim.demonstrateNetlistOpcodeDecoding("NOR") == 6);
    assert(sim.demonstrateNetlistOpcodeDecoding("XNOR") == 7);

    std::cout << " -> 3 LogicCraft Processor-Level Functionalities PASS!\n";
}

void runCOATask4IntegrationSuite() {
    std::cout << "\n======================================================\n";
    std::cout << "     COA / ALP TASK 4 INTEGRATION TEST SUITE (CO4)    \n";
    std::cout << "======================================================\n";
    testApplicationToProcessorMapping();
    testAssemblyArithmeticDelayAndTableIntegration();
    testAssemblyStringAndCommandProcessing();
    testAssemblyErrorHandlingAndBoundaries();
    testALUComplete10OperationsAndFlags();
    testControlUnitSignalsAndTechniques();
    testFiveStagePipelineAndHazards();
    testProcessorSimulatorThreeCoreFunctionalities();
    std::cout << "======================================================\n";
    std::cout << " ALL COA TASK 4 INTEGRATION TESTS PASSED! (CO4)       \n";
    std::cout << "======================================================\n";
}
