# LogicCraft — Comprehensive Course Outcome (CO) to File Mapping Matrix

**Project Title:** LogicCraft — Visual Logic Gate Simulator  
**Task Phase:** Task 4 Implementation & Multi-Subject Integration  
**Academic Target Date:** 24 October 2026 (Verified & Complete)  
**Authoritative Documentation for Examiner Viva & Code Review Inspection**

---

## 1. Master Course Outcome (CO) Mapping Matrix (CO1 to CO6)

| Subject | CO | Task | Concept / Feature Implemented | Exact Source File Path | Important Functions / Classes | How to Demonstrate / Run | Test File Path |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Programming Lab (C++)** | **CO1** | Task 1 | 1D & 2D Arrays, Procedural Gate Table & State Vectors | `Cpp/DataStructures/ArrayOperations.cpp`<br>`Cpp/CircuitEngine/Circuit.cpp` | `initGateArray()`, `insertGate()`, `deleteGate()`, `printGateArray()`, `evaluateGateArray()` | Run `Cpp/Tests/test_runner.exe` (Suite 1: Array Operations). | `Cpp/Tests/TestRunner.cpp` |
| **Programming Lab (C++)** | **CO2** | Task 2 | Singly Linked List for Dynamic Circuit Wiring Connections | `Cpp/DataStructures/LinkedList.cpp`<br>`Cpp/CircuitEngine/Circuit.cpp` | `initConnectionList()`, `insertConnection()`, `removeConnection()`, `freeConnectionList()` | Run `Cpp/Tests/test_runner.exe` (Suite 2: Linked List Wires). | `Cpp/Tests/TestRunner.cpp` |
| **Programming Lab (C++)** | **CO3** | Task 3 | LIFO Stack for Circuit Undo & Circular FIFO Event Queue | `Cpp/DataStructures/StackOperations.cpp`<br>`Cpp/DataStructures/QueueOperations.cpp` | `initUndoStack()`, `pushAction()`, `popAction()`, `enqueueEvent()`, `dequeueEvent()`, `executeUndoOperation()` | Run `Cpp/Tests/test_runner.exe` (Suite 3: Stack/Queue). In web canvas, click `↺ Undo`. | `Cpp/Tests/TestRunner.cpp` |
| **Programming Lab (C++)** | **CO4** | Task 4 | Linear Search, Binary Search, Bubble Sort & Selection Sort | `Cpp/Searching/Searching.cpp`<br>`Cpp/Sorting/Sorting.cpp`<br>`Cpp/Algorithms/PerformanceComparison.cpp` | `linearSearchById()`, `linearSearchByName()`, `binarySearchById()`, `binarySearchByName()`, `bubbleSortComponents()`, `selectionSortComponents()` | Run `Cpp/Tests/test_runner.exe` (Suite 4: Search & Sort). Web UI: "Task 4" Tab -> Search & Sort cards. | `Cpp/Tests/Task4Tests.cpp` |
| **Programming Lab (C++)** | **CO5** | Task 4 | Open-Addressing Hash Table with Linear Probing Collision Handling | `Cpp/Hashing/HashTable.cpp` | `initHashTable()`, `insertHashTable()`, `searchHashTableById()`, `deleteHashTableById()`, `printHashTable()`, `getHashTableLoadFactor()` | Run `Cpp/Tests/test_runner.exe` (Suite 5: Hash Table). Web UI: "Task 4" Tab -> Hash Table Grid & Collision Demo. | `Cpp/Tests/Task4Tests.cpp` |
| **Problem Solving (C++)** | **CO4** | Task 4 | Smart Circuit Validation & Decision-Making Recommendation Engine | `Cpp/CircuitEngine/CircuitValidator.cpp` | `validateCircuit()`, `hasCyclicDependency()`, `runCircuitValidationScenarios()`, `printValidationReport()` | Run `Cpp/Tests/test_runner.exe` (Suite 6: Validator). Web UI: "Task 4" Tab -> Circuit Validation Scenarios 1, 2, 3. | `Cpp/Tests/Task4Tests.cpp` |
| **COA Lab** | **CO1** | Task 1 | 8-Bit Register Processor Simulator & Instruction Fetch-Decode-Execute | `COA/ProcessorSimulation/ProcessorSimulator.cpp` | `ProcessorSimulator::loadProgram()`, `ProcessorSimulator::step()`, `ProcessorSimulator::run()` | Run `COA/Tests/test_alp.exe` (Test 1: Processor Simulator). | `COA/Tests/TestALP.cpp` |
| **COA Lab** | **CO2** | Task 2 | 64-Bit AMD64 Assembly Logic Gate Operations (AND, OR, NOT, XOR, NAND, NOR, XNOR) | `COA/ALP/logic_ops_64.s`<br>`COA/BooleanOperations/BooleanALU.cpp` | `asm_and_64`, `asm_or_64`, `asm_not_64`, `asm_xor_64`, `asm_nand_64`, `asm_nor_64`, `asm_xnor_64` | Run `COA/Tests/test_alp.exe` (Test 2: 64-Bit Logic Operations). | `COA/Tests/TestALP.cpp` |
| **COA Lab** | **CO3** | Task 3 | Propagation Delay Successive Addition, Truth Table Row Doubling & String Parsing | `COA/ALP/logic_ops_64.s` | `asm_mul_successive_addition_64`, `asm_compute_truth_table_rows_64`, `asm_parse_gate_type_64`, `asm_validate_command_64` | Run `COA/Tests/test_alp.exe` (Test 3: Arithmetic & Strings). | `COA/Tests/TestALP.cpp` |
| **COA Lab** | **CO4** | Task 4 | Application-to-Processor Mapping, Register Verification & Error Handling | `COA/Tests/Task4IntegrationTests.cpp`<br>`COA/main.cpp` | `asm_branch_verify_64`, `demonstrateCO4_IntegrationAndDemonstration()`, `testCOAApplicationProcessorMapping()` | Run `COA/Tests/test_alp.exe` (Test 4: Task 4 Integration Suite). | `COA/Tests/Task4IntegrationTests.cpp` |
| **Computer Graphics Lab** | **CO1** | Task 1 | 2D Primitives: DDA / Bresenham Line Drawing & Mid-point Circle Wire Terminals | `Computer Graphics/Rendering/GateRenderer.cpp` | `GateRenderer::drawLine()`, `GateRenderer::drawCircle()`, `GateRenderer::drawRectangle()` | Run `logiccraft_cg.exe --test` (Primitive checks). | `Computer Graphics/main.cpp` |
| **Computer Graphics Lab** | **CO2** | Task 2 | 2D Transformations (Translate, Scale, Zoom, Pan) & Cohen-Sutherland Clipping | `Computer Graphics/Transformations/Camera.cpp`<br>`Computer Graphics/Canvas/Canvas.cpp` | `Camera::pan()`, `Camera::zoom()`, `Camera::applyTransformations()`, `Canvas::clipWire()` | Run `logiccraft_cg.exe --test` (Transformation and clipping checks). | `Computer Graphics/main.cpp` |
| **Computer Graphics Lab** | **CO4** | Task 4 | Authentic ANSI/IEEE Logic Gate Symbol Shapes & Pin Coordinate Precision | `Computer Graphics/Rendering/GateRenderer.cpp`<br>`Computer Graphics/Canvas/Canvas.cpp` | `GateRenderer::drawGate()`, `drawANDGate()`, `drawNANDGate()`, `drawORGate()`, `drawNORGate()`, `drawXORGate()`, `drawXNORGate()`, `drawNOTGate()`, `drawInversionBubble()`, `getGatePinPosition()`, `Canvas::initAll7GatesShowcase()` | Run `logiccraft_cg.exe --test` (Test 8 & 9). In interactive GUI (`run_graphics.bat`), press `[7]` for 7-gate showcase or `[G]` to cycle gate types. | `Computer Graphics/main.cpp` |
| **Computer Graphics Lab** | **CO6** | Task 3 | Texture Mapping (PCB & Chip) and Material Lighting / Color Effects | `Computer Graphics/Rendering/GateRenderer.cpp` | `GateRenderer::initTextures()`, `GateRenderer::initLighting()`, `GateRenderer::drawTexturedSubstrate()`, `GateRenderer::applyMaterial()` | Press `[T]` to toggle textures, `[L]` to toggle lighting in `run_graphics.bat`. | `Computer Graphics/main.cpp` |
| **Problem Solving (Java)** | **CO1** | Task 1 | Core OOP: Encapsulation, Classes, Objects, LogicGate State | `java/src/logiccraft/gates/LogicGate.java`<br>`java/src/logiccraft/circuit/Circuit.java` | `LogicGate`, `Circuit`, `GateFactory`, `Connection` | Run `java -cp java/bin test.LogicCraftTest`. | `java/src/test/LogicCraftTest.java` |
| **Problem Solving (Java)** | **CO2** | Task 2 | Inheritance, Polymorphism, Interfaces & Multi-Stage Circuit Simulation | `java/src/logiccraft/gates/*`<br>`java/src/logiccraft/features/*` | `ANDGate`, `ORGate`, `NOTGate`, `CircuitValidatable`, `CircuitPrintable`, `CircuitEvaluator` | Run `java -cp java/bin test.InheritanceTest` and `test.MultiStageCircuitTest`. | `java/src/test/InheritanceTest.java` |
| **Problem Solving (Java)** | **CO3** | Task 3 | Exception Handling, Collections, Swing Desktop GUI & Package Hierarchy | `java/src/logiccraft/exceptions/InvalidInputException.java`<br>`java/src/logiccraft/collections/ComponentList.java`<br>`java/src/logiccraft/ui/LogicCraftUI.java` | `InvalidInputException`, `ComponentList<T>`, `LogicCraftUI`, `InputValidator` | Run `java -cp java/bin test.ExceptionTest` and `run_java.bat --gui`. | `java/src/test/ExceptionTest.java` |
| **Problem Solving (Java)** | **CO4** | Task 4 | File Handling (Save/Load/Corrupted Safety) & Java Generics Reusable Architecture | `java/src/logiccraft/file/ProjectFileManager.java`<br>`java/src/logiccraft/collections/ComponentList.java` | `saveCircuitToFile()`, `loadCircuitFromFile()`, `ComponentList<T>::filter()`, `ComponentList<T>::remove()`, `ComponentList<T>::clear()` | Run `java -cp java/bin test.Task4FileGenericTest`. In Swing GUI: Click "Save Circuit" and "Load Circuit". | `java/src/test/Task4FileGenericTest.java` |

---

## 2. Subject-by-Subject Task 4 Implementation Breakdown

### A. Programming Lab (C++) — CO4 (Searching & Sorting) and CO5 (Hashing)
> **Academic Syllabus Constraint:** Strict procedural C++ only. No classes, inheritance, polymorphism, encapsulation, or OOP design patterns are used. All logic uses C structs, pointers, arrays, and functions.

1. **Source Files:**
   - `Cpp/Searching/Searching.h` and `Cpp/Searching/Searching.cpp`
   - `Cpp/Sorting/Sorting.h` and `Cpp/Sorting/Sorting.cpp`
   - `Cpp/Hashing/HashTable.h` and `Cpp/Hashing/HashTable.cpp`
   - `Cpp/Algorithms/PerformanceComparison.h` and `Cpp/Algorithms/PerformanceComparison.cpp`
   - `Cpp/Tests/Task4Tests.h` and `Cpp/Tests/Task4Tests.cpp`
2. **Key Functions & Structs:**
   - `linearSearchById(const CircuitComponent* arr, int size, int targetId, int* outComparisons)`: Scans linearly, handles not-found, counts comparisons.
   - `binarySearchById(const CircuitComponent* arr, int size, int targetId, int* outComparisons)`: Binary search on pre-sorted array, returns index, tracks comparisons.
   - `bubbleSortComponents(CircuitComponent* arr, int size, SortField field, SortOrder order, int* outComparisons, int* outSwaps)`: Bubble sort with early termination flag.
   - `selectionSortComponents(CircuitComponent* arr, int size, SortField field, SortOrder order, int* outComparisons, int* outSwaps)`: In-place selection sort minimizing swap count ($O(N)$ writes).
   - `struct ComponentHashTable`: Fixed prime modulus 17 array with `HashSlot slots[17]`, `size`, `capacity`.
   - `hashString(const char* str, int capacity)`: Polynomial rolling hash algorithm: $H = \left(\sum c_i \cdot 31^i\right) \pmod{17}$.
   - `insertHashTable()`, `searchHashTableById()`, `deleteHashTableById()`: Linear probing collision resolution with tombstone reuse and full-table guard.

### B. Problem Solving Using C++ — CO4 (Smart Circuit Validation & Decision Engine)
1. **Source Files:**
   - `Cpp/CircuitEngine/CircuitValidator.h` and `Cpp/CircuitEngine/CircuitValidator.cpp`
2. **Key Functions:**
   - `validateCircuit(const Circuit* circuit, ValidationReport* report)`: Evaluates circuit completeness, connectivity, and acyclic properties.
   - `hasCyclicDependency(const Circuit* circuit, int* outCycleStart, int* outCycleEnd)`: Depth-First Search (DFS) three-color algorithm (White/Gray/Black) to detect feedback cycles and self-loops.
   - `runCircuitValidationScenarios()`: Tests 3 formal academic scenarios:
     1. Scenario 1 (Valid): 3-gate cascade (AND -> OR -> NOT), all pins bound, verdict = `VALID`.
     2. Scenario 2 (Invalid): Disconnected gates with floating dangling input pins, verdict = `WARNING` / `INVALID`.
     3. Scenario 3 (Cyclic Loop): 2-gate circular feedback loop (G1 -> G2 -> G1), verdict = `INVALID`.

### C. Computer Organization and Architecture Lab — CO4 (Integration & Processor Mapping)
1. **Source Files:**
   - `COA/ALP/logic_ops_64.s`
   - `COA/Tests/Task4IntegrationTests.cpp`
   - `COA/main.cpp`
2. **Key Functions:**
   - `testCOAApplicationProcessorMapping()`: Maps application gate types directly to 64-bit hardware instructions (`AND`, `OR`, `NOT`, `XOR`, `NAND`, `NOR`, `XNOR`).
   - `asm_branch_verify_64(uint64_t a, uint64_t b, uint64_t op)`: Demonstrates processor status flags (`ZF`, `SF`) and conditional branches (`je`, `jne`).
   - `asm_mul_successive_addition_64()`: Computes multi-stage gate propagation delay without hardware multipliers.
   - `asm_compute_truth_table_rows_64()`: Computes $2^N$ truth table rows via hardware registers.
   - `asm_parse_gate_type_64()`: Hardware-level ASCII string parser with boundary error handling.

### D. Computer Graphics Lab — CO4 (Authentic Logic Gate Symbols & Precision Alignment)
1. **Source Files:**
   - `Computer Graphics/Rendering/GateRenderer.h` and `Computer Graphics/Rendering/GateRenderer.cpp`
   - `Computer Graphics/Canvas/Canvas.h` and `Computer Graphics/Canvas/Canvas.cpp`
   - `Computer Graphics/OpenGL/GLWindow.cpp`
   - `Computer Graphics/main.cpp`
2. **Key Functions & Methods:**
   - `GateRenderer::drawANDGate()`: Renders standard D-shaped body (flat input side, semi-circular curved output side, `GL_QUADS` + `GL_TRIANGLE_FAN` + `GL_LINE_STRIP`).
   - `GateRenderer::drawNANDGate()`: D-shaped body with circular inversion bubble at output apex (`drawInversionBubble`).
   - `GateRenderer::drawORGate()`: Concave curved input back arc with two sweeping convex curves meeting at the right apex (`GL_POLYGON` + `GL_LINE_STRIP`).
   - `GateRenderer::drawNORGate()`: OR-shaped body with circular inversion bubble at output apex.
   - `GateRenderer::drawNOTGate()`: Triangular inverter body with circular inversion bubble at output apex (`GL_TRIANGLES` + `GL_LINE_LOOP`).
   - `GateRenderer::drawXORGate()`: OR-shaped body with an additional detached curved arc on the input side.
   - `GateRenderer::drawXNORGate()`: XOR-shaped body with detached input arc and circular inversion bubble at output apex.
   - `GateRenderer::drawGate()`: Master dispatcher directing type strings to their respective ANSI/IEEE standard drawing routines.
   - `GateRenderer::drawInversionBubble()`: Renders dark-filled circular bubble with crisp outline matching the gate outline color.
   - `GateRenderer::getGatePinPosition()`: Calculates exact sub-pixel terminal coordinates so wire endpoints seamlessly snap to pin centers within $\pm 0.001$ units.
   - `Canvas::initAll7GatesShowcase()`: Arranges all 7 standard logic gate symbols simultaneously in a 2-row layout (triggered by key `[7]`).
   - `Canvas::cycleSelectedGateType()`: Dynamically morphs the selected gate's symbol between all 7 types (triggered by key `[G]`).
   - `Canvas::renderHUD()`: On-screen dashboard displaying camera parameters, active gate count, and updated controls.

### E. Problem Solving Using OOP (Java) — CO4 (File Handling & Java Generics)
1. **Source Files:**
   - `java/src/logiccraft/file/ProjectFileManager.java`
   - `java/src/logiccraft/collections/ComponentList.java`
   - `java/src/logiccraft/ui/LogicCraftUI.java`
   - `java/src/test/Task4FileGenericTest.java`
2. **Key Functions:**
   - `saveCircuitToFile(Circuit circuit, String filePath)`: Serializes circuit gates, connections, and properties into structured plain text.
   - `loadCircuitFromFile(String filePath)`: Deserializes circuit file, reinstantiating gate polymorphism via `GateFactory`.
   - Missing and corrupted file safety: Throws and catches `FileNotFoundException` and `IOException` with friendly alerts.
   - `ComponentList<T>`: Reusable generic container with `filter(Predicate<T>)`, `remove(int)`, `set(int, T)`, `clear()`, and `Iterable<T>`.
   - `LogicCraftUI`: Swing GUI toolbar buttons: "Save Circuit", "Load Circuit", "Export Report", "Filter (Generics)", "Clear Circuit".

---

## 3. How to Demonstrate in a Viva Examination

1. **Demonstrating C++ (Procedural CO4 & CO5):**
   ```powershell
   cd Cpp
   .\Tests\test_runner.exe
   ```
   - Test 6 demonstrates Linear Search vs Binary Search with comparison count tracking and element-not-found handling.
   - Test 7 demonstrates Bubble Sort vs Selection Sort with comparison and swap metrics.
   - Test 8 demonstrates Prime Modulus 17 Hash Table with Linear Probing collision resolution and tombstone deletion.
   - Test 9 executes the Empirical Benchmark comparison across $N=8, 16, 32$ datasets.
   - Test 10 executes the Smart Circuit Validation Engine across 3 distinct test scenarios.

2. **Demonstrating COA (CO4 Assembly Integration):**
   ```powershell
   cd COA
   .\Tests\test_alp.exe
   ```
   - Runs full Task 1 to Task 4 integration suite verifying register mapping, flags, delay repeated addition, and opcode decoding.

3. **Demonstrating Computer Graphics (CO4 Coordinate Precision):**
   ```powershell
   cd "Computer Graphics"
   .\logiccraft_cg.exe --test
   ```
   - Test 8 validates pin position precision, ensuring terminal offsets match gate dimensions within $\pm 0.001$ units.

4. **Demonstrating Java (CO4 File Handling & Generics):**
   ```powershell
   cd java
   java -cp bin test.Task4FileGenericTest
   ```
   - Verifies file persistence, corrupted file safety, and generic `ComponentList<T>` operations.
   - For graphical interface: `java -cp bin logiccraft.Main --gui`

5. **Demonstrating Full-Stack Web Application:**
   ```powershell
   cd backend
   node server.js
   ```
   - Open browser to `http://localhost:3000`. Navigate to **"Task 4 (Search, Sort, Hash, Validate)"** tab to interactively test searching, sorting, hash table linear probing, performance benchmark tables, circuit validation scenarios, and file persistence.
