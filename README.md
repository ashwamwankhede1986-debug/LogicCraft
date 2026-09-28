# LogicCraft — Visual Logic Gate Simulator
### Academic Interdisciplinary Project (Review-2 Prototype)

LogicCraft is an interdisciplinary visual logic-circuit simulator designed and implemented across four core computer science subjects. This repository contains the complete **Review-2 Prototype**, strictly focusing on **CO1 and CO2** for each subject.

---

## 🚀 Single Unified Executable: `LogicCraft.exe`

All four academic subject modules have been compiled and linked into **one single standalone executable**:
👉 **`LogicCraft.exe`**

Double-clicking `LogicCraft.exe` opens an interactive master console allowing you to:
1. Run **C++ / Programming Lab** (CO1 Arrays & CO2 Linked List)
2. Run **Computer Graphics / OpenGL** verification & launch the live visual window
3. Run **Java / Problem Solving Using OOP** (CO1 Fundamentals & CO2 Polymorphic Hierarchy / Lambdas)
4. Run **COA / 64-Bit Assembly (ALP)** (CO1 Processor Model & CO2 Native x86-64 Machine Code)
5. Execute the **Full 12-Step Review-2 Academic Demonstration**
6. Launch the **Interactive OpenGL Circuit Canvas Window** directly (drag gates, zoom, pan, toggle inputs)

### Command-Line Arguments:
You can also launch specific modules or automated checks directly:
* `.\LogicCraft.exe` (Interactive Master Menu)
* `.\LogicCraft.exe --all` (Runs full 12-step review sequence)
* `.\LogicCraft.exe --graphics` (Launches OpenGL interactive canvas directly)
* `.\LogicCraft.exe --cpp` (Runs C++ Lab demonstration)
* `.\LogicCraft.exe --java` (Runs Java OOP demonstration)
* `.\LogicCraft.exe --coa` (Runs COA / 64-bit Assembly demonstration)

---

## 🏛️ Academic Course Outcome (CO) Mapping

| Subject | CO1 | CO2 | Review-2 Demonstration Evidence |
| :--- | :--- | :--- | :--- |
| **1. C++ (Programming Lab)** | **Arrays** | **Linked List** | Array CRUD operations + Truth-table row generation + Dynamic heap linked list |
| **2. Computer Graphics (OpenGL)** | **OpenGL Primitives** | **Transformation & Clipping** | Hardware canvas rendering of AND/OR/NOT/wires/pins + Translation, Scaling, Viewport clipping |
| **3. Java (OOP)** | **Fundamentals** | **Advanced OOP** | Classes, objects, arrays, loops, `StringBuilder` reports + Polymorphic gate hierarchy & Lambda expressions |
| **4. COA / ALP** | **Processor Modeling** | **64-bit Assembly** | Register-Transfer Level (RTL) simulation (R1, R2, R3, ALU, Flags) + Genuine x86-64 assembly instructions & branching |

---

## 📁 Repository Structure

```
LogicCraft/
│
├── LogicCraft.exe                      # ⭐ ALL-IN-ONE UNIFIED STANDALONE EXECUTABLE
├── build_single_exe.bat                # Recompiles the single LogicCraft.exe from source
├── README.md                           # Main Project & Viva Documentation
├── libgcc_s_seh-1.dll                  # Portable runtime DLL
├── libstdc++-6.dll                     # Portable runtime DLL
├── libwinpthread-1.dll                 # Portable runtime DLL
│
├── Cpp/                                # PART 1: Programming Lab (C++)
│   ├── Gates/ (Gate.h, Gate.cpp)
│   ├── DataStructures/ (ArrayOperations.h/.cpp, LinkedList.h/.cpp)
│   ├── CircuitEngine/ (TruthTable.h/.cpp, Circuit.h/.cpp)
│   ├── Algorithms/ (AlgorithmsPlaceholder.h)
│   ├── Tests/ (TestRunner.cpp)
│   └── main.cpp
│
├── Computer Graphics/                  # PART 2: Computer Graphics (OpenGL)
│   ├── OpenGL/ (GLWindow.h, GLWindow.cpp)
│   ├── Rendering/ (GateRenderer.h, GateRenderer.cpp)
│   ├── Canvas/ (Canvas.h, Canvas.cpp)
│   ├── Transformations/ (Camera.h, Camera.cpp)
│   └── main.cpp
│
├── java/                               # PART 3: Problem Solving Using OOP (Java)
│   ├── gates/ (LogicGate.java, ANDGate.java, ORGate.java, NOTGate.java, XOR/NAND/NORGate.java, GateEvaluator.java)
│   ├── circuit/ (Circuit.java, Connection.java)
│   ├── project/ (Project.java)
│   ├── reports/ (ReportGenerator.java)
│   ├── simulation/ (SimulationEngine.java)
│   └── Main.java
│
├── COA/                                # PART 4: Computer Organization & Architecture / ALP
│   ├── ALP/ (logic_ops_64.s - Native x86-64 Assembly)
│   ├── BooleanOperations/ (BooleanALU.h, BooleanALU.cpp)
│   ├── ProcessorSimulation/ (ProcessorSimulator.h, ProcessorSimulator.cpp)
│   ├── Tests/ (TestALP.cpp)
│   └── main.cpp
│
├── shared/
│   └── circuit_data.json               # Common reference circuit data
└── docs/
    ├── Architecture/
    ├── CO-Mapping/
    └── Review-2/ (Review2-Implementation.md)
```

---

## 🚀 How to Run Each Module

All modules have been compiled and verified on Windows with GCC 64-bit, standard Windows OpenGL (`opengl32`, `gdi32`), and Java 17+.

### ⚡ Quick Start: Master Demonstration
To run all modules sequentially with automated test verification:
```powershell
Set-Location "C:\Users\kunal\.gemini\antigravity-ide\scratch\LogicCraft"
.\run_all.ps1
```

---

### Module 1: C++ / Programming Lab (CO1 & CO2)
* **Direct Script:** Double-click `run_cpp.bat`
* **Manual Compilation:**
  ```powershell
  cd "Cpp"
  g++ -std=c++14 main.cpp Gates/Gate.cpp DataStructures/ArrayOperations.cpp DataStructures/LinkedList.cpp CircuitEngine/TruthTable.cpp CircuitEngine/Circuit.cpp -o logiccraft_cpp.exe
  .\logiccraft_cpp.exe
  ```
* **Run Unit Tests:**
  ```powershell
  g++ -std=c++14 Tests/TestRunner.cpp Gates/Gate.cpp DataStructures/ArrayOperations.cpp DataStructures/LinkedList.cpp CircuitEngine/TruthTable.cpp CircuitEngine/Circuit.cpp -o Tests/test_runner.exe
  .\Tests\test_runner.exe
  ```

---

### Module 2: Computer Graphics / OpenGL (CO1 & CO2)
* **Direct Script:** Double-click `run_graphics.bat`
* **Manual Compilation:**
  ```powershell
  cd "Computer Graphics"
  g++ -std=c++14 main.cpp OpenGL/GLWindow.cpp Rendering/GateRenderer.cpp Canvas/Canvas.cpp Transformations/Camera.cpp -lopengl32 -lgdi32 -o logiccraft_cg.exe
  .\logiccraft_cg.exe
  ```
* **Automated Self-Test (Headless / Non-Interactive):**
  ```powershell
  .\logiccraft_cg.exe --test
  ```
* **Interactive Canvas Controls:**
  * **Left-Click & Drag:** Move selected gate (**Translation — CO2**)
  * **Right-Click & Drag / Arrow Keys:** Pan camera canvas (**Translation — CO2**)
  * **Mouse Wheel / (+ / - Keys):** Zoom In / Out (**Scaling — CO2**)
  * **Keys [1], [2], [3]:** Select AND, OR, or NOT gate
  * **Spacebar:** Toggle input of selected gate and re-evaluate circuit in real time
  * **Key [R]:** Reset camera view to origin
  * **ESC:** Exit window

---

### Module 3: Java / Problem Solving Using OOP (CO1 & CO2)
* **Direct Script:** Double-click `run_java.bat`
* **Manual Compilation:**
  ```powershell
  cd "java"
  if (-not (Test-Path "bin")) { New-Item -ItemType Directory -Path "bin" }
  javac -d bin gates/*.java circuit/*.java project/*.java reports/*.java simulation/*.java Main.java
  java -cp bin Main
  ```

---

### Module 4: COA / 64-Bit Assembly (ALP) (CO1 & CO2)
* **Direct Script:** Double-click `run_coa.bat`
* **Manual Compilation:**
  ```powershell
  cd "COA"
  g++ -std=c++14 main.cpp ProcessorSimulation/ProcessorSimulator.cpp BooleanOperations/BooleanALU.cpp ALP/logic_ops_64.s -o logiccraft_coa.exe
  .\logiccraft_coa.exe
  ```
* **Run Unit Tests:**
  ```powershell
  g++ -std=c++14 Tests/TestRunner.cpp ProcessorSimulation/ProcessorSimulator.cpp BooleanOperations/BooleanALU.cpp ALP/logic_ops_64.s -o Tests/test_alp.exe
  .\Tests\test_alp.exe
  ```

---

## 🎯 Viva & Faculty Demonstration Sequence (12 Steps)

Follow this exact flow during project review:

| Step | Action | Academic Concept Demonstrated |
| :---: | :--- | :--- |
| **STEP 1** | Open the LogicCraft project directory. | Show unified modular folder structure for all 4 subjects. |
| **STEP 2** | Launch `run_graphics.bat`. | Observe the dark IDE canvas and coordinate grid drawn using `GL_LINES`. |
| **STEP 3** | Inspect Gate symbols on canvas. | Show AND, OR, NOT, input/output pins, and stepped wires rendered via **OpenGL Primitives (CG CO1)**. |
| **STEP 4** | Click on a gate and drag it with mouse. | Demonstrate **Translation Transformation (CG CO2)** updating gate model coordinates. |
| **STEP 5** | Use scroll wheel to zoom and right-click to pan. | Demonstrate **Scaling & Camera Viewport Handling (CG CO2)**. Gates outside frustum are clipped. |
| **STEP 6** | Open Java source and launch `run_java.bat`. | Display the `LogicGate` abstract base class and concrete subclasses (`ANDGate`, `ORGate`, `NOTGate`, etc.). |
| **STEP 7** | Review Java console output. | Demonstrate **Polymorphism (Java CO2)**: single `LogicGate` reference bound to different gates; show Lambda Majority Gate and `StringBuilder` report. |
| **STEP 8** | Launch `run_cpp.bat`. | Show **Arrays (C++ CO1)**: CRUD operations on `GateArray` and automated $2^N$ row generation in `TruthTable`. |
| **STEP 9** | Review C++ Linked List output. | Show **Linked Lists (C++ CO2)**: heap-allocated `GateNode` traversal, dynamic insertion, deletion with memory cleanup, and pointer inspection. |
| **STEP 10** | Review End-to-End Circuit Evaluation. | Show signals propagating through G1 $\rightarrow$ G2 $\rightarrow$ G3 across both C++ and Java engines. |
| **STEP 11** | Launch `run_coa.bat`. | Show **Processor-Level Modeling (COA CO1)**: micro-operation RTL trace loading R1, R2, ALU execution, and status flags (ZF, SF). |
| **STEP 12** | Inspect 64-Bit Assembly execution. | Show **64-Bit Assembly (COA CO2)**: native execution of `asm_and_64`, `asm_or_64`, and conditional jump verification (`cmp`, `je`, `jne`). |

---

## 💡 Viva Q&A Cheat Sheet

1. **Why use Arrays in C++ for CO1?**
   * Arrays provide direct contiguous memory storage, index-based $O(1)$ access, and a natural tabular representation for truth tables where all $2^N$ input bit combinations are stored in row arrays.
2. **Why use a Linked List in C++ for CO2 instead of an Array?**
   * In a visual circuit simulator, users can add and delete components on the canvas at runtime without predictable upper bounds. A linked list provides dynamic heap allocation without fixed array capacity and enables $O(1)$ pointer insertion and deletion without shifting elements.
3. **How does OpenGL Primitive rendering work in CO1?**
   * Gates are decomposed into fundamental geometric primitives: rectangles (`GL_QUADS`), circular arcs (`GL_LINE_STRIP` / `GL_TRIANGLE_FAN`), triangles (`GL_TRIANGLES`), and wire segments (`GL_LINES`).
4. **How are Transformations handled in CO2?**
   * Model transformation uses `glTranslatef(gate.x, gate.y, 0)` to place gates. Camera transformation applies viewport orthographic projection (`glOrtho`) combined with pan translation and zoom scaling.
5. **How does Runtime Polymorphism work in Java CO2?**
   * The abstract class `LogicGate` declares `abstract boolean evaluate()`. At runtime, a reference of type `LogicGate` points to an instance of `ANDGate`, `ORGate`, or `NOTGate`. The JVM executes the concrete overridden method via dynamic method dispatch (vtable).
6. **How does the 64-Bit Assembly implementation work in COA CO2?**
   * It uses native x86-64 machine instructions (`mov`, `and`, `or`, `cmp`) following the 64-bit ABI. Operands are passed in 64-bit registers (`RCX`, `RDX`), executed by the hardware ALU, and returned in `RAX`. Conditional branches (`je`, `jne`) inspect the Zero Flag (`ZF`) to alter control flow.
