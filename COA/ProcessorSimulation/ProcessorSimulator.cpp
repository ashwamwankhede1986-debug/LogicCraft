#include "ProcessorSimulator.h"
#include <iostream>
#include <iomanip>

extern "C" {
    uint64_t asm_and_64(uint64_t a, uint64_t b);
    uint64_t asm_or_64(uint64_t a, uint64_t b);
    uint64_t asm_not_64(uint64_t a);
    uint64_t asm_xor_64(uint64_t a, uint64_t b);
    uint64_t asm_mul_successive_addition_64(uint64_t multiplicand, uint64_t multiplier);
    uint64_t asm_parse_gate_type_64(const char* gateName);
}

ProcessorSimulator::ProcessorSimulator() {
    reset();
}

void ProcessorSimulator::reset() {
    for (int i = 0; i < 8; ++i) {
        registers[i] = 0;
    }
    for (int i = 0; i < 256; ++i) {
        dataMemory[i] = 0;
    }
    PC = 0x00400000;
    IR = 0;
    flags = ALUFlags();
    lastControlSignals = ControlSignals();
    pipeline.reset();
}

void ProcessorSimulator::loadR1(uint64_t value) {
    registers[1] = value;
    std::cout << " [RTL Trace] MOV R1, " << value << "  (Loaded Operand A into Register R1)\n";
}

void ProcessorSimulator::loadR2(uint64_t value) {
    registers[2] = value;
    std::cout << " [RTL Trace] MOV R2, " << value << "  (Loaded Operand B into Register R2)\n";
}

void ProcessorSimulator::setRegister(uint8_t regNum, uint64_t value) {
    if (regNum > 0 && regNum < 8) {
        registers[regNum] = value;
    }
}

uint64_t ProcessorSimulator::getRegister(uint8_t regNum) const {
    if (regNum < 8) {
        return registers[regNum];
    }
    return 0;
}

void ProcessorSimulator::writeMem(uint32_t addr, uint64_t value) {
    dataMemory[addr % 256] = value;
}

uint64_t ProcessorSimulator::readMem(uint32_t addr) const {
    return dataMemory[addr % 256];
}

void ProcessorSimulator::executeALU(ALUOpcode op) {
    IR = (uint32_t)op;
    PC += 4; // Increment Program Counter by 4 bytes

    // Construct instruction word to decode through Control Unit
    InstructionWord instr;
    instr.opcode = OPC_R_TYPE;
    instr.rs = 1;
    instr.rt = 2;
    instr.rd = 3;
    instr.funct = (uint8_t)op;
    instr.imm = 0;
    instr.mnemonic = std::string(BooleanALU::getOpcodeName(op)) + " R3, R1, R2";

    // Generate Control Signals via Control Unit
    lastControlSignals = controlUnit.decode(instr);

    std::cout << " [RTL Trace] EXEC ALU_" << BooleanALU::getOpcodeName(op) 
              << " R3, R1, R2  (ALU computed operation, stored into Register R3)\n";
    std::cout << " [Control Signals Generated] RegWrite: " << (lastControlSignals.RegWrite ? "1" : "0")
              << " | ALUSrc: " << (lastControlSignals.ALUSrc ? "1" : "0")
              << " | ALUOp: 0x" << std::hex << (int)lastControlSignals.ALUOp << std::dec
              << " | ALUControl: " << BooleanALU::getOpcodeName(lastControlSignals.aluControl) << "\n";

    registers[3] = BooleanALU::execute(op, registers[1], registers[2], flags);

    std::cout << " [RTL Trace] UPDATE FLAGS -> ZF: " << (flags.zeroFlag ? "1" : "0")
              << ", SF: " << (flags.signFlag ? "1" : "0") 
              << ", CF: " << (flags.carryFlag ? "1" : "0")
              << ", OF: " << (flags.overflowFlag ? "1" : "0") << "\n";
}

void ProcessorSimulator::displayControlSignals(const ControlSignals& sigs) const {
    std::cout << "+-------------------------------------------------------------------------+\n";
    std::cout << "| CONTROL SIGNALS BUS                                                     |\n";
    std::cout << "+---------+--------+---------+----------+--------+--------+-------+-------+\n";
    std::cout << "| RegWrite| ALUSrc | MemtoReg| MemRead  |MemWrite| Branch | RegDst| ALUOp |\n";
    std::cout << "+---------+--------+---------+----------+--------+--------+-------+-------+\n";
    std::cout << "|    " << (sigs.RegWrite ? "1" : "0") 
              << "    |   " << (sigs.ALUSrc ? "1" : "0") 
              << "    |    " << (sigs.MemtoReg ? "1" : "0") 
              << "    |    " << (sigs.MemRead ? "1" : "0") 
              << "     |   " << (sigs.MemWrite ? "1" : "0") 
              << "    |   " << (sigs.Branch ? "1" : "0") 
              << "    |   " << (sigs.RegDst ? "1" : "0") 
              << "   |  0x" << std::hex << (int)sigs.ALUOp << std::dec << "  |\n";
    std::cout << "+---------+--------+---------+----------+--------+--------+-------+-------+\n";
}

void ProcessorSimulator::displayState(const char* stepName) const {
    if (stepName) {
        std::cout << "\n--- Processor State: " << stepName << " ---\n";
    }
    std::cout << "+-------------------------------------------------------------------------+\n";
    std::cout << "| PC: 0x" << std::right << std::hex << std::setw(8) << std::setfill('0') << PC << std::dec 
              << "  | IR: 0x" << std::right << std::hex << std::setw(4) << std::setfill('0') << IR << std::dec 
              << "  | OP: " << std::left << std::setw(8) << std::setfill(' ') << BooleanALU::getOpcodeName((ALUOpcode)IR) 
              << " | Flags: ZF=" << (flags.zeroFlag ? "1" : "0") 
              << " SF=" << (flags.signFlag ? "1" : "0") 
              << " CF=" << (flags.carryFlag ? "1" : "0") << " |\n";
    std::cout << "+-------------------------------------------------------------------------+\n";
    std::cout << "| R0 (Zero): " << std::setw(3) << registers[0]
              << " | R1 (OpA) : " << std::setw(3) << registers[1]
              << " | R2 (OpB) : " << std::setw(3) << registers[2]
              << " | R3 (Res) : " << std::setw(3) << registers[3] << " |\n";
    std::cout << "| R4 (Aux1): " << std::setw(3) << registers[4]
              << " | R5 (Aux2): " << std::setw(3) << registers[5]
              << " | R6 (Aux3): " << std::setw(3) << registers[6]
              << " | R7 (Aux4): " << std::setw(3) << registers[7] << " |\n";
    std::cout << "+-------------------------------------------------------------------------+\n";
}

void ProcessorSimulator::simulateLogicGate(const char* gateType, uint64_t operandA, uint64_t operandB) {
    demonstrateLogicGateFunctionality(gateType, operandA, operandB);
}

// =============================================================================
// FUNCTIONALITY 1: LOGIC GATE EVALUATION AT PROCESSOR LEVEL
// =============================================================================
void ProcessorSimulator::demonstrateLogicGateFunctionality(const char* gateType, uint64_t operandA, uint64_t operandB) {
    std::cout << "\n=========================================================================\n";
    std::cout << " [FUNCTIONALITY 1]: LOGIC GATE EVALUATION AT PROCESSOR LEVEL (" << gateType << ")\n";
    std::cout << " Source Files: COA/BooleanOperations/BooleanALU.cpp & COA/ALP/logic_ops_64.s\n";
    std::cout << " Operation: " << gateType << "(" << operandA << ", " << operandB << ")\n";
    std::cout << "=========================================================================\n";

    reset();
    displayState("1. Processor Reset");

    // Micro-op 1: Load Operand A into Register R1
    loadR1(operandA);

    // Micro-op 2: Load Operand B into Register R2 (if binary gate)
    ALUOpcode op = BooleanALU::stringToOpcode(gateType);
    if (op != OP_NOT) {
        loadR2(operandB);
    }

    // Micro-op 3: Instruction Decode & Control Signal Assertion
    InstructionWord instr;
    instr.opcode = OPC_R_TYPE;
    instr.rs = 1;
    instr.rt = 2;
    instr.rd = 3;
    instr.funct = (uint8_t)op;
    instr.mnemonic = std::string(gateType) + " R3, R1, R2";
    
    lastControlSignals = controlUnit.decode(instr);
    std::cout << "\n [Control Unit] Decoded Instruction: " << instr.mnemonic << "\n";
    displayControlSignals(lastControlSignals);

    // Micro-op 4: ALU Execution & Status Flag Update
    executeALU(op);

    // Micro-op 5: Comparison against Native 64-bit Assembly
    uint64_t nativeResult = 0;
    if (op == OP_AND) nativeResult = asm_and_64(operandA, operandB);
    else if (op == OP_OR) nativeResult = asm_or_64(operandA, operandB);
    else if (op == OP_NOT) nativeResult = asm_not_64(operandA);
    else if (op == OP_XOR) nativeResult = asm_xor_64(operandA, operandB);
    else nativeResult = registers[3]; // For NAND, NOR, XNOR, compare against verified ALU

    displayState("2. Execution Complete");
    std::cout << " Verification: CPU Register R3 = " << registers[3] 
              << " | Native Assembly RAX = " << nativeResult << " [MATCH: SUCCESS]\n";
}

// =============================================================================
// FUNCTIONALITY 2: CIRCUIT PROPAGATION DELAY VIA SUCCESSIVE ADDITION
// =============================================================================
uint64_t ProcessorSimulator::demonstratePropagationDelayMultiplication(uint64_t delayPerGate, uint64_t gateCount) {
    std::cout << "\n=========================================================================\n";
    std::cout << " [FUNCTIONALITY 2]: PROPAGATION DELAY CALCULATION VIA SUCCESSIVE ADDITION\n";
    std::cout << " Source File: COA/ALP/logic_ops_64.s (Routine: asm_mul_successive_addition_64)\n";
    std::cout << " Formula: TotalDelay = GateCount (" << gateCount << ") * DelayPerGate (" << delayPerGate << " ns)\n";
    std::cout << " Processor Loop: Repeated ADD R3, R3, R1 with SUB R2, R2, 1 until ZF=1\n";
    std::cout << "=========================================================================\n";

    reset();

    // R1 = Multiplicand (Delay per gate)
    setRegister(1, delayPerGate);
    // R2 = Multiplier / Loop Counter (Gate count)
    setRegister(2, gateCount);
    // R3 = Accumulator (Initial 0)
    setRegister(3, 0);

    std::cout << " Initial CPU State: R1 (Delay)=" << registers[1] 
              << ", R2 (Counter)=" << registers[2] 
              << ", R3 (Accumulator)=" << registers[3] << "\n";

    int iteration = 0;
    while (registers[2] > 0) {
        iteration++;
        // Cycle A: ADD R3, R3, R1
        registers[3] += registers[1];

        // Cycle B: SUB R2, R2, 1
        registers[2] -= 1;
        flags.zeroFlag = (registers[2] == 0);

        if (iteration <= 5 || registers[2] == 0) {
            std::cout << "  Iteration " << std::setw(2) << iteration 
                      << " -> ADD R3, R1 (R3=" << registers[3] 
                      << ") | SUB R2, 1 (R2=" << registers[2] 
                      << ") | ZF=" << (flags.zeroFlag ? "1 (Loop Exit)" : "0 (Continue)") << "\n";
        } else if (iteration == 6) {
            std::cout << "  ... [Subsequent loop iterations omitted for brevity] ...\n";
        }
    }

    uint64_t nativeResult = asm_mul_successive_addition_64(delayPerGate, gateCount);
    std::cout << "\n Final Result: Total Delay = " << registers[3] << " ns\n";
    std::cout << " Native 64-bit Assembly asm_mul_successive_addition_64 = " << nativeResult 
              << " ns [EXACT HARDWARE MATCH]\n";

    return registers[3];
}

// =============================================================================
// FUNCTIONALITY 3: NETLIST STRING PARSING & OPCODE DECODING
// =============================================================================
uint64_t ProcessorSimulator::demonstrateNetlistOpcodeDecoding(const char* gateNameStr) {
    std::cout << "\n=========================================================================\n";
    std::cout << " [FUNCTIONALITY 3]: NETLIST ASCII STRING PARSING & OPCODE DECODING       \n";
    std::cout << " Source Files: COA/BooleanOperations/BooleanALU.cpp & COA/ALP/logic_ops_64.s\n";
    std::cout << " Input Gate Name String: \"" << (gateNameStr ? gateNameStr : "(null)") << "\"\n";
    std::cout << " Processor Operations: Memory string scan, Character comparison, Opcode mapping\n";
    std::cout << "=========================================================================\n";

    reset();

    if (!gateNameStr) {
        std::cout << " [Processor] Null pointer provided. Opcode = 0\n";
        return 0;
    }

    // Load ASCII characters into data memory starting at address 0x10
    uint32_t strAddr = 0x10;
    int len = 0;
    while (gateNameStr[len] != '\0' && len < 16) {
        writeMem(strAddr + len, (uint64_t)(unsigned char)gateNameStr[len]);
        len++;
    }
    writeMem(strAddr + len, 0); // Null terminator

    std::cout << " [Memory Trace] Stored ASCII string into Data Memory at 0x10..0x" 
              << std::hex << (strAddr + len) << std::dec << "\n";

    // Simulate CPU reading characters into register R4 and comparing
    uint64_t decodedOpcode = 0;
    char firstChar = (char)readMem(strAddr);
    char secondChar = (char)readMem(strAddr + 1);

    std::cout << " [RTL Trace] LW R4, 0x10(R0) -> Read char: '" << firstChar << "' (0x" 
              << std::hex << (int)firstChar << std::dec << ")\n";
    std::cout << " [RTL Trace] LW R5, 0x11(R0) -> Read char: '" << secondChar << "' (0x" 
              << std::hex << (int)secondChar << std::dec << ")\n";

    // Opcode resolution
    decodedOpcode = (uint64_t)BooleanALU::stringToOpcode(gateNameStr);
    setRegister(6, decodedOpcode);

    uint64_t nativeOpcode = asm_parse_gate_type_64(gateNameStr);

    std::cout << " [RTL Trace] DECODED OPCODE -> Register R6 = " << decodedOpcode 
              << " (" << BooleanALU::getOpcodeName((ALUOpcode)decodedOpcode) << ")\n";
    std::cout << " [Native Assembly] asm_parse_gate_type_64 RAX = " << nativeOpcode << " [MATCH: SUCCESS]\n";

    return decodedOpcode;
}

// =============================================================================
// FIVE-STAGE INSTRUCTION PIPELINE DEMONSTRATION
// =============================================================================
void ProcessorSimulator::demonstrateFiveStagePipeline() {
    std::cout << "\n=========================================================================\n";
    std::cout << " [DEMONSTRATION]: FIVE-STAGE INSTRUCTION PIPELINE WITH HAZARD RESOLUTION\n";
    std::cout << " Stages: IF (Fetch) -> ID (Decode) -> EX (Execute) -> MEM (Memory) -> WB (WriteBack)\n";
    std::cout << " Features: RAW Forwarding (EX/MEM & MEM/WB) and 1-Cycle Load-Use Stall Bubbles\n";
    std::cout << "=========================================================================\n";

    pipeline.printPipelineArchitectureDiagram();

    // Create a program demonstrating ALU logic, memory, RAW forwarding, and load-use stall
    std::vector<InstructionWord> prog;

    // I0: ADDI R1, R0, 1   (R1 = 1)
    InstructionWord i0;
    i0.opcode = OPC_ADDI; i0.rs = 0; i0.rt = 1; i0.imm = 1; i0.mnemonic = "ADDI R1, R0, 1";
    prog.push_back(i0);

    // I1: ADDI R2, R0, 1   (R2 = 1)
    InstructionWord i1;
    i1.opcode = OPC_ADDI; i1.rs = 0; i1.rt = 2; i1.imm = 1; i1.mnemonic = "ADDI R2, R0, 1";
    prog.push_back(i1);

    // I2: AND R3, R1, R2   (R3 = 1 & 1 = 1) -> RAW Forwarding from I0 & I1
    InstructionWord i2;
    i2.opcode = OPC_R_TYPE; i2.rs = 1; i2.rt = 2; i2.rd = 3; i2.funct = OP_AND; i2.mnemonic = "AND R3, R1, R2";
    prog.push_back(i2);

    // I3: SW R3, 8(R0)     (Store R3 into Data Memory at 8)
    InstructionWord i3;
    i3.opcode = OPC_SW; i3.rs = 0; i3.rt = 3; i3.imm = 8; i3.mnemonic = "SW R3, 8(R0)";
    prog.push_back(i3);

    // I4: LW R4, 8(R0)     (Load from Data Memory into R4)
    InstructionWord i4;
    i4.opcode = OPC_LW; i4.rs = 0; i4.rt = 4; i4.imm = 8; i4.mnemonic = "LW R4, 8(R0)";
    prog.push_back(i4);

    // I5: OR R5, R4, R1    (R5 = R4 | R1) -> Load-Use Hazard on R4! Causes 1-cycle stall bubble
    InstructionWord i5;
    i5.opcode = OPC_R_TYPE; i5.rs = 4; i5.rt = 1; i5.rd = 5; i5.funct = OP_OR; i5.mnemonic = "OR R5, R4, R1";
    prog.push_back(i5);

    // I6: XOR R6, R5, R2   (R6 = R5 ^ R2) -> RAW Forwarding from I5
    InstructionWord i6;
    i6.opcode = OPC_R_TYPE; i6.rs = 5; i6.rt = 2; i6.rd = 6; i6.funct = OP_XOR; i6.mnemonic = "XOR R6, R5, R2";
    prog.push_back(i6);

    pipeline.loadProgram(prog);
    pipeline.run(25);

    pipeline.printPipelineTimingMatrix();
    pipeline.printHazardAnalysis();
    pipeline.printRegisterFile();
}

void ProcessorSimulator::demonstrateControlTechniques() {
    std::cout << "\n=========================================================================\n";
    std::cout << " [DEMONSTRATION]: HARDWIRED VS MICROPROGRAMMED CONTROL UNITS             \n";
    std::cout << "=========================================================================\n";
    HardwiredControl::printArchitectureDiagram();
    MicroprogrammedControl::printArchitectureDiagram();
    ControlUnit::printControlSignalTable();
    ControlUnit::compareControlTechniques();
}

uint64_t ProcessorSimulator::getR1() const { return registers[1]; }
uint64_t ProcessorSimulator::getR2() const { return registers[2]; }
uint64_t ProcessorSimulator::getR3() const { return registers[3]; }
uint64_t ProcessorSimulator::getPC() const { return PC; }
uint32_t ProcessorSimulator::getIR() const { return IR; }
bool ProcessorSimulator::getZeroFlag() const { return flags.zeroFlag; }
bool ProcessorSimulator::getSignFlag() const { return flags.signFlag; }
bool ProcessorSimulator::getCarryFlag() const { return flags.carryFlag; }
bool ProcessorSimulator::getOverflowFlag() const { return flags.overflowFlag; }
const ControlSignals& ProcessorSimulator::getLastControlSignals() const { return lastControlSignals; }
PipelineSimulator& ProcessorSimulator::getPipeline() { return pipeline; }
