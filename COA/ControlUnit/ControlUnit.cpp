#include "ControlUnit.h"
#include <iostream>
#include <iomanip>
#include <cstring>

// =============================================================================
// PHASE 2 & 3: CONTROL UNIT & CONTROL SIGNAL GENERATION
// =============================================================================

ControlUnit::ControlUnit() {
}

ALUOpcode ControlUnit::deriveALUControl(uint8_t ALUOp, uint8_t funct) {
    // 2-bit ALUOp decoding standard:
    // ALUOp = 00: Memory Address Calculation (LW, SW, ADDI) -> Force ADD
    // ALUOp = 01: Branch Condition Evaluation (BEQ, BNE)    -> Force SUB
    // ALUOp = 10: R-Type Logic & Arithmetic (AND, OR, XOR, etc.) -> Decode Funct field
    switch (ALUOp) {
        case 0b00:
            return OP_ADD;
        case 0b01:
            return OP_SUB;
        case 0b10:
            switch (funct) {
                case OP_AND:  return OP_AND;
                case OP_OR:   return OP_OR;
                case OP_NOT:  return OP_NOT;
                case OP_XOR:  return OP_XOR;
                case OP_NAND: return OP_NAND;
                case OP_NOR:  return OP_NOR;
                case OP_XNOR: return OP_XNOR;
                case OP_ADD:  return OP_ADD;
                case OP_SUB:  return OP_SUB;
                case OP_CMP:  return OP_CMP;
                default:      return OP_AND;
            }
        default:
            return OP_AND;
    }
}

ControlSignals ControlUnit::generateSignals(const InstructionWord& instr, bool useMicroprogrammed) {
    if (useMicroprogrammed) {
        return microprogrammedUnit.executeMicrocode(instr);
    } else {
        return HardwiredControl::decode(instr);
    }
}

// Instruction Helper Builders
InstructionWord ControlUnit::createRType(const char* name, uint8_t rd, uint8_t rs, uint8_t rt) {
    InstructionWord w;
    w.opcode = OPC_R_TYPE;
    w.rs = rs;
    w.rt = rt;
    w.rd = rd;
    w.funct = (uint8_t)BooleanALU::stringToOpcode(name);
    w.imm = 0;
    w.target = 0;
    w.mnemonic = std::string(name) + " R" + std::to_string(rd) + ", R" + std::to_string(rs) + ", R" + std::to_string(rt);
    return w;
}

InstructionWord ControlUnit::createIType(CPUOpcode op, uint8_t rt, uint8_t rs, int16_t imm, const char* name) {
    InstructionWord w;
    w.opcode = (uint8_t)op;
    w.rs = rs;
    w.rt = rt;
    w.rd = 0;
    w.funct = 0;
    w.imm = imm;
    w.target = 0;
    w.mnemonic = std::string(name) + " R" + std::to_string(rt) + ", " + std::to_string(imm) + "(R" + std::to_string(rs) + ")";
    return w;
}

InstructionWord ControlUnit::createBranch(CPUOpcode op, uint8_t rs, uint8_t rt, int16_t offset, const char* name) {
    InstructionWord w;
    w.opcode = (uint8_t)op;
    w.rs = rs;
    w.rt = rt;
    w.rd = 0;
    w.funct = 0;
    w.imm = offset;
    w.target = 0;
    w.mnemonic = std::string(name) + " R" + std::to_string(rs) + ", R" + std::to_string(rt) + ", offset=" + std::to_string(offset);
    return w;
}

// =============================================================================
// PHASE 4A: HARDWIRED CONTROL IMPLEMENTATION
// =============================================================================

ControlSignals HardwiredControl::decode(const InstructionWord& instr) {
    ControlSignals sig;
    memset(&sig, 0, sizeof(ControlSignals));

    // Combinational Instruction Decoder
    bool is_R    = (instr.opcode == OPC_R_TYPE);
    bool is_LW   = (instr.opcode == OPC_LW);
    bool is_SW   = (instr.opcode == OPC_SW);
    bool is_BEQ  = (instr.opcode == OPC_BEQ);
    bool is_BNE  = (instr.opcode == OPC_BNE);
    bool is_ADDI = (instr.opcode == OPC_ADDI);
    bool is_JMP  = (instr.opcode == OPC_JMP);

    // Combinational Boolean Logic Equations
    sig.RegDst   = is_R;
    sig.ALUSrc   = is_LW || is_SW || is_ADDI;
    sig.MemtoReg = is_LW;
    sig.RegWrite = is_R || is_LW || is_ADDI;
    sig.MemRead  = is_LW;
    sig.MemWrite = is_SW;
    sig.Branch   = is_BEQ || is_BNE;
    sig.Jump     = is_JMP;

    // ALUOp Generation
    if (is_R) {
        sig.ALUOp = 0b10; // R-type class
    } else if (is_BEQ || is_BNE) {
        sig.ALUOp = 0b01; // Branch subtraction class
    } else {
        sig.ALUOp = 0b00; // Address addition class
    }

    // Secondary ALU Control Generation
    sig.aluControl = ControlUnit::deriveALUControl(sig.ALUOp, instr.funct);

    return sig;
}

void HardwiredControl::printArchitectureDiagram() {
    std::cout << "\n";
    std::cout << "  +-------------------------------------------------------------+\n";
    std::cout << "  |          HARDWIRED CONTROL UNIT ARCHITECTURE                |\n";
    std::cout << "  +-------------------------------------------------------------+\n";
    std::cout << "  Instruction Register (IR) [Opcode: bits 31-26]\n";
    std::cout << "               |\n";
    std::cout << "               v\n";
    std::cout << "       +-----------------+\n";
    std::cout << "       | 6-to-64 Opcode  |  -> Activated lines: is_R, is_LW, is_SW,\n";
    std::cout << "       | Decoder Gates   |                      is_BEQ, is_BNE, is_ADDI\n";
    std::cout << "       +-----------------+\n";
    std::cout << "               |\n";
    std::cout << "               v\n";
    std::cout << "       +-----------------------------------------+\n";
    std::cout << "       | COMBINATIONAL CONTROL MATRIX (AND/OR)   |\n";
    std::cout << "       |  RegWrite = is_R | is_LW | is_ADDI      |\n";
    std::cout << "       |  ALUSrc   = is_LW | is_SW | is_ADDI     |\n";
    std::cout << "       |  MemtoReg = is_LW                       |\n";
    std::cout << "       |  MemRead  = is_LW                       |\n";
    std::cout << "       |  MemWrite = is_SW                       |\n";
    std::cout << "       |  Branch   = is_BEQ | is_BNE             |\n";
    std::cout << "       |  ALUOp[1] = is_R                        |\n";
    std::cout << "       |  ALUOp[0] = is_BEQ | is_BNE             |\n";
    std::cout << "       +-----------------------------------------+\n";
    std::cout << "               |\n";
    std::cout << "               +---> [CONTROL BUS TO PROCESSOR DATAPATH]\n";
    std::cout << "                     (Single-cycle propagation delay)\n";
    std::cout << "\n";
}

// =============================================================================
// PHASE 4B: MICROPROGRAMMED CONTROL IMPLEMENTATION
// =============================================================================

MicroprogrammedControl::MicroprogrammedControl() {
    CAR = 0;
    initializeMicrocodeROM();
}

void MicroprogrammedControl::initializeMicrocodeROM() {
    controlMemory.clear();

    // Microcode ROM Layout:
    // Address 0x000: Fetch Micro-routine
    // Address 0x010: R-Type Execution (Logic Gates, Arithmetic)
    // Address 0x020: LW Memory Read
    // Address 0x030: SW Memory Write
    // Address 0x040: BEQ Branch Evaluation
    // Address 0x050: BNE Branch Evaluation
    // Address 0x060: JMP Target Jump

    // 0: Fetch Stage
    MicroInstruction mFetch;
    mFetch.microAddress = 0x000;
    mFetch.microMnemonic = "FETCH: MAR <- PC, IR <- M[MAR], PC <- PC+4";
    memset(&mFetch.signals, 0, sizeof(ControlSignals));
    mFetch.nextAddress = 0x001;
    mFetch.branchCondition = 2; // Dispatch to opcode routine
    controlMemory.push_back(mFetch);

    // 1: R-Type Micro-routine
    MicroInstruction mRType;
    mRType.microAddress = 0x010;
    mRType.microMnemonic = "EXEC_R: RegWrite=1, RegDst=1, ALUOp=10 (R-Type Logic/Math)";
    memset(&mRType.signals, 0, sizeof(ControlSignals));
    mRType.signals.RegDst   = true;
    mRType.signals.ALUSrc   = false;
    mRType.signals.MemtoReg = false;
    mRType.signals.RegWrite = true;
    mRType.signals.ALUOp    = 0b10;
    mRType.nextAddress = 0x000;
    mRType.branchCondition = 3; // Return to Fetch
    controlMemory.push_back(mRType);

    // 2: Load Word (LW)
    MicroInstruction mLW;
    mLW.microAddress = 0x020;
    mLW.microMnemonic = "EXEC_LW: ALUSrc=1, MemRead=1, MemtoReg=1, RegWrite=1";
    memset(&mLW.signals, 0, sizeof(ControlSignals));
    mLW.signals.RegDst   = false;
    mLW.signals.ALUSrc   = true;
    mLW.signals.MemtoReg = true;
    mLW.signals.RegWrite = true;
    mLW.signals.MemRead  = true;
    mLW.signals.ALUOp    = 0b00;
    mLW.nextAddress = 0x000;
    mLW.branchCondition = 3;
    controlMemory.push_back(mLW);

    // 3: Store Word (SW)
    MicroInstruction mSW;
    mSW.microAddress = 0x030;
    mSW.microMnemonic = "EXEC_SW: ALUSrc=1, MemWrite=1, RegWrite=0";
    memset(&mSW.signals, 0, sizeof(ControlSignals));
    mSW.signals.ALUSrc   = true;
    mSW.signals.MemWrite = true;
    mSW.signals.ALUOp    = 0b00;
    mSW.nextAddress = 0x000;
    mSW.branchCondition = 3;
    controlMemory.push_back(mSW);

    // 4: Branch on Equal (BEQ)
    MicroInstruction mBEQ;
    mBEQ.microAddress = 0x040;
    mBEQ.microMnemonic = "EXEC_BEQ: ALUSrc=0, Branch=1, ALUOp=01 (Sub/ZeroCheck)";
    memset(&mBEQ.signals, 0, sizeof(ControlSignals));
    mBEQ.signals.Branch = true;
    mBEQ.signals.ALUOp  = 0b01;
    mBEQ.nextAddress = 0x000;
    mBEQ.branchCondition = 1; // Branch on Zero Flag
    controlMemory.push_back(mBEQ);
}

ControlSignals MicroprogrammedControl::executeMicrocode(const InstructionWord& instr, bool zeroFlag) {
    // Opcode mapping to starting Control Address Register (CAR)
    switch (instr.opcode) {
        case OPC_R_TYPE: CAR = 0x010; break;
        case OPC_LW:     CAR = 0x020; break;
        case OPC_SW:     CAR = 0x030; break;
        case OPC_BEQ:    CAR = 0x040; break;
        case OPC_BNE:    CAR = 0x040; break;
        default:         CAR = 0x010; break;
    }

    // Retrieve microinstruction from ROM
    ControlSignals sig;
    memset(&sig, 0, sizeof(ControlSignals));

    for (const auto& uinst : controlMemory) {
        if (uinst.microAddress == CAR) {
            sig = uinst.signals;
            sig.aluControl = ControlUnit::deriveALUControl(sig.ALUOp, instr.funct);
            break;
        }
    }
    return sig;
}

void MicroprogrammedControl::printMicrocodeROM() const {
    std::cout << "\n+-------------------------------------------------------------------------+\n";
    std::cout << "|                 MICROPROGRAMMED CONTROL STORE (ROM)                     |\n";
    std::cout << "+-------+-------------------------------------------------+---------------+|\n";
    std::cout << "| CAR   | Micro-Operation Description                     | Control Sig   |\n";
    std::cout << "+-------+-------------------------------------------------+---------------+|\n";
    for (const auto& m : controlMemory) {
        std::cout << "| 0x" << std::hex << std::setw(3) << std::setfill('0') << m.microAddress << std::dec << " | "
                  << std::left << std::setw(47) << std::setfill(' ') << m.microMnemonic << " | "
                  << "RegW=" << m.signals.RegWrite << " MemR=" << m.signals.MemRead << " |\n";
    }
    std::cout << "+-------+-------------------------------------------------+---------------+|\n";
}

void MicroprogrammedControl::printArchitectureDiagram() {
    std::cout << "\n";
    std::cout << "  +-------------------------------------------------------------+\n";
    std::cout << "  |          MICROPROGRAMMED CONTROL UNIT ARCHITECTURE          |\n";
    std::cout << "  +-------------------------------------------------------------+\n";
    std::cout << "  Instruction Register (IR) [Opcode]\n";
    std::cout << "               |\n";
    std::cout << "               v\n";
    std::cout << "       +-------------------+\n";
    std::cout << "       | Opcode to Address |  (Mapping ROM / Logic: e.g. R-Type -> 0x010)\n";
    std::cout << "       | Dispatch Logic    |\n";
    std::cout << "       +-------------------+\n";
    std::cout << "               |\n";
    std::cout << "               v\n";
    std::cout << "       +-------------------+\n";
    std::cout << "       |  CAR (Control     | <----+  (Address Sequencer Increment: CAR + 1\n";
    std::cout << "       |  Address Register)|      |   or Conditional Micro-Branch)\n";
    std::cout << "       +-------------------+\n";
    std::cout << "               |\n";
    std::cout << "               v\n";
    std::cout << "       +-------------------+\n";
    std::cout << "       | CONTROL MEMORY    |  (High-speed microcode ROM storing\n";
    std::cout << "       | (ROM Store)       |   horizontal micro-instruction words)\n";
    std::cout << "       +-------------------+\n";
    std::cout << "               |\n";
    std::cout << "       +-------+-------+\n";
    std::cout << "       |               |\n";
    std::cout << "       v               v\n";
    std::cout << "  [CONTROL SIGNALS]  [NEXT ADDRESS FIELD & SEQUENCING LOGIC]\n";
    std::cout << "  RegWrite, ALUSrc,   (Evaluates ZF/SF condition flags to advance CAR)\n";
    std::cout << "  MemtoReg, MemRead...\n";
    std::cout << "\n";
}

// =============================================================================
// PHASE 3: MASTER CONTROL SIGNAL TRUTH TABLE
// =============================================================================

void ControlUnit::printControlSignalTable() {
    std::cout << "\n===========================================================================================\n";
    std::cout << "                    LOGICCRAFT MASTER CONTROL SIGNAL TRUTH TABLE                           \n";
    std::cout << "===========================================================================================\n";
    std::cout << "| Instruction | RegDst | ALUSrc | MemtoReg | RegWrite | MemRead | MemWrite | Branch | ALUOp |\n";
    std::cout << "|-------------|:------:|:------:|:--------:|:--------:|:-------:|:--------:|:------:|:-----:|\n";

    struct TableRow {
        const char* name;
        InstructionWord instr;
    };

    TableRow rows[] = {
        {"AND (Logic)", createRType("AND", 3, 1, 2)},
        {"OR  (Logic)", createRType("OR",  3, 1, 2)},
        {"NOT (Logic)", createRType("NOT", 3, 1, 0)},
        {"XOR (Logic)", createRType("XOR", 3, 1, 2)},
        {"NAND(Logic)", createRType("NAND",3, 1, 2)},
        {"NOR (Logic)", createRType("NOR", 3, 1, 2)},
        {"XNOR(Logic)", createRType("XNOR",3, 1, 2)},
        {"ADD (Arith)", createRType("ADD", 3, 1, 2)},
        {"SUB (Arith)", createRType("SUB", 3, 1, 2)},
        {"CMP (Arith)", createRType("CMP", 0, 1, 2)},
        {"LW  (Memory)",createIType(OPC_LW, 1, 2, 4, "LW")},
        {"SW  (Memory)",createIType(OPC_SW, 1, 2, 4, "SW")},
        {"BEQ (Branch)",createBranch(OPC_BEQ, 1, 2, 8, "BEQ")},
        {"BNE (Branch)",createBranch(OPC_BNE, 1, 2, 8, "BNE")}
    };

    for (const auto& r : rows) {
        ControlSignals s = HardwiredControl::decode(r.instr);
        std::cout << "| " << std::left << std::setw(11) << r.name << " |   "
                  << s.RegDst << "    |   "
                  << s.ALUSrc << "    |    "
                  << s.MemtoReg << "     |    "
                  << s.RegWrite << "     |    "
                  << s.MemRead << "    |    "
                  << s.MemWrite << "     |   "
                  << s.Branch << "    |  "
                  << std::setw(2) << std::setfill('0') << (int)s.ALUOp << std::setfill(' ') << "   |\n";
    }
    std::cout << "===========================================================================================\n";
    std::cout << "Signal Legend:\n";
    std::cout << " - RegDst:   Determines destination register (0 = rt, 1 = rd).\n";
    std::cout << " - ALUSrc:   Selects second ALU operand (0 = Register rt, 1 = Sign-Extended Immediate).\n";
    std::cout << " - MemtoReg: Routes data to register file (0 = ALU Result, 1 = Data Memory Read Output).\n";
    std::cout << " - RegWrite: Enables write strobe to destination register in Register File.\n";
    std::cout << " - MemRead:  Asserts read enable strobe on Data Memory bus.\n";
    std::cout << " - MemWrite: Asserts write enable strobe on Data Memory bus.\n";
    std::cout << " - Branch:   Asserts conditional branch logic (ANDed with ALU Zero Flag for BEQ/BNE).\n";
    std::cout << " - ALUOp:    High-level ALU function category (00=Add/Mem, 01=Branch/Sub, 10=R-type/Gate).\n";
    std::cout << "===========================================================================================\n";
}

void ControlUnit::compareControlTechniques() {
    std::cout << "\n=========================================================================================\n";
    std::cout << "       ARCHITECTURAL COMPARISON: HARDWIRED VS. MICROPROGRAMMED CONTROL                   \n";
    std::cout << "=========================================================================================\n";
    std::cout << "| Architectural Attribute  | Hardwired Control Unit      | Microprogrammed Control Unit |\n";
    std::cout << "|--------------------------|-----------------------------|------------------------------|\n";
    std::cout << "| Operating Speed          | Very Fast (Single-cycle)    | Slower (ROM access cycle)    |\n";
    std::cout << "| Implementation Core      | Fixed combinational gates   | Control Memory (ROM Store)   |\n";
    std::cout << "| Modification/Flexibility | Difficult (Silicon respin)  | Flexible (Update microcode)  |\n";
    std::cout << "| Circuit Complexity       | Grows exponentially with ISA| Scales linearly with ROM size|\n";
    std::cout << "| Instruction Set Match    | RISC Architectures (ARM)    | CISC Architectures (x86)     |\n";
    std::cout << "| Silicon Area Cost        | Compact for small ISA       | Larger due to ROM array      |\n";
    std::cout << "| LogicCraft Usage         | Fast single-cycle gates     | Micro-step education trace   |\n";
    std::cout << "=========================================================================================\n";
}
