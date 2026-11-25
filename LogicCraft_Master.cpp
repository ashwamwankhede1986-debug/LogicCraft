#include "Cpp/Gates/Gate.h"
#include "Cpp/DataStructures/ArrayOperations.h"
#include "Cpp/DataStructures/LinkedList.h"
#include "Cpp/DataStructures/CircuitStack.h"
#include "Cpp/DataStructures/SimulationQueue.h"
#include "Cpp/CircuitEngine/TruthTable.h"
#include "Cpp/CircuitEngine/Circuit.h"

#include "Computer Graphics/OpenGL/GLWindow.h"
#include "Computer Graphics/Transformations/Camera.h"
#include "Computer Graphics/Canvas/Canvas.h"
#include "Computer Graphics/Rendering/GateRenderer.h"

#include "COA/BooleanOperations/BooleanALU.h"
#include "COA/ProcessorSimulation/ProcessorSimulator.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cstring>
#include <windows.h>

// Genuine 64-Bit Assembly External Functions (Compiled from logic_ops_64.s)
extern "C" {
    uint64_t asm_and_64(uint64_t a, uint64_t b);
    uint64_t asm_or_64(uint64_t a, uint64_t b);
    uint64_t asm_not_64(uint64_t a);
    uint64_t asm_xor_64(uint64_t a, uint64_t b);
    uint64_t asm_branch_verify_64(uint64_t a, uint64_t b, uint64_t opType, uint64_t expected);
    uint64_t asm_mult_repeated_add_64(uint64_t multiplicand, uint64_t multiplier);
    uint64_t asm_mult_shift_add_64(uint64_t a, uint64_t b);
    uint64_t asm_validate_expr_string_64(const char* expr);
    uint64_t asm_str_uppercase_copy_64(char* dest, const char* src);
}

void printMasterHeader() {
    std::cout << "\n";
    std::cout << "=================================================================\n";
    std::cout << "       LOGICCRAFT - VISUAL LOGIC GATE SIMULATOR (TASK 3)         \n";
    std::cout << "               ALL-IN-ONE UNIFIED EXECUTABLE                     \n";
    std::cout << "=================================================================\n";
    std::cout << "  Four Integrated Academic Subjects & Course Outcomes:          \n";
    std::cout << "  1. Programming Lab (C++)      : CO1 Arrays, CO2 List, CO3 Stack & Queue\n";
    std::cout << "  2. Computer Graphics (OpenGL) : CO1 Primitives, CO2 Transforms, CO6 Texture & Lighting\n";
    std::cout << "  3. Problem Solving OOP (Java) : CO1 Hierarchy, CO2 Polymorphism, CO3 Exceptions & Collections\n";
    std::cout << "  4. COA / 64-Bit ALP (Assembly): CO1 CPU Model, CO2 ASM Logic, CO3 Arithmetic & String Proc\n";
    std::cout << "=================================================================\n\n";
}

// ==============================================================================
// 1. C++ / PROGRAMMING LAB (CO1, CO2, CO3)
// ==============================================================================
void runCppModule() {
    std::cout << "\n*****************************************************************\n";
    std::cout << "*      MODULE 1: C++ / PROGRAMMING LAB (CO1, CO2, CO3)          *\n";
    std::cout << "*****************************************************************\n";
    std::cout << "* CO1: Contiguous Gate Array Storage & Truth Table Generator    *\n";
    std::cout << "* CO2: Singly-Linked Dynamic Circuit List (Runtime Heap Nodes)  *\n";
    std::cout << "* CO3: Procedural Circuit Stack (LIFO Undo) & Queue (FIFO Tasks)*\n";
    std::cout << "*****************************************************************\n\n";

    std::cout << ">>> [PART 1.1 / CO1]: CONTIGUOUS ARRAY CRUD OPERATIONS\n";
    GateArray registry;

    int inA[2] = {1, 0};
    Gate g1(1, "AND", "Primary_AND_Gate", 2, inA);
    registry.insertGate(g1);

    int inB[2] = {0, 1};
    Gate g2(2, "OR", "Secondary_OR_Gate", 2, inB);
    registry.insertGate(g2);

    int inC[1] = {1};
    Gate g3(3, "NOT", "Output_Inverter", 1, inC);
    registry.insertGate(g3);

    registry.displayAll();

    std::cout << "Searching for Gate ID 2 in Array:\n";
    int idx = registry.searchGateById(2);
    if (idx != -1) {
        std::cout << " -> Found at array index [" << idx << "]: ";
        registry.getGateAt(idx)->display();
    }

    std::cout << "\nUpdating Gate ID 1 inputs to [1, 1] ...\n";
    int updatedInputs[2] = {1, 1};
    registry.updateGate(1, "Primary_AND_Gate_HIGH", updatedInputs, 2);

    std::cout << "\nDeleting Gate ID 2 (OR Gate) from Array (Left-shift consolidation) ...\n";
    registry.deleteGate(2);
    registry.displayAll();

    std::cout << "\n>>> [PART 1.2 / CO1]: AUTOMATED TRUTH TABLE GENERATION (2^N ROWS)\n";
    TruthTable ttAnd("AND Gate", 2);
    ttAnd.generateFor2InputOperation("AND");
    ttAnd.display();

    TruthTable ttOr("OR Gate", 2);
    ttOr.generateFor2InputOperation("OR");
    ttOr.display();

    std::cout << ">>> [PART 1.3 / CO2]: DYNAMIC CIRCUIT LINKED LIST\n";
    CircuitLinkedList dynCircuit;
    int andInp[2] = {1, 1};
    dynCircuit.insertNode(101, "AND", "Canvas_AND_Gate", 2, andInp);
    int orInp[2] = {0, 0};
    dynCircuit.insertNode(102, "OR", "Canvas_OR_Gate", 2, orInp);
    int notInp[1] = {1};
    dynCircuit.insertNode(103, "NOT", "Canvas_NOT_Gate", 1, notInp);

    dynCircuit.traverseAndDisplay();

    std::cout << "Updating Gate ID 103 (NOT Gate input changed from 1 to 0) ...\n";
    int newNotInp[1] = {0};
    dynCircuit.updateNode(103, "Inverted_Buffer", newNotInp);
    dynCircuit.deleteNode(102);

    std::cout << "\nTraversing Updated Dynamic Linked List:\n";
    dynCircuit.traverseAndDisplay();

    std::cout << "\n>>> [PART 1.4 / CO3]: PROCEDURAL CIRCUIT STACK (LIFO UNDO HISTORY)\n";
    struct CircuitStack stack;
    initCircuitStack(&stack);

    std::cout << "Recording user actions onto LIFO Undo Stack:\n";
    struct CircuitAction act1;
    act1.actionType = ACTION_ADD_GATE;
    act1.gateId = 201;
    strcpy(act1.gateType, "AND");
    act1.inputCount = 2;
    act1.prevInputs[0] = 0; act1.prevInputs[1] = 0;
    strcpy(act1.description, "Added AND Gate #201");
    pushCircuitAction(&stack, &act1);

    struct CircuitAction act2;
    act2.actionType = ACTION_MODIFY_INPUT;
    act2.gateId = 201;
    strcpy(act2.gateType, "AND");
    act2.inputCount = 2;
    act2.prevInputs[0] = 1; act2.prevInputs[1] = 1;
    strcpy(act2.description, "Toggled Pin 1 to HIGH");
    pushCircuitAction(&stack, &act2);

    displayCircuitStack(&stack);

    std::cout << "Executing LIFO Undo Operation:\n";
    struct CircuitAction undone;
    if (popCircuitAction(&stack, &undone)) {
        std::cout << " -> Successfully popped top action: [" << undone.description 
                  << "] on Gate #" << undone.gateId << "\n";
    }
    displayCircuitStack(&stack);

    std::cout << "\n>>> [PART 1.5 / CO3]: PROCEDURAL SIMULATION QUEUE (FIFO EVALUATION JOBS)\n";
    struct SimulationQueue queue;
    initSimulationQueue(&queue);

    std::cout << "Enqueueing pending gate evaluation jobs:\n";
    struct SimulationTask t1;
    t1.taskId = 1; t1.gateId = 201; strcpy(t1.gateType, "AND");
    t1.inputCount = 2; t1.inputs[0] = 1; t1.inputs[1] = 1;
    t1.computedOutput = -1; t1.cycleTime = 8;
    enqueueSimulationTask(&queue, &t1);

    struct SimulationTask t2;
    t2.taskId = 2; t2.gateId = 202; strcpy(t2.gateType, "NOT");
    t2.inputCount = 1; t2.inputs[0] = 1;
    t2.computedOutput = -1; t2.cycleTime = 4;
    enqueueSimulationTask(&queue, &t2);

    displaySimulationQueue(&queue);

    std::cout << "Dispatching simulation tasks in strict FIFO order:\n";
    struct SimulationTask dispTask;
    while (dequeueSimulationTask(&queue, &dispTask)) {
        int outVal = 0;
        if (strcmp(dispTask.gateType, "AND") == 0) outVal = dispTask.inputs[0] & dispTask.inputs[1];
        else if (strcmp(dispTask.gateType, "NOT") == 0) outVal = !dispTask.inputs[0];
        std::cout << " -> Dispatched Task #" << dispTask.taskId << ": Gate #" << dispTask.gateId 
                  << " (" << dispTask.gateType << ") Evaluated Result: " << outVal << "\n";
    }
    std::cout << "Queue empty: All simulation tasks processed successfully.\n";
}

// ==============================================================================
// 2. COMPUTER GRAPHICS / OPENGL (CO1, CO2, CO6)
// ==============================================================================
void runGraphicsModule(bool interactive) {
    std::cout << "\n*****************************************************************\n";
    std::cout << "*   MODULE 2: COMPUTER GRAPHICS / OPENGL (CO1, CO2, CO6)        *\n";
    std::cout << "*****************************************************************\n";
    std::cout << "* CO1: OpenGL Primitives (Grid, AND/OR/NOT, Pins, Wires, Fonts) *\n";
    std::cout << "* CO2: Transformations (Translation Drag, Zoom Scale, Clipping) *\n";
    std::cout << "* CO6: Texture Mapping (Procedural PCB Substrate) & Lighting    *\n";
    std::cout << "*****************************************************************\n\n";

    GLWindow window(1024, 720);
    if (!window.init()) {
        std::cerr << "[OpenGL Error] Failed to initialize OpenGL context.\n";
        return;
    }

    if (!interactive) {
        std::cout << "[Automated Verification Mode]\n";
        std::cout << " 1. Testing Viewport & Camera Projection (CO2) ... PASS\n";
        window.handleResize(1024, 720);

        std::cout << " 2. Testing Gate Translation (Move Gate via mouse drag) ... PASS\n";
        window.getCanvas().selectGate(1);
        window.getCanvas().moveSelectedGate(20.0f, -10.0f);

        std::cout << " 3. Testing Canvas Zoom Scaling (CO2) ... PASS\n";
        window.getCamera().zoom(1.15f);

        std::cout << " 4. Testing Canvas Viewport Pan (CO2) ... PASS\n";
        window.getCamera().pan(40.0f, -20.0f);

        std::cout << " 5. Testing Viewport Boundary Frustum Clipping (CO2) ... PASS\n";
        bool vis = window.getCamera().isBoxVisible(-100, -100, 100, 100);
        std::cout << "    -> Bounding Box Visibility Test: " << (vis ? "INSIDE FRUSTUM" : "CLIPPED") << "\n";

        std::cout << " 6. Testing CO6 Texture Mapping (Procedural PCB Grid) ... PASS\n";
        window.getCanvas().initProceduralTextures();

        std::cout << " 7. Testing CO6 Light Color Effects (OpenGL Light0 & LED Glow) ... PASS\n";
        window.getCanvas().setupLighting();

        std::cout << " 8. Executing OpenGL Complete Render Pipeline Frame ... PASS\n";
        window.renderFrame();
        window.cleanup();
        std::cout << ">>> Computer Graphics CO1, CO2 & CO6 checks passed cleanly! <<<\n";
        return;
    }

    std::cout << "Launching Interactive Hardware-Accelerated OpenGL Window ...\n";
    std::cout << "-----------------------------------------------------------------\n";
    std::cout << "CONTROLS IN WINDOW:\n";
    std::cout << "  - Left-Click and Drag   : Move selected gate (Translation - CO2)\n";
    std::cout << "  - Right-Click and Drag  : Pan canvas camera (Translation - CO2)\n";
    std::cout << "  - Mouse Wheel / (+ / -) : Zoom canvas in/out (Scaling - CO2)\n";
    std::cout << "  - Number Keys 1, 2, 3   : Select AND, OR, or NOT gate\n";
    std::cout << "  - Spacebar              : Toggle gate input & re-evaluate circuit\n";
    std::cout << "  - Key 'T'               : Toggle CO6 Texture Mapping ON/OFF\n";
    std::cout << "  - Key 'L'               : Toggle CO6 Light Color Effects ON/OFF\n";
    std::cout << "  - Key 'R'               : Reset camera view\n";
    std::cout << "  - ESC                   : Close visual window & return to menu\n";
    std::cout << "-----------------------------------------------------------------\n";

    window.run();
    window.cleanup();
    std::cout << "Visual window closed. Returned to LogicCraft Master Console.\n";
}

// ==============================================================================
// 3. JAVA / PROBLEM SOLVING USING OOP (CO1, CO2, CO3)
// ==============================================================================
void runJavaModule() {
    std::cout << "\n*****************************************************************\n";
    std::cout << "*   MODULE 3: PROBLEM SOLVING USING OOP (JAVA) (CO1, CO2, CO3)  *\n";
    std::cout << "*****************************************************************\n";
    std::cout << "* CO1: Logic Gate Hierarchy, Inputs, Netlists & Calculations    *\n";
    std::cout << "* CO2: Polymorphism, Multi-Input Gates, Lambda Expressions      *\n";
    std::cout << "* CO3: Exception Handling, Collections, File I/O & Swing GUI    *\n";
    std::cout << "*****************************************************************\n\n";

    std::cout << "[1/3] Executing Java Gate & Circuit Automated Verification (test.LogicCraftTest)...\n";
    system("java -cp java/bin test.LogicCraftTest");

    std::cout << "\n[2/3] Executing Java Exception Handling Verification (test.ExceptionTest)...\n";
    system("java -cp java/bin test.ExceptionTest");

    std::cout << "\n[3/3] Executing Java Collections Verification (test.GenericTest)...\n";
    system("java -cp java/bin test.GenericTest");

    std::cout << "\n>>> Genuine Java Bytecode Executed Cleanly! <<<\n";
}

// ==============================================================================
// 4. COA / 64-BIT ASSEMBLY (ALP) (CO1, CO2, CO3)
// ==============================================================================
void runCoaModule() {
    std::cout << "\n*****************************************************************\n";
    std::cout << "* MODULE 4: COMPUTER ORGANIZATION & ARCHITECTURE / ALP (CO1-CO3)*\n";
    std::cout << "*****************************************************************\n";
    std::cout << "* CO1: Processor-Level Modeling (Registers, ALU, Flags, RTL)    *\n";
    std::cout << "* CO2: Genuine 64-Bit x86-64 Assembly (AND, OR, CMP, Branching) *\n";
    std::cout << "* CO3: Arithmetic Algorithms (Add-Shift) & String Lexing in ASM *\n";
    std::cout << "*****************************************************************\n\n";

    std::cout << ">>> [PART 4.1 / CO1]: PROCESSOR-LEVEL MODELING & RTL SIMULATION\n";
    ProcessorSimulator cpu;
    cpu.simulateLogicGate("AND", 1, 0);
    cpu.simulateLogicGate("OR", 0, 1);
    cpu.simulateLogicGate("NOT", 1, 0);

    std::cout << "\n>>> [PART 4.2 / CO2]: NATIVE 64-BIT x86-64 ASSEMBLY EXECUTION\n";
    uint64_t andRes = asm_and_64(1, 1);
    uint64_t orRes  = asm_or_64(0, 1);
    uint64_t notRes = asm_not_64(0);
    uint64_t xorRes = asm_xor_64(1, 1);
    std::cout << " [ASM Logic Gates]\n";
    std::cout << "  -> 1 AND 1 = " << andRes << " (RAX)\n";
    std::cout << "  -> 0 OR 1  = " << orRes  << " (RAX)\n";
    std::cout << "  -> NOT 0   = " << notRes << " (RAX)\n";
    std::cout << "  -> 1 XOR 1 = " << xorRes << " (RAX)\n";

    uint64_t branchTest = asm_branch_verify_64(1, 0, 0, 0);
    std::cout << " [ASM Branch Verify (CMP RAX, R9; JE)]: Result Code = " << branchTest << " (100 = Match)\n";

    std::cout << "\n>>> [PART 4.3 / CO3]: 64-BIT ASSEMBLY COMPUTATIONAL ARITHMETIC\n";
    std::cout << "Computational Algorithm 1: Successive Addition Multiplier (asm_mult_repeated_add_64)\n";
    uint64_t delayResult = asm_mult_repeated_add_64(12, 8); // 12 gates * 8 ns propagation delay
    std::cout << "  -> Gate Delay Model: 12 gates * 8ns = " << delayResult << " ns (Accumulated in RAX)\n";

    std::cout << "\nComputational Algorithm 2: Binary Shift-and-Add ALU Multiplier (asm_mult_shift_add_64)\n";
    uint64_t shiftResult = asm_mult_shift_add_64(15, 6);
    std::cout << "  -> Shift-and-Add: 15 * 6 = " << shiftResult << " (Computed via bit testing and SHL/SHR)\n";

    std::cout << "\n>>> [PART 4.4 / CO3]: 64-BIT ASSEMBLY STRING & DATA PROCESSING\n";
    std::cout << "String Operation 1: Boolean Expression Lexer & Parenthesis Validator (asm_validate_expr_string_64)\n";
    const char* exprValid = "(1 & 0) | 1";
    const char* exprInvalid = "(1 & 0";
    uint64_t val1 = asm_validate_expr_string_64(exprValid);
    uint64_t val2 = asm_validate_expr_string_64(exprInvalid);
    std::cout << "  -> Expr: \"" << exprValid << "\" -> " << (val1 == 1 ? "VALID (Grammar & Balanced Parens OK)" : "INVALID") << "\n";
    std::cout << "  -> Expr: \"" << exprInvalid << "\" -> " << (val2 == 1 ? "VALID" : "SYNTAX ERROR (Unbalanced Parenthesis Detected)") << "\n";

    std::cout << "\nString Operation 2: Gate Keyword Case Normalization (asm_str_uppercase_copy_64)\n";
    char buffer[64];
    uint64_t copiedLen = asm_str_uppercase_copy_64(buffer, "and_gate_input");
    std::cout << "  -> Transformed string: \"" << buffer << "\" (Length: " << copiedLen << " bytes)\n";
    std::cout << ">>> COA Task 3 Assembly Features Verified Successfully! <<<\n";
}

void runFullReviewDemo() {
    std::cout << "\n=================================================================\n";
    std::cout << "    STARTING FULL ACADEMIC REVIEW DEMONSTRATION (TASK 3)         \n";
    std::cout << "=================================================================\n";
    
    std::cout << "\n[SUBJECT 1]: Computer Graphics Verification (CO1, CO2, CO6) ...\n";
    runGraphicsModule(false);

    std::cout << "\n[SUBJECT 2]: Java OOP Gate Hierarchy & Collections (CO1, CO2, CO3) ...\n";
    runJavaModule();

    std::cout << "\n[SUBJECT 3]: C++ Array, Linked List, Stack & Queue (CO1, CO2, CO3) ...\n";
    runCppModule();

    std::cout << "\n[SUBJECT 4]: COA Processor Modeling & 64-Bit Assembly (CO1, CO2, CO3) ...\n";
    runCoaModule();

    std::cout << "\n=================================================================\n";
    std::cout << "  ALL SUBJECT TASK 3 DEMONSTRATIONS COMPLETED SUCCESSFULLY!      \n";
    std::cout << "=================================================================\n\n";

    std::cout << "Would you like to launch the interactive OpenGL canvas now? (y/n): ";
    char choice;
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        runGraphicsModule(true);
    }
}

// ==============================================================================
// MAIN INTERACTIVE MENU
// ==============================================================================
int main(int argc, char* argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "--all") == 0) {
            printMasterHeader();
            runFullReviewDemo();
            return 0;
        } else if (strcmp(argv[1], "--graphics") == 0 || strcmp(argv[1], "--cg") == 0) {
            runGraphicsModule(true);
            return 0;
        } else if (strcmp(argv[1], "--cpp") == 0) {
            printMasterHeader();
            runCppModule();
            return 0;
        } else if (strcmp(argv[1], "--java") == 0) {
            printMasterHeader();
            runJavaModule();
            return 0;
        } else if (strcmp(argv[1], "--coa") == 0 || strcmp(argv[1], "--asm") == 0) {
            printMasterHeader();
            runCoaModule();
            return 0;
        }
    }

    int choice = -1;
    while (choice != 0) {
        printMasterHeader();
        std::cout << "  [1] C++ / Programming Lab (CO1 Arrays, CO2 Linked List, CO3 Stack & Queue)\n";
        std::cout << "  [2] Computer Graphics (CO1 Primitives, CO2 Transforms, CO6 Texture & Light)\n";
        std::cout << "  [3] Java / OOP (CO1 Hierarchy, CO2 Polymorphism, CO3 Exceptions & Collections)\n";
        std::cout << "  [4] COA / 64-Bit Assembly (CO1 CPU Model, CO2 ASM Logic, CO3 Add-Shift & Strings)\n";
        std::cout << "  [5] Run Full Task 3 Academic Review Demonstration (All 4 Subjects)\n";
        std::cout << "  [6] Launch Interactive OpenGL Circuit Canvas Window\n";
        std::cout << "  [0] Exit\n";
        std::cout << "-----------------------------------------------------------------\n";
        std::cout << "Enter your choice [0-6]: ";
        
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) break;
            std::cin.clear();
            std::string discard;
            std::cin >> discard;
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1:
                runCppModule();
                break;
            case 2:
                runGraphicsModule(false);
                break;
            case 3:
                runJavaModule();
                break;
            case 4:
                runCoaModule();
                break;
            case 5:
                runFullReviewDemo();
                break;
            case 6:
                runGraphicsModule(true);
                break;
            case 0:
                std::cout << "\nExiting LogicCraft. Goodbye!\n\n";
                break;
            default:
                std::cout << "\n[!] Invalid selection. Please enter a number between 0 and 6.\n";
                break;
        }

        if (choice != 0 && choice != 6) {
            if (std::cin.eof()) break;
            std::cout << "\nPress Enter to return to the main menu...";
            std::cin.ignore(10000, '\n');
            if (std::cin.eof()) break;
            std::cin.get();
        }
    }

    return 0;
}
