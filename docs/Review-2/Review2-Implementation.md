# LogicCraft — Visual Logic Gate Simulator
## Review-2 Implementation & Course Outcome (CO) Mapping Document

---

## 1. Project Objective

**LogicCraft** is an interdisciplinary visual logic-circuit simulator where users can create digital circuits using logic gates, connect gates using signal wires, simulate Boolean logic, and inspect operations from high-level visual representations down to processor hardware execution.

The project unites four academic computer science disciplines into one integrated architecture:
1. **Programming Lab (C++)** — Core data structures, circuit component storage, and truth-table evaluation.
2. **Computer Graphics (OpenGL)** — 2D circuit visual canvas, hardware-accelerated gate symbol rendering, interactive transformations, and viewport camera.
3. **Problem Solving Using OOP (Java)** — Object-oriented domain model, polymorphic gate hierarchy, lambda expressions, and textual reporting engine.
4. **Computer Organization & Architecture / ALP (64-Bit Assembly)** — Processor-level RTL execution trace (Registers, ALU, Flags) and native x86-64 assembly instructions (`AND`, `OR`, `CMP`, conditional branching).

---

## 2. Review 2 Scope & Compliance Policy

In strict adherence to academic review guidelines:
- **Review 2 implements ONLY CO1 and CO2 for each subject.**
- Higher Course Outcomes (CO3, CO4, CO5, CO6) are explicitly kept out of scope, with modular extension interfaces provided for subsequent reviews.
- **Single Master Executable (`LogicCraft.exe`):** All four subjects are compiled and linked together into a unified standalone executable file that provides an interactive review menu, launches the native OpenGL graphical window, and coordinates the entire 12-step demonstration flow.

---

## 3. Official Subject & CO-Mapping Table

| Subject | CO1 | CO2 | Review 2 Evidence |
| :--- | :--- | :--- | :--- |
| **C++ (Programming Lab)** | **Arrays** | **Linked List** | Array-based CRUD operations & truth tables + Dynamic heap component linked list |
| **Computer Graphics (OpenGL)** | **OpenGL Primitives** | **Transformation & Clipping** | Hardware rendering of AND, OR, NOT, wires, pins + Translation (drag), Scaling (zoom), Viewport clipping |
| **Java (OOP)** | **Fundamentals** | **Advanced OOP** | Encapsulated gate classes, arrays, loops, `StringBuilder` reports + Polymorphic gate hierarchy & Lambda expressions |
| **COA / ALP** | **Processor Modeling** | **64-Bit Assembly** | Register-transfer level simulation (R1, R2, R3, ALU, Flags) + Native x86-64 assembly routines (`AND`, `OR`, `CMP`, `JE`/`JNE`) |

---

## 4. Detailed Subject Implementation Breakdown

### 4.1. C++ / Programming Lab
* **CO1 — Arrays:**
  * **Array Storage:** Boolean inputs and gate records stored using contiguous fixed arrays (`inputs[MAX_INPUTS]`, `gates[MAX_CIRCUIT_GATES]`).
  * **Array CRUD Operations:** Implemented `insertGate()`, `deleteGate()` (with contiguous left-shifting), `updateGate()`, `searchGateById()`, `searchGateByName()`, and `displayAll()`.
  * **Truth Table Generation:** Computes all $2^N$ binary input permutations into a 2D array, applies each permutation to the gate, and stores the evaluated output array.
* **CO2 — Linked List:**
  * **Dynamic Circuit Node Structure:** Implemented `GateNode` (`gateId`, `gateType`, `gateName`, `inputCount`, `inputs[]`, `outputValue`, `next`).
  * **Dynamic Operations:** `insertNode()`, `deleteNode()`, `updateNode()`, `searchNode()`, `traverseAndDisplay()`.
  * **Academic Justification:** In visual EDA (Electronic Design Automation) tools like LogicCraft, users dynamically place and delete gates on the canvas at runtime without fixed memory boundaries; dynamic singly-linked lists offer heap allocation and $O(1)$ pointer-based splicing.
* **Out of Scope (CO3-CO5):** Stack/Queue for topological sorting (CO3) and Hash Tables for timing lookup (CO4) reserved for Review 3.

### 4.2. Computer Graphics / OpenGL
* **CO1 — OpenGL Primitives:**
  * **Canvas & Grid:** Rendered using `GL_LINES` with major coordinate axes and minor engineering grid lines.
  * **AND Gate:** Constructed via `GL_QUADS` (rectangular body) and `GL_TRIANGLE_FAN` / `GL_LINE_STRIP` (semi-circular curved output face).
  * **OR Gate:** Constructed with a concave input curve and twin sweeping curved arcs converging to a sharp apex.
  * **NOT Gate:** Constructed with a `GL_TRIANGLES` body and `GL_LINE_LOOP` inversion circle bubble.
  * **Terminals & Wires:** Circular pin pads (`GL_LINE_LOOP` / `GL_TRIANGLE_FAN`) and orthogonal stepped wire connections (`GL_LINE_STRIP`).
  * **Typography:** Self-contained vector-stroke font drawn with `GL_LINES`.
* **CO2 — Transformation & Clipping:**
  * **Translation:** Gate dragging and camera panning via `glTranslatef()`.
  * **Scaling:** Smooth canvas zooming via `glScalef()` and orthographic frustum resizing.
  * **Coordinate Conversion:** Screen pixel coordinates mapped to World Space via inverse viewport projection (`screenToWorld`).
  * **Viewport & Clipping:** Handled window resizing with `glViewport()` and implemented Axis-Aligned Bounding Box (AABB) culling against view frustum bounds.
* **Out of Scope (CO3-CO6):** Shaders, 3D lighting, texture mapping, and graph viewports reserved for Reviews 3-4.

### 4.3. Java / Problem Solving Using OOP
* **CO1 — Java Fundamentals:**
  * Created foundational classes: `LogicGate`, `ANDGate`, `ORGate`, `NOTGate`, `Circuit`, `Connection`, `Project`, and `ReportGenerator`.
  * Encapsulation with private/protected fields and strict getters/setters.
  * State formatting and textual audit reporting implemented via `StringBuilder`.
* **CO2 — Advanced OOP:**
  * **Abstract Base Class:** `abstract class LogicGate` defining abstract methods `evaluate()` and `getGateType()`.
  * **Inheritance & Subclasses:** Concrete classes `ANDGate`, `ORGate`, `NOTGate`, `XORGate`, `NANDGate`, `NORGate`.
  * **Runtime Polymorphism:** Generic `LogicGate` references dynamically bound to different subclass instances to execute overridden `evaluate()` logic.
  * **Keywords:** Demonstrated `static final` constants (`MAX_INPUTS`), `static` instance counters, and `final` getters (`getId()`).
  * **Functional Interface & Lambda Expressions:** Created `@FunctionalInterface GateEvaluator` demonstrating custom lambda logic (Majority Vote Gate) and circuit filtering (`circuit.filterGates(g -> g.getOutput() == true)`).
* **Out of Scope (CO3-CO4):** Complex Swing UI, file persistence, and serialization reserved for Review 3.

### 4.4. COA / 64-Bit Assembly (ALP)
* **CO1 — Processor-Level Modeling:**
  * Boolean values modeled as binary hardware states ($0$ and $1$).
  * Modeled architectural CPU components: General Purpose Registers (`R1`, `R2`, `R3`), Program Counter (`PC`), Instruction Register (`IR`), and Flags Register (`ZF`, `SF`, `CF`).
  * Annotated Register-Transfer Level (RTL) trace of micro-operations for logic gate execution.
* **CO2 — 64-Bit Assembly (ALP):**
  * Implemented pure x86-64 assembly in `COA/ALP/logic_ops_64.s` using the Microsoft x64 calling convention.
  * 64-bit Registers used: `RAX`, `RCX`, `RDX`, `R8`, `R9`.
  * Instructions: `mov`, `and`, `or`, `xor`, `cmp`.
  * Conditional branching: Implemented `cmp` and conditional jump `jne` / `je` routines to verify gate output equivalence.
* **Out of Scope (CO3-CO4):** ASCII string processing and microcode control unit reserved for Review 3.

---

## 5. Repository File Structure

```
LogicCraft/
│
├── README.md                           # Main Project & Viva Documentation
├── run_cpp.bat                         # C++ Compile & Run Batch Script
├── run_graphics.bat                    # OpenGL Visual Canvas Launch Script
├── run_java.bat                        # Java Compile & Run Batch Script
├── run_coa.bat                         # COA / Assembly Compile & Run Script
├── run_all.ps1                         # Master Automated 12-Step Review Demo Script
│
├── Cpp/                                # PART 1: Programming Lab (C++)
│   ├── Gates/
│   │   ├── Gate.h                      # Gate Class Header
│   │   └── Gate.cpp                    # Gate Class Implementation
│   ├── DataStructures/
│   │   ├── ArrayOperations.h           # CO1: Array-Based Component Registry Header
│   │   ├── ArrayOperations.cpp         # CO1: Array Insert/Delete/Update/Search Implementation
│   │   ├── LinkedList.h                # CO2: Dynamic Singly-Linked List Header
│   │   └── LinkedList.cpp              # CO2: Dynamic Heap Node Management
│   ├── CircuitEngine/
│   │   ├── TruthTable.h                # CO1: Array Truth Table Generator Header
│   │   ├── TruthTable.cpp              # CO1: Truth Table Generation Implementation
│   │   ├── Circuit.h                   # Array Circuit Wiring Engine
│   │   └── Circuit.cpp                 # Signal Propagation Implementation
│   ├── Algorithms/
│   │   └── AlgorithmsPlaceholder.h     # Documented CO3/CO4 Future Extension Points
│   ├── Tests/
│   │   └── TestRunner.cpp              # Automated Regression Unit Test Suite
│   └── main.cpp                        # C++ Review-2 Demonstration Entry Point
│
├── Computer Graphics/                  # PART 2: Computer Graphics (OpenGL)
│   ├── OpenGL/
│   │   ├── GLWindow.h                  # Native Win32 OpenGL Window Header
│   │   └── GLWindow.cpp                # Context Creation & Message Loop
│   ├── Rendering/
│   │   ├── GateRenderer.h              # CO1: Primitives Gate Renderer Header
│   │   └── GateRenderer.cpp            # AND, OR, NOT, Wire & Pin Rendering
│   ├── Canvas/
│   │   ├── Canvas.h                    # Circuit Canvas & Component Storage Header
│   │   └── Canvas.cpp                  # Grid, Wires, Gate Dragging & Evaluation
│   ├── Transformations/
│   │   ├── Camera.h                    # CO2: Camera, Pan, Zoom & Viewport Header
│   │   └── Camera.cpp                  # Matrix Transformations & Frustum Clipping
│   └── main.cpp                        # OpenGL Application Entry Point
│
├── java/                               # PART 3: Problem Solving Using OOP (Java)
│   ├── gates/
│   │   ├── LogicGate.java              # CO2: Abstract Base Gate Class
│   │   ├── ANDGate.java                # Concrete AND Gate Subclass
│   │   ├── ORGate.java                 # Concrete OR Gate Subclass
│   │   ├── NOTGate.java                # Concrete NOT Gate Subclass
│   │   ├── XORGate.java                # Concrete XOR Gate Subclass
│   │   ├── NANDGate.java               # Concrete NAND Gate Subclass
│   │   ├── NORGate.java                # Concrete NOR Gate Subclass
│   │   └── GateEvaluator.java          # CO2: Functional Interface for Lambdas
│   ├── circuit/
│   │   ├── Circuit.java                # CO1: Circuit Model (Array of Gates)
│   │   └── Connection.java             # Directed Inter-Gate Signal Wire
│   ├── project/
│   │   └── Project.java                # Project Domain Model & Metadata
│   ├── reports/
│   │   └── ReportGenerator.java        # CO1: StringBuilder Circuit Audit Reporter
│   ├── simulation/
│   │   └── SimulationEngine.java       # Polymorphic Signal Propagation Engine
│   └── Main.java                       # Java Review-2 Demonstration Entry Point
│
├── COA/                                # PART 4: Computer Organization & Architecture / ALP
│   ├── ALP/
│   │   └── logic_ops_64.s              # CO2: Genuine 64-Bit x86-64 Assembly Routines
│   ├── BooleanOperations/
│   │   ├── BooleanALU.h                # CO1: Arithmetic Logic Unit Header
│   │   └── BooleanALU.cpp              # Opcode Decoder & Flag Updater
│   ├── ProcessorSimulation/
│   │   ├── ProcessorSimulator.h        # CO1: CPU Registers, PC, IR & RTL Simulator
│   │   └── ProcessorSimulator.cpp      # Register Transfer Trace Implementation
│   ├── Tests/
│   │   └── TestALP.cpp                 # Verification Harness for Assembly & CPU Model
│   └── main.cpp                        # COA Review-2 Demonstration Entry Point
│
├── shared/
│   └── circuit_data.json               # Unified 3-Gate Reference Circuit Data
│
└── docs/
    ├── Architecture/                   # High-Level Cross-Subject Architectural Diagrams
    ├── CO-Mapping/                     # Course Outcome Syllabus Mapping
    └── Review-2/
        └── Review2-Implementation.md   # This Review-2 Specification Document
```

---

## 6. How to Run Each Module

### Prerequisites
* **Operating System:** Windows 10 / 11 (64-bit)
* **C++ & Assembly Toolchain:** MinGW-w64 GCC / G++ (supporting x86-64)
* **Java SDK:** Java Development Kit (JDK 17+)
* **OpenGL Libraries:** Standard Windows OpenGL (`-lopengl32 -lgdi32`) — pre-installed natively on Windows.

### Compilation and Execution Instructions

#### 1. C++ / Programming Lab
```powershell
cd "LogicCraft\Cpp"
g++ -std=c++14 main.cpp Gates/Gate.cpp DataStructures/ArrayOperations.cpp DataStructures/LinkedList.cpp CircuitEngine/TruthTable.cpp CircuitEngine/Circuit.cpp -o logiccraft_cpp.exe
.\logiccraft_cpp.exe
```
*Or simply execute:* `.\run_cpp.bat`

#### 2. Computer Graphics / OpenGL
```powershell
cd "LogicCraft\Computer Graphics"
g++ -std=c++14 main.cpp OpenGL/GLWindow.cpp Rendering/GateRenderer.cpp Canvas/Canvas.cpp Transformations/Camera.cpp -lopengl32 -lgdi32 -o logiccraft_cg.exe
.\logiccraft_cg.exe
```
*For automated non-interactive self-test verification:*
```powershell
.\logiccraft_cg.exe --test
```
*Or simply execute:* `.\run_graphics.bat`

#### 3. Java / Problem Solving Using OOP
```powershell
cd "LogicCraft\java"
if (-not (Test-Path "bin")) { New-Item -ItemType Directory -Path "bin" }
javac -d bin gates/*.java circuit/*.java project/*.java reports/*.java simulation/*.java Main.java
java -cp bin Main
```
*Or simply execute:* `.\run_java.bat`

#### 4. COA / 64-Bit Assembly (ALP)
```powershell
cd "LogicCraft\COA"
g++ -std=c++14 main.cpp ProcessorSimulation/ProcessorSimulator.cpp BooleanOperations/BooleanALU.cpp ALP/logic_ops_64.s -o logiccraft_coa.exe
.\logiccraft_coa.exe
```
*Or simply execute:* `.\run_coa.bat`

#### 5. Master Automated Suite (All 4 Modules in Sequence)
```powershell
cd "LogicCraft"
.\run_all.ps1
```

---

## 7. Cross-Subject Integration Architecture

All four subject modules simulate the exact same reference circuit defined in `shared/circuit_data.json`:
* **Gate G1:** `AND` Gate (Inputs $A = 1, B = 0 \implies \text{Output } G1 = 0$)
* **Gate G2:** `OR` Gate (Inputs $G1 = 0, C = 1 \implies \text{Output } G2 = 1$)
* **Gate G3:** `NOT` Gate (Input $G2 = 1 \implies \text{Output } G3 = 0$)

```
             ┌─────────┐
A (1) ───────┤   G1    │
             │   AND   ├────── (0) ──┐
B (0) ───────┤         │             │
             └─────────┘             │    ┌─────────┐
                                     ├───┤   G2    │
                                     │   │   OR    ├────── (1) ───┐
                             C (1) ──┘   │         │              │
                                         └─────────┘              │    ┌─────────┐
                                                                  └───┤   G3    │
                                                                      │   NOT   ├──── Final Out (0)
                                                                      └─────────┘
```

* **OpenGL** renders this topology with interactive panning, zooming, and dragging.
* **Java** structures it using encapsulated polymorphic objects and prints the audit report.
* **C++** maintains it in both contiguous arrays and dynamic linked-list nodes, evaluating truth tables.
* **COA / Assembly** steps through the register transfers (`R1`, `R2`, `R3`) and executes native 64-bit machine instructions (`and`, `or`, `not`).

---

## 8. Review-2 Demonstration Sequence (Step-by-Step Viva Guide)

1. **Step 1:** Open LogicCraft workspace.
2. **Step 2:** Launch OpenGL application (`run_graphics.bat`) and display coordinate grid.
3. **Step 3:** Show AND, OR, and NOT gates rendered using geometric OpenGL primitives.
4. **Step 4:** Left-click and drag a gate across the canvas to demonstrate **Translation (CO2)**.
5. **Step 5:** Use mouse wheel and right-click drag to demonstrate **Scaling and Viewport Panning (CO2)**.
6. **Step 6:** Launch Java demonstration (`run_java.bat`) to display the `LogicGate` class hierarchy.
7. **Step 7:** Demonstrate **Inheritance & Runtime Polymorphism (CO2)** through overridden `evaluate()` calls.
8. **Step 8:** Launch C++ demonstration (`run_cpp.bat`) to show **Array Operations & Truth Tables (CO1)**.
9. **Step 9:** Show C++ heap-allocated **Dynamic Linked List (CO2)** with insert, update, delete, and search.
10. **Step 10:** Execute end-to-end signal evaluation across the 3-gate circuit.
11. **Step 11:** Launch COA module (`run_coa.bat`) to show **Processor-Level Register Transfers (CO1)**.
12. **Step 12:** Execute native **64-Bit Assembly (CO2)** and verify branch flow (`JE`/`JNE`).

---

## 9. Extension Points for Review 3+

* **C++:**
  * CO3: Stack (canvas undo/redo history) & Queue (topological BFS/DFS evaluation order).
  * CO4: Hash Tables for $O(1)$ component lookup and timing propagation delays.
* **Computer Graphics:**
  * CO3: Textures and color-coded voltage shaders.
  * CO4: Wire routing algorithms and multi-viewports.
* **Java:**
  * CO3: Custom circuit exceptions and Swing graphical editor.
  * CO4: JSON / XML circuit file serialization and Generics.
* **COA / ALP:**
  * CO3: ASCII netlist string parsing and instruction decoding in assembly.
  * CO4: Microcode control unit and full multi-cycle datapath integration.
