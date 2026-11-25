#include "PipelineSimulator.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

PipelineSimulator::PipelineSimulator() {
    reset();
}

void PipelineSimulator::reset() {
    for (int i = 0; i < 8; ++i) {
        regFile[i] = 0;
    }
    for (int i = 0; i < 256; ++i) {
        dataMemory[i] = 0;
    }
    instructionMemory.clear();
    PC = 0;

    IF_ID.clear();
    ID_EX.clear();
    EX_MEM.clear();
    MEM_WB.clear();

    next_IF_ID.clear();
    next_ID_EX.clear();
    next_EX_MEM.clear();
    next_MEM_WB.clear();

    currentCycle = 0;
    totalStalls = 0;
    totalForwardEvents = 0;
    totalFlushes = 0;
    stallRequested = false;
    branchFlushRequested = false;
    branchFlushTarget = 0;

    forwardA = FORWARD_NONE;
    forwardB = FORWARD_NONE;

    history.clear();
}

void PipelineSimulator::loadProgram(const std::vector<InstructionWord>& program) {
    reset();
    instructionMemory = program;
}

void PipelineSimulator::writeDataMem(uint32_t address, uint64_t value) {
    dataMemory[address % 256] = value;
}

uint64_t PipelineSimulator::readDataMem(uint32_t address) const {
    return dataMemory[address % 256];
}

void PipelineSimulator::setRegister(uint8_t regNum, uint64_t value) {
    if (regNum > 0 && regNum < 8) {
        regFile[regNum] = value;
    }
}

uint64_t PipelineSimulator::getRegister(uint8_t regNum) const {
    if (regNum < 8) {
        return regFile[regNum];
    }
    return 0;
}

void PipelineSimulator::evaluateHazardDetection() {
    stallRequested = false;
    // Load-Use Data Hazard condition:
    // If instruction in ID/EX is a memory read (LW) and its destination register (rt) matches
    // either source register (rs or rt) of the instruction currently in IF/ID stage.
    if (ID_EX.valid && !ID_EX.isBubble && ID_EX.ctrl.MemRead && ID_EX.rt != 0) {
        if (IF_ID.valid) {
            uint8_t rs = IF_ID.instr.rs;
            uint8_t rt = IF_ID.instr.rt;
            bool usesRtAsSource = (IF_ID.instr.opcode == OPC_R_TYPE || IF_ID.instr.opcode == OPC_SW || 
                                   IF_ID.instr.opcode == OPC_BEQ || IF_ID.instr.opcode == OPC_BNE);
            
            if (ID_EX.rt == rs || (usesRtAsSource && ID_EX.rt == rt)) {
                stallRequested = true;
            }
        }
    }
}

void PipelineSimulator::evaluateForwardingUnit() {
    forwardA = FORWARD_NONE;
    forwardB = FORWARD_NONE;

    if (!ID_EX.valid || ID_EX.isBubble) return;

    // 1. EX Hazard (Forward from EX/MEM stage to ALU inputs or Store data)
    if (EX_MEM.valid && !EX_MEM.isBubble && EX_MEM.ctrl.RegWrite && EX_MEM.destReg != 0) {
        if (EX_MEM.destReg == ID_EX.rs) {
            forwardA = FORWARD_FROM_MEM;
            totalForwardEvents++;
        }
        if (EX_MEM.destReg == ID_EX.rt && (!ID_EX.ctrl.ALUSrc || ID_EX.instr.opcode == OPC_SW)) {
            forwardB = FORWARD_FROM_MEM;
            totalForwardEvents++;
        }
    }

    // 2. MEM Hazard (Forward from MEM/WB stage to ALU inputs or Store data)
    if (MEM_WB.valid && MEM_WB.ctrl.RegWrite && MEM_WB.destReg != 0) {
        if (forwardA == FORWARD_NONE && MEM_WB.destReg == ID_EX.rs) {
            forwardA = FORWARD_FROM_WB;
            totalForwardEvents++;
        }
        if (forwardB == FORWARD_NONE && MEM_WB.destReg == ID_EX.rt && (!ID_EX.ctrl.ALUSrc || ID_EX.instr.opcode == OPC_SW)) {
            forwardB = FORWARD_FROM_WB;
            totalForwardEvents++;
        }
    }
}

void PipelineSimulator::stageWB() {
    if (MEM_WB.valid) {
        if (MEM_WB.ctrl.RegWrite && MEM_WB.destReg != 0) {
            uint64_t writeVal = MEM_WB.ctrl.MemtoReg ? MEM_WB.memReadData : MEM_WB.aluResult;
            regFile[MEM_WB.destReg] = writeVal;
        }

        StageHistory rec;
        rec.cycle = currentCycle;
        rec.instrIndex = MEM_WB.instrIndex;
        rec.mnemonic = MEM_WB.instr.mnemonic;
        rec.stage = STAGE_WB;
        rec.annotation = "WriteBack -> R" + std::to_string(MEM_WB.destReg);
        history.push_back(rec);
    }
}

void PipelineSimulator::stageMEM() {
    if (EX_MEM.valid) {
        if (EX_MEM.isBubble) {
            next_MEM_WB.clear();
        } else {
            next_MEM_WB.valid = true;
            next_MEM_WB.instr = EX_MEM.instr;
            next_MEM_WB.instrIndex = EX_MEM.instrIndex;
            next_MEM_WB.pc = EX_MEM.pc;
            next_MEM_WB.aluResult = EX_MEM.aluResult;
            next_MEM_WB.destReg = EX_MEM.destReg;
            next_MEM_WB.ctrl = EX_MEM.ctrl;

            if (EX_MEM.ctrl.MemRead) {
                uint32_t addr = (uint32_t)(EX_MEM.aluResult % 256);
                next_MEM_WB.memReadData = dataMemory[addr];
            } else {
                next_MEM_WB.memReadData = 0;
            }

            if (EX_MEM.ctrl.MemWrite) {
                uint32_t addr = (uint32_t)(EX_MEM.aluResult % 256);
                dataMemory[addr] = EX_MEM.writeData;
            }

            StageHistory rec;
            rec.cycle = currentCycle;
            rec.instrIndex = EX_MEM.instrIndex;
            rec.mnemonic = EX_MEM.instr.mnemonic;
            rec.stage = STAGE_MEM;
            if (EX_MEM.ctrl.MemRead) rec.annotation = "MemRead[0x" + std::to_string(EX_MEM.aluResult % 256) + "]";
            else if (EX_MEM.ctrl.MemWrite) rec.annotation = "MemWrite[0x" + std::to_string(EX_MEM.aluResult % 256) + "]";
            else rec.annotation = "Pass";
            history.push_back(rec);
        }
    } else {
        next_MEM_WB.clear();
    }
}

void PipelineSimulator::stageEX() {
    if (ID_EX.valid) {
        if (ID_EX.isBubble) {
            next_EX_MEM.clear();
            next_EX_MEM.isBubble = true;

            StageHistory rec;
            rec.cycle = currentCycle;
            rec.instrIndex = ID_EX.instrIndex;
            rec.mnemonic = ID_EX.instr.mnemonic;
            rec.stage = STAGE_STALL;
            rec.annotation = "Bubble (Stall)";
            history.push_back(rec);
        } else {
            evaluateForwardingUnit();

            // ALU Input A Selection
            uint64_t aluInA = ID_EX.readData1;
            std::string annotA = "";
            if (forwardA == FORWARD_FROM_MEM) {
                aluInA = EX_MEM.aluResult;
                annotA = "(Fwd EX/MEM)";
            } else if (forwardA == FORWARD_FROM_WB) {
                aluInA = MEM_WB.ctrl.MemtoReg ? MEM_WB.memReadData : MEM_WB.aluResult;
                annotA = "(Fwd MEM/WB)";
            }

            // ALU Input B Selection
            uint64_t rawOpB = ID_EX.readData2;
            std::string annotB = "";
            if (forwardB == FORWARD_FROM_MEM) {
                rawOpB = EX_MEM.aluResult;
                annotB = "(Fwd EX/MEM)";
            } else if (forwardB == FORWARD_FROM_WB) {
                rawOpB = MEM_WB.ctrl.MemtoReg ? MEM_WB.memReadData : MEM_WB.aluResult;
                annotB = "(Fwd MEM/WB)";
            }

            uint64_t aluInB = ID_EX.ctrl.ALUSrc ? ID_EX.signExtImm : rawOpB;

            // Execute in ALU
            ALUFlags flags;
            uint64_t aluRes = BooleanALU::execute(ID_EX.ctrl.aluControl, aluInA, aluInB, flags);

            // Register Destination Mux: RegDst ? rd : rt
            uint8_t dReg = ID_EX.ctrl.RegDst ? ID_EX.rd : ID_EX.rt;

            // Branch Condition Evaluation
            bool taken = false;
            if (ID_EX.ctrl.Branch) {
                if (ID_EX.instr.opcode == OPC_BEQ && flags.zeroFlag) taken = true;
                else if (ID_EX.instr.opcode == OPC_BNE && !flags.zeroFlag) taken = true;
            }

            if (taken) {
                branchFlushRequested = true;
                branchFlushTarget = ID_EX.pc + ((int64_t)ID_EX.signExtImm * 4);
                totalFlushes++;
            }

            next_EX_MEM.valid = true;
            next_EX_MEM.isBubble = false;
            next_EX_MEM.instr = ID_EX.instr;
            next_EX_MEM.instrIndex = ID_EX.instrIndex;
            next_EX_MEM.pc = ID_EX.pc;
            next_EX_MEM.aluResult = aluRes;
            next_EX_MEM.writeData = rawOpB;
            next_EX_MEM.destReg = dReg;
            next_EX_MEM.flags = flags;
            next_EX_MEM.branchTarget = ID_EX.pc + ((int64_t)ID_EX.signExtImm * 4);
            next_EX_MEM.branchTaken = taken;
            next_EX_MEM.ctrl = ID_EX.ctrl;

            StageHistory rec;
            rec.cycle = currentCycle;
            rec.instrIndex = ID_EX.instrIndex;
            rec.mnemonic = ID_EX.instr.mnemonic;
            rec.stage = STAGE_EX;
            rec.annotation = "ALU=" + std::to_string(aluRes) + (annotA.empty() ? "" : " " + annotA) + (annotB.empty() ? "" : " " + annotB);
            history.push_back(rec);
        }
    } else {
        next_EX_MEM.clear();
    }
}

void PipelineSimulator::stageID() {
    if (IF_ID.valid) {
        evaluateHazardDetection();

        if (stallRequested) {
            // Load-Use Hazard Stall:
            // 1. Next cycle, ID/EX gets a bubble (zeroed signals)
            next_ID_EX.clear();
            next_ID_EX.valid = true;
            next_ID_EX.isBubble = true;
            next_ID_EX.instrIndex = IF_ID.instrIndex;
            next_ID_EX.instr.mnemonic = "STALL (Bubble)";

            // 2. Retain current IF/ID instruction
            next_IF_ID = IF_ID;
            totalStalls++;

            StageHistory rec;
            rec.cycle = currentCycle;
            rec.instrIndex = IF_ID.instrIndex;
            rec.mnemonic = IF_ID.instr.mnemonic;
            rec.stage = STAGE_STALL;
            rec.annotation = "Stall (Load-Use)";
            history.push_back(rec);
            return;
        }

        // Normal Instruction Decode
        ControlSignals signals = HardwiredControl::decode(IF_ID.instr);

        uint64_t rData1 = regFile[IF_ID.instr.rs];
        uint64_t rData2 = regFile[IF_ID.instr.rt];

        // Register File Internal Forwarding (WB write-during-read)
        if (MEM_WB.valid && MEM_WB.ctrl.RegWrite && MEM_WB.destReg != 0) {
            uint64_t wbVal = MEM_WB.ctrl.MemtoReg ? MEM_WB.memReadData : MEM_WB.aluResult;
            if (MEM_WB.destReg == IF_ID.instr.rs) rData1 = wbVal;
            if (MEM_WB.destReg == IF_ID.instr.rt) rData2 = wbVal;
        }

        next_ID_EX.valid = true;
        next_ID_EX.isBubble = false;
        next_ID_EX.instr = IF_ID.instr;
        next_ID_EX.instrIndex = IF_ID.instrIndex;
        next_ID_EX.pc = IF_ID.pc;
        next_ID_EX.readData1 = rData1;
        next_ID_EX.readData2 = rData2;
        next_ID_EX.signExtImm = (int64_t)IF_ID.instr.imm;
        next_ID_EX.rs = IF_ID.instr.rs;
        next_ID_EX.rt = IF_ID.instr.rt;
        next_ID_EX.rd = IF_ID.instr.rd;
        next_ID_EX.ctrl = signals;

        StageHistory rec;
        rec.cycle = currentCycle;
        rec.instrIndex = IF_ID.instrIndex;
        rec.mnemonic = IF_ID.instr.mnemonic;
        rec.stage = STAGE_ID;
        rec.annotation = "Decoded " + std::string(BooleanALU::getOpcodeName(signals.aluControl));
        history.push_back(rec);
    } else {
        next_ID_EX.clear();
    }
}

void PipelineSimulator::stageIF() {
    if (branchFlushRequested) {
        // Control Hazard: Flush IF/ID and ID/EX, branch target loaded into PC
        PC = branchFlushTarget;
        next_IF_ID.clear();
        branchFlushRequested = false;
        return;
    }

    if (stallRequested) {
        // Frozen PC on stall
        return;
    }

    uint64_t instrIdx = PC / 4;
    if (instrIdx < instructionMemory.size()) {
        next_IF_ID.valid = true;
        next_IF_ID.instr = instructionMemory[instrIdx];
        next_IF_ID.pc = PC;
        next_IF_ID.instrIndex = (int)instrIdx;

        StageHistory rec;
        rec.cycle = currentCycle;
        rec.instrIndex = (int)instrIdx;
        rec.mnemonic = instructionMemory[instrIdx].mnemonic;
        rec.stage = STAGE_IF;
        rec.annotation = "PC=0x" + std::to_string(PC);
        history.push_back(rec);

        PC += 4;
    } else {
        next_IF_ID.clear();
    }
}

bool PipelineSimulator::stepCycle() {
    currentCycle++;

    // Execute 5 pipeline stages in reverse order to simulate register transitions
    stageWB();
    stageMEM();
    stageEX();
    stageID();
    stageIF();

    // Clock Edge: Transfer next states to active pipeline registers
    IF_ID = next_IF_ID;
    ID_EX = next_ID_EX;
    EX_MEM = next_EX_MEM;
    MEM_WB = next_MEM_WB;

    // Active condition: any valid instruction remains in any pipeline register or PC is within bounds
    bool isActive = (IF_ID.valid || ID_EX.valid || EX_MEM.valid || MEM_WB.valid || (PC / 4 < instructionMemory.size()));
    return isActive;
}

void PipelineSimulator::run(int maxCycles) {
    while (currentCycle < maxCycles && stepCycle()) {
        // Continue until pipeline drains or cycle limit reached
    }
}

double PipelineSimulator::getIPC() const {
    if (currentCycle == 0) return 0.0;
    return (double)instructionMemory.size() / (double)currentCycle;
}

void PipelineSimulator::printPipelineTimingMatrix() const {
    std::cout << "\n========================================================================================\n";
    std::cout << "                 FIVE-STAGE INSTRUCTION PIPELINE CLOCK CYCLE TIMING MATRIX             \n";
    std::cout << "========================================================================================\n";

    if (instructionMemory.empty()) {
        std::cout << " [Pipeline] No instructions loaded.\n";
        return;
    }

    int totalCyclesObserved = currentCycle;

    // Header
    std::cout << std::left << std::setw(28) << "Instruction Mnemonic";
    for (int c = 1; c <= totalCyclesObserved; ++c) {
        std::cout << "| CC" << std::setw(2) << c << " ";
    }
    std::cout << "|\n";

    std::cout << std::string(28 + totalCyclesObserved * 7 + 1, '-') << "\n";

    for (size_t i = 0; i < instructionMemory.size(); ++i) {
        std::string label = instructionMemory[i].mnemonic;
        if (label.length() > 26) label = label.substr(0, 26);
        std::cout << std::left << std::setw(28) << label;

        for (int c = 1; c <= totalCyclesObserved; ++c) {
            std::string stageName = "   ";
            for (const auto& h : history) {
                if (h.cycle == c && h.instrIndex == (int)i) {
                    switch (h.stage) {
                        case STAGE_IF:    stageName = " IF "; break;
                        case STAGE_ID:    stageName = " ID "; break;
                        case STAGE_EX:    stageName = " EX "; break;
                        case STAGE_MEM:   stageName = " MEM"; break;
                        case STAGE_WB:    stageName = " WB "; break;
                        case STAGE_STALL: stageName = "STL*"; break;
                        default:          stageName = "   "; break;
                    }
                    break;
                }
            }
            std::cout << "|" << std::setw(5) << stageName << " ";
        }
        std::cout << "|\n";
    }
    std::cout << std::string(28 + totalCyclesObserved * 7 + 1, '-') << "\n";
    std::cout << "Legend: IF = Fetch, ID = Decode, EX = Execute, MEM = Memory, WB = Write-Back, STL* = Stall Bubble\n";
}

void PipelineSimulator::printHazardAnalysis() const {
    std::cout << "\n========================================================================================\n";
    std::cout << "                   PIPELINE PERFORMANCE & HAZARD RESOLUTION REPORT                     \n";
    std::cout << "========================================================================================\n";
    std::cout << " Instructions Completed : " << instructionMemory.size() << "\n";
    std::cout << " Total Clock Cycles     : " << currentCycle << "\n";
    std::cout << " Instructions Per Cycle : " << std::fixed << std::setprecision(3) << getIPC() << " IPC\n";
    std::cout << " Total Stalls Injected  : " << totalStalls << " (Resolved via Hardware Bubble Insertion)\n";
    std::cout << " Total Forward Events   : " << totalForwardEvents << " (Resolved via EX/MEM & MEM/WB Forwarding)\n";
    std::cout << " Total Branch Flushes   : " << totalFlushes << " (Resolved via Pipeline Flush to Branch Target)\n";
    std::cout << "----------------------------------------------------------------------------------------\n";
    std::cout << " Hazard Resolution Mechanism Summary:\n";
    std::cout << " 1. RAW Data Hazards (ALU -> ALU): Forwarded from EX/MEM or MEM/WB register directly\n";
    std::cout << "    into ALU Input A/B multiplexers, eliminating 2 data-hazard stall cycles.\n";
    std::cout << " 2. Load-Use Data Hazards: Detected in ID stage when ID/EX.MemRead is active;\n";
    std::cout << "    Hardware Hazard Detection Unit freezes PC & IF/ID, injecting 1 NOP stall bubble.\n";
    std::cout << " 3. Control Hazards (Branch): Evaluated at EX stage; when taken, flushes speculative\n";
    std::cout << "    instructions from IF/ID, redirecting PC to target address.\n";
    std::cout << "========================================================================================\n";
}

void PipelineSimulator::printRegisterFile() const {
    std::cout << "\n--- Pipeline Register File State (Post-Execution) ---\n";
    std::cout << "+---------------------------------------------------+\n";
    for (int i = 0; i < 8; i += 2) {
        std::cout << "| R" << i << ": 0x" << std::right << std::hex << std::setw(8) << std::setfill('0') << regFile[i] << std::dec
                  << "  |  R" << (i + 1) << ": 0x" << std::right << std::hex << std::setw(8) << std::setfill('0') << regFile[i + 1] << std::dec << " |\n";
    }
    std::cout << "+---------------------------------------------------+\n";
}

void PipelineSimulator::printPipelineArchitectureDiagram() const {
    std::cout << "\n";
    std::cout << "========================================================================================\n";
    std::cout << "            FIVE-STAGE INSTRUCTION PIPELINE HARDWARE ARCHITECTURE DIAGRAM               \n";
    std::cout << "========================================================================================\n";
    std::cout << "                                                                                        \n";
    std::cout << "    +-------+        +-------+        +-------+        +-------+        +-------+       \n";
    std::cout << "    |   IF  | =====> |   ID  | =====> |   EX  | =====> |  MEM  | =====> |   WB  |       \n";
    std::cout << "    | Stage |        | Stage |        | Stage |        | Stage |        | Stage |       \n";
    std::cout << "    +-------+        +-------+        +-------+        +-------+        +-------+       \n";
    std::cout << "        |                |                |                |                |           \n";
    std::cout << "     [IF/ID]          [ID/EX]          [EX/MEM]         [MEM/WB]         [RegFile]      \n";
    std::cout << "     Register         Register         Register         Register         Write-Back     \n";
    std::cout << "        |                |                |                |                ^           \n";
    std::cout << "        |          Hazard Detection       |                |                |           \n";
    std::cout << "        |          Unit (Load-Use)        |                |                |           \n";
    std::cout << "        |                |                |                |                |           \n";
    std::cout << "        +----------------|----------------+                |                |           \n";
    std::cout << "                         v                                 |                |           \n";
    std::cout << "                 Forwarding Unit <-------------------------+                |           \n";
    std::cout << "                 (EX & MEM Hazards)-----------------------------------------+           \n";
    std::cout << "                                                                                        \n";
    std::cout << "========================================================================================\n";
}
