# LogicCraft — Task 4 Course Outcomes (CO) Integration Guide

**Document Purpose:** Authoritative directory and reference manual detailing the exact source files, line ranges, symbols, and cross-module integration points for **Task 4** across all four academic subjects.

---

## 1. Quick Reference: Subject, CO, and File Location Matrix

| Subject | Course Outcome | Focus Area / Concept | Primary Source File(s) | Line Range | Key Functions / Methods | Test & Verification File |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Programming Lab (C++)** | **CO4** | **Searching & Sorting** (Linear/Binary Search, Bubble/Selection Sort) | `Cpp/Searching/Searching.cpp`<br>`Cpp/Sorting/Sorting.cpp`<br>`Cpp/Algorithms/PerformanceComparison.cpp` | `Searching.cpp: L1-L90`<br>`Sorting.cpp: L1-L120`<br>`PerformanceComparison.cpp: L1-L180` | `linearSearchById()`, `binarySearchById()`, `bubbleSortComponents()`, `selectionSortComponents()`, `runComprehensiveBenchmark()` | `Cpp/Tests/Task4Tests.cpp`<br>`Cpp/Tests/TestRunner.cpp` |
| **Programming Lab (C++)** | **CO5** | **Hashing with Linear Probing** (Prime Modulus 17, Tombstones, Collisions) | `Cpp/Hashing/HashTable.cpp`<br>`Cpp/Hashing/HashTable.h` | `HashTable.cpp: L1-L243`<br>`HashTable.h: L1-L85` | `initHashTable()`, `hashFunctionId()`, `insertHashTable()`, `searchHashTableById()`, `deleteHashTableById()`, `printHashTable()` | `Cpp/Tests/Task4Tests.cpp` |
| **Problem Solving (C++)** | **CO4** | **Smart Circuit Validation** (DFS 3-Color Cycle Detection, Pin Verification) | `Cpp/CircuitEngine/CircuitValidator.cpp`<br>`Cpp/CircuitEngine/CircuitValidator.h` | `CircuitValidator.cpp: L1-L220` | `validateCircuit()`, `hasCyclicDependency()`, `runCircuitValidationScenarios()`, `printValidationReport()` | `Cpp/Tests/Task4Tests.cpp` |
| **COA Lab** | **CO4** | **Assembly Integration & Processor Mapping** (AMD64 Opcode Mapping, Status Flags, Delay) | `COA/Tests/Task4IntegrationTests.cpp`<br>`COA/ALP/logic_ops_64.s`<br>`COA/main.cpp` | `Task4IntegrationTests.cpp: L1-L160`<br>`logic_ops_64.s: L1-L135` | `asm_branch_verify_64()`, `testCOAApplicationProcessorMapping()`, `testArithmeticSuccessiveAdditionDelay()`, `testProcessorFlagsAndBranching()` | `COA/Tests/TestALP.cpp` |
| **Computer Graphics Lab** | **CO4** | **ANSI/IEEE Gate Symbols & Precision Pin Alignment** (All 7 Standard Symbols, Wire Snapping) | `Computer Graphics/Rendering/GateRenderer.cpp`<br>`Computer Graphics/Canvas/Canvas.cpp` | `GateRenderer.cpp: L180-L620`<br>`Canvas.cpp: L150-L240` | `drawGate()`, `drawANDGate()`, `drawORGate()`, `drawNOTGate()`, `drawNANDGate()`, `drawNORGate()`, `drawXORGate()`, `drawXNORGate()`, `getGatePinPosition()` | `Computer Graphics/main.cpp` (`--test`) |
| **Problem Solving (Java)** | **CO4** | **File Persistence & Java Generics** (Save/Load `.circ`, Generic `ComponentList<T>`) | `java/src/logiccraft/file/ProjectFileManager.java`<br>`java/src/logiccraft/collections/ComponentList.java` | `ProjectFileManager.java: L1-L191`<br>`ComponentList.java: L1-L80` | `saveCircuitToFile()`, `loadCircuitFromFile()`, `ComponentList<T>::filter()`, `ComponentList<T>::remove()`, `ComponentList<T>::clear()` | `java/src/test/Task4FileGenericTest.java` |

---

## 2. In-Depth Subject Integration Details

### A. Programming Lab (C++) — CO4 & CO5
> **Academic Constraint:** Strict procedural C++ only. No classes, inheritance, polymorphism, or OOP patterns are used. Implemented purely with structs, arrays, pointers, and functions.

1. **Searching (`Cpp/Searching/Searching.cpp`):**
   - **Linear Search (`linearSearchById`, `linearSearchByName`)**: Iterates sequentially through component arrays. Handles element-not-found cleanly and tracks comparison count ($O(N)$ worst-case).
   - **Binary Search (`binarySearchById`, `binarySearchByName`)**: Validates that the array is sorted using `isSortedById()`, then divides the search range logarithmically ($O(\log_2 N)$ worst-case).
2. **Sorting (`Cpp/Sorting/Sorting.cpp`):**
   - **Bubble Sort (`bubbleSortComponents`)**: Implements standard adjacent comparisons with an early-termination `swapped` flag to optimize best-case passes.
   - **Selection Sort (`selectionSortComponents`)**: Scans for minimum/maximum elements and performs at most $N-1$ swaps, minimizing memory write cycles.
   - Supports sorting by component **ID**, **Name**, or **Gate Type** in both ascending and descending order.
3. **Hashing with Linear Probing (`Cpp/Hashing/HashTable.cpp`):**
   - **Hash Table Struct (`ComponentHashTable`)**: Fixed array of 17 slots (prime capacity `HASH_TABLE_CAPACITY = 17`).
   - **Hash Functions**: Modular hash for numeric IDs (`id % 17`) and polynomial rolling hash for string names ($\sum c_i \cdot 31^i \pmod{17}$).
   - **Collision Resolution**: Open addressing via linear probing: $h(k, i) = (\text{hash}(k) + i) \pmod{17}$.
   - **State Tracking**: Uses `SLOT_EMPTY`, `SLOT_OCCUPIED`, and `SLOT_DELETED` (tombstone) to support efficient deletions and slot recycling. Handles full-table condition gracefully.
4. **Performance Benchmarking (`Cpp/Algorithms/PerformanceComparison.cpp`):**
   - Compares Linear vs. Binary search and Bubble vs. Selection sort across dataset sizes $N = 8, 16, 32$.
   - Measures comparison counts, swap counts, and execution times.
5. **Smart Circuit Validation (`Cpp/CircuitEngine/CircuitValidator.cpp`):**
   - Evaluates circuit graph topology, detects floating inputs, and uses a 3-color DFS algorithm to detect feedback loops/cycles. Issues actionable recommendations across 3 test scenarios.

---

### B. Computer Organization & Architecture Lab (COA) — CO4
> **Integration Goal:** Map application-level logic gate evaluations directly to 64-bit AMD64 hardware processor instructions, registers, and status flags.

1. **Assembly Operations (`COA/ALP/logic_ops_64.s`):**
   - Written in 64-bit GNU Assembly conforming to the Microsoft x64 ABI (registers `RCX`, `RDX`, `R8`, `R9` for arguments; return value in `RAX`).
   - Bitwise operations: `asm_and_64` (`AND`), `asm_or_64` (`OR`), `asm_not_64` (`NOT`), `asm_xor_64` (`XOR`), `asm_nand_64`, `asm_nor_64`, `asm_xnor_64`.
   - Successive addition delay: `asm_mul_successive_addition_64` uses loop-based addition (`ADD`, `SUB`, `JNZ`) to compute multi-stage propagation delay without hardware multipliers.
   - Branch verification: `asm_branch_verify_64` checks Zero Flag (`ZF`) and Sign Flag (`SF`) using `CMP` and conditional jumps (`JE`, `JNE`).
2. **Integration Verification (`COA/Tests/Task4IntegrationTests.cpp`):**
   - `testCOAApplicationProcessorMapping()`: Passes application gate objects into assembly routines and asserts bit-exact output.
   - `testArithmeticSuccessiveAdditionDelay()`: Simulates 5-stage propagation delay (5 stages $\times$ 5 ns = 25 ns) via native assembly loops.
   - `testProcessorFlagsAndBranching()`: Validates processor status flags under matching and non-matching logic conditions.

---

### C. Computer Graphics Lab (OpenGL) — CO4
> **Integration Goal:** Replace placeholder rectangular boxes with authentic ANSI/IEEE Std 91-1984 logic gate shapes, while preserving sub-pixel pin alignment and wire connectivity.

1. **Gate Symbol Renderer (`Computer Graphics/Rendering/GateRenderer.cpp`):**
   - **AND Gate (`drawANDGate`)**: Flat input side, semi-circular curved output side constructed with `GL_QUADS`, `GL_TRIANGLE_FAN`, and `GL_LINE_STRIP`.
   - **OR Gate (`drawORGate`)**: Inward concave back arc and pointed dual-convex curves meeting at output apex using `GL_POLYGON` and `GL_LINE_STRIP`.
   - **NOT Gate (`drawNOTGate`)**: Triangular body (`GL_TRIANGLES`) with output circular inversion bubble (`drawInversionBubble`).
   - **NAND Gate (`drawNANDGate`)**: D-shape with output inversion bubble.
   - **NOR Gate (`drawNORGate`)**: OR-shape with output inversion bubble.
   - **XOR Gate (`drawXORGate`)**: OR-shape with an additional detached curved arc on the input side.
   - **XNOR Gate (`drawXNORGate`)**: XOR-shape with detached input arc and output inversion bubble.
2. **Sub-Pixel Pin Alignment (`getGatePinPosition`):**
   - Mathematically calculates input pins ($A, B$) and output pin ($Y$) coordinates based on gate dimensions.
   - Wire endpoints snap exactly to terminal centers within $\pm 0.001$ coordinate precision.
3. **Interactive Showcase (`Computer Graphics/Canvas/Canvas.cpp`):**
   - Key `[7]`: Activates `initAll7GatesShowcase()`, rendering all 7 gate symbols side-by-side.
   - Key `[G]`: Cycles the selected gate's type dynamically through all 7 shapes in real-time.

---

### D. Problem Solving Using OOP (Java) — CO4
> **Integration Goal:** Implement safe file persistence for circuit diagrams and develop a generic container architecture.

1. **File Persistence (`java/src/logiccraft/file/ProjectFileManager.java`):**
   - **File Serialization (`saveCircuitToFile`)**: Serializes complete circuit topologies into structured `.circ` files containing gate IDs, types, input counts, default states, and wire connections.
   - **File Deserialization (`loadCircuitFromFile`)**: Parses `.circ` files and reconstructs the circuit dynamically using the polymorphic `GateFactory`.
   - **Error Handling**: Throws and handles `FileNotFoundException` (missing files), `IOException` (read/write failures), and format corruption errors.
2. **Reusable Generic Container (`java/src/logiccraft/collections/ComponentList.java`):**
   - Generic class `ComponentList<T>` supporting type-safe component collections.
   - Functional predicate filtering: `ComponentList<T> filter(Predicate<T> predicate)`.
   - Supports generic methods: `add()`, `get()`, `remove()`, `set()`, `clear()`, `size()`, and `iterator()`.
3. **Desktop Swing GUI (`java/src/logiccraft/ui/LogicCraftUI.java`):**
   - Integrated toolbar buttons: "Save Circuit" and "Load Circuit" using standard `JFileChooser` dialogs, along with a "Filter (Generics)" button.

---

### E. Full-Stack Web Integration
> **Integration Goal:** Connect all subject features to the live web application so examiner demonstrations can be conducted from a single interactive browser window.

1. **Backend REST API (`backend/server.js`):**
   - `POST /api/search` (lines ~605-690): Executes linear and binary search on circuit components.
   - `POST /api/sort` (lines ~695-780): Executes bubble and selection sort on circuit components.
   - `GET & POST /api/hashtable` (lines ~785-845): Manages the in-memory hash table, handles insertions, and triggers collision demos.
   - `GET & POST /api/benchmark` (lines ~850-880): Generates search and sort performance comparisons across $N = 8, 16, 32$.
   - `POST /api/validate` (lines ~885-970): Runs circuit graph validation and cycle detection.
   - `GET & POST /api/circuit/export` (lines ~975-1010): Serializes circuits into `.circ` files matching Java's `ProjectFileManager` format.
   - Native test spawners: `/api/execute/cpp`, `/api/execute/coa`, `/api/execute/graphics`, `/api/execute/java-task4`.
2. **Frontend Dashboard (`frontend/app.js` & `frontend/index.html`):**
   - **Task 4 Tab**:
     - Interactive Search card (Linear vs. Binary search with comparison counter).
     - Interactive Sort card (Bubble vs. Selection sort by ID, Name, or Type).
     - 17-Bucket Hash Table Grid with dynamic linear probing and collision resolution demonstration log.
     - Empirical Benchmark Table (N=8, 16, 32).
     - Smart Circuit Validation engine with Scenarios 1, 2, 3.
     - Circuit File Export & Generic Filter cards.
   - **Circuit Canvas**:
     - Renders authentic ANSI/IEEE vector shapes for all 7 logic gates matching the OpenGL implementation.

---

## 3. How to Demonstrate Task 4 in an Examination / Viva

### Option 1: Master Demonstration Script
Run the master PowerShell harness to execute all test suites sequentially:
```powershell
.\run_all.ps1
```

### Option 2: Subject-by-Subject Terminal Execution

1. **C++ (Searching, Sorting, Hashing, Benchmarking, Validation):**
   ```powershell
   cd Cpp
   $env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\mingw64\bin;" + $env:PATH
   .\Tests\test_runner.exe
   ```
   *Verifies 6 test suites covering CO1 to CO5.*

2. **COA (64-Bit Assembly Hardware Integration):**
   ```powershell
   cd COA
   $env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\mingw64\bin;" + $env:PATH
   .\Tests\test_alp.exe
   ```
   *Executes Task 1–4 integration tests, opcode mapping, status flags, and successive addition delay.*

3. **Computer Graphics (OpenGL ANSI/IEEE Gate Shapes & Precision):**
   ```powershell
   cd "Computer Graphics"
   $env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\mingw64\bin;" + $env:PATH
   .\logiccraft_cg.exe --test
   ```
   *For interactive 3D/2D OpenGL window: run `.\run_graphics.bat`, press `[7]` for 7-gate showcase or `[G]` to cycle gate shapes.*

4. **Java (File Persistence & Generics):**
   ```powershell
   cd java
   java -cp bin test.Task4FileGenericTest
   ```
   *For desktop Swing GUI: run `java -cp bin logiccraft.Main --gui`.*

### Option 3: Full-Stack Web Application (Interactive Browser UI)
1. Start the server:
   ```powershell
   node backend/server.js
   ```
2. Open **http://localhost:3000** in any browser.
3. Click on the **"Task 4 (Search, Sort, Hash, Validate)"** tab at the top to access all interactive demonstrations.
