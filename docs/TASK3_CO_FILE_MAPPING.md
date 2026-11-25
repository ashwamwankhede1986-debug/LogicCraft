# LogicCraft — Task 3 Academic Course Outcome (CO) to File Mapping

**Project Title:** LogicCraft — Visual Logic Gate Simulator  
**Task Phase:** Task 3 Implementation & Cross-Subject Integration  
**Academic Target Date:** 19 October 2026 (Verified & Complete)  
**Authoritative Documentation for Viva & Reviewer Code Inspection**

---

## 1. Executive Summary Table

| Subject | CO | Concept | Exact File Path | Function/Class | Project Usage | Test/Demo | Status |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Programming Lab** | **CO3** | Stack & Queue Data Structures (LIFO Undo & FIFO Simulation) | `Cpp/DataStructures/StackOperations.cpp`<br>`Cpp/DataStructures/QueueOperations.cpp`<br>`Cpp/CircuitEngine/Circuit.cpp` | `CircuitUndoStack`<br>`pushAction()`<br>`popAction()`<br>`SimulationQueue`<br>`enqueueEvent()`<br>`executeUndoOperation()` | Procedural LIFO stack stores reverse circuit actions (add/delete gate, toggle input, add wire) for non-destructive Undo. Circular FIFO queue processes discrete Boolean signal ripple events in arrival order. Protected against underflow and overflow. | Run `Cpp/Tests/test_runner.exe` or `.\run_cpp.bat` (Test 4 & 5). Live Web Canvas: Click `↺ Undo`. | **Complete** |
| **Computer Organization & Architecture Lab** | **CO3** | 64-Bit Assembly Arithmetic & ASCII String Processing | `COA/ALP/logic_ops_64.s`<br>`COA/main.cpp` | `asm_mul_successive_addition_64`<br>`asm_compute_truth_table_rows_64`<br>`asm_parse_gate_type_64`<br>`asm_validate_command_64`<br>`asm_eval_boolean_expr_str_64` | 64-bit AMD64 assembly calculates circuit propagation delay via repeated addition (multiplication without hardware `imul`) and $2^N$ truth table rows via successive doubling. Parses ASCII gate opcodes and evaluates Boolean string expressions in hardware registers. | Run `COA/Tests/test_alp.exe` or `.\run_coa.bat`. Web Demo: Task 3 Tab -> 64-Bit Assembly Calculator. | **Complete** |
| **Computer Graphics Lab** | **CO6** | Texture Mapping & Material Lighting / Color Effects | `Computer Graphics/Rendering/GateRenderer.cpp`<br>`Computer Graphics/OpenGL/GLWindow.cpp`<br>`Computer Graphics/Canvas/Canvas.cpp` | `GateRenderer::initTextures()`<br>`GateRenderer::initLighting()`<br>`GateRenderer::drawTexturedSubstrate()`<br>`GateRenderer::applyMaterial()`<br>`GateRenderer::toggleTextures()` | Maps 2D procedural textured PCB copper trace substrate and silicon IC package patterns using `GL_TEXTURE_2D`. Employs OpenGL `GL_LIGHT0`, material specular highlights, and active terminal emissive glows toggled dynamically with keys `[T]` and `[L]`. | Run `logiccraft_cg.exe --test` or `.\run_graphics.bat`. Web Canvas: Toggle `PCB Substrate (CO6)` & `Signal Glow (CO6)`. | **Complete** |
| **Problem Solving Using OOP (Java)** | **CO3** | Exception Handling, Collections, Swing GUI & Package Architecture | `java/src/logiccraft/exceptions/InvalidInputException.java`<br>`java/src/logiccraft/collections/ComponentList.java`<br>`java/src/logiccraft/ui/LogicCraftUI.java`<br>`java/src/logiccraft/Main.java` | `InvalidInputException`<br>`ComponentList<T>`<br>`LogicCraftUI`<br>`logiccraft.*` packages | Authoritative Java package hierarchy. Validates circuit constraints and pin indices with custom exceptions. Generic collection manages gates and connections. Launches complete Java Swing desktop GUI. | Run `java/src/test/ExceptionTest.java` and `GenericTest.java`, or `.\run_java.bat --gui`. | **Complete** |

---

## 2. In-Depth Subject Implementation Details

### A. Programming Lab — C++ (CO3: Stack and Queue)

> **Important Academic Constraint:** The C++ syllabus for this student does **not** include OOP. No classes, inheritance, polymorphism, or virtual methods were introduced for this outcome. All implementations use pure procedural C-style structs and functions.

1. **Exact Source Files:**
   - [StackOperations.h](file:///c:/Users/LOQ/Downloads/LogicCraft/Cpp/DataStructures/StackOperations.h) (Lines 1–45)
   - [StackOperations.cpp](file:///c:/Users/LOQ/Downloads/LogicCraft/Cpp/DataStructures/StackOperations.cpp) (Lines 1–135)
   - [QueueOperations.h](file:///c:/Users/LOQ/Downloads/LogicCraft/Cpp/DataStructures/QueueOperations.h) (Lines 1–42)
   - [QueueOperations.cpp](file:///c:/Users/LOQ/Downloads/LogicCraft/Cpp/DataStructures/QueueOperations.cpp) (Lines 1–125)
   - [Circuit.cpp](file:///c:/Users/LOQ/Downloads/LogicCraft/Cpp/CircuitEngine/Circuit.cpp) (Lines 185–270)
2. **Exact Functions & Structs:**
   - `struct CircuitAction`: Stores `actionType` (`ACTION_ADD_GATE`, `ACTION_DELETE_GATE`, `ACTION_TOGGLE_INPUT`, `ACTION_ADD_CONNECTION`), gate IDs, pin indices, previous states, and gate definitions.
   - `struct CircuitUndoStack`: Procedural stack with array `CircuitAction actions[MAX_UNDO_STACK_SIZE]`, integer `top`, and `capacity = 32`.
   - `initUndoStack(CircuitUndoStack* stack)`: Initializes empty stack pointer (`top = -1`).
   - `pushAction(CircuitUndoStack* stack, const CircuitAction* action)`: Pushes action with overflow boundary protection.
   - `popAction(CircuitUndoStack* stack, CircuitAction* poppedAction)`: Pops top action with underflow boundary protection.
   - `struct SimulationEvent`: Stores `eventId`, `targetGateId`, `pinIndex`, `newSignalValue`, and descriptive message.
   - `struct SimulationQueue`: Procedural circular buffer with `SimulationEvent events[MAX_SIM_QUEUE_SIZE]`, `front`, `rear`, and `count = 32`.
   - `enqueueEvent(SimulationQueue* queue, const SimulationEvent* event)`: Inserts event at rear with circular wrap-around and overflow detection.
   - `dequeueEvent(SimulationQueue* queue, SimulationEvent* dequeuedEvent)`: Removes event from front with circular wrap-around and underflow detection.
   - `executeUndoOperation(CircuitUndoStack* stack, ...)`: Reverts the most recent user action in LIFO order and restores prior state.
3. **Project Usage:**
   - Every time a gate is added, deleted, an input pin is toggled, or a wire is connected, an action token is pushed onto `CircuitUndoStack`.
   - Invoking Undo pops the token and executes the algebraic inverse (e.g., inverting `ADD_GATE` by removing the gate).
   - Signal changes trigger `SimulationEvent` tokens enqueued in `SimulationQueue` to propagate ripple outputs in FIFO sequence.
4. **Demonstration & Test Evidence:**
   - Run: `Cpp/Tests/test_runner.exe`
   - Verified Tests:
     - `testStackOperations()`: Verifies LIFO push/pop order, empty underflow, and 32-element full overflow protection.
     - `testQueueOperations()`: Verifies FIFO enqueue/dequeue order, circular wrap-around, empty underflow, and 32-element full overflow protection.
     - `testCircuitUndoIntegration()`: Adds a gate, connects a wire, toggles input, and undos each sequentially, verifying the circuit returns to its exact original state.

---

### B. Computer Organization and Architecture Lab (CO3: 64-Bit Assembly)

1. **Exact Source Files:**
   - [logic_ops_64.s](file:///c:/Users/LOQ/Downloads/LogicCraft/COA/ALP/logic_ops_64.s) (Lines 135–330)
   - [main.cpp](file:///c:/Users/LOQ/Downloads/LogicCraft/COA/main.cpp) (Lines 180–260)
   - [TestALP.cpp](file:///c:/Users/LOQ/Downloads/LogicCraft/COA/Tests/TestALP.cpp) (Lines 95–175)
2. **Exact Functions & Assembly Labels:**
   - `asm_mul_successive_addition_64(uint64_t a, uint64_t b)`:
     - Implements multiplication using repeated addition in 64-bit AMD64 assembly.
     - Algorithm: Sets `%rax = 0`, `%rcx = b`. In a loop, adds `%rdi` (`a`) into `%rax` and decrements `%rcx` until zero.
     - Registers used: `%rax` (accumulator/return), `%rcx` (loop counter), `%r8` (multiplicand). Handles `b = 0` boundary cleanly.
   - `asm_compute_truth_table_rows_64(uint64_t num_inputs)`:
     - Computes $2^N$ truth table rows using successive addition (`rows = rows + rows`) in a loop.
     - Registers used: `%rax` (row count accumulator initialized to 1), `%rcx` (input counter). Caps at 64.
   - `asm_parse_gate_type_64(const char* str)`:
     - 64-bit ASCII string processor that scans input byte-by-byte, converts characters to uppercase, and matches string prefixes to gate opcodes (1: AND, 2: OR, 3: NOT, 4: NAND, 5: NOR, 6: XOR, 7: XNOR, 0: Unknown).
   - `asm_validate_command_64(const char* cmd_str)`:
     - Validates circuit CLI commands (`ADD`, `CONNECT`, `SIMULATE`, `DELETE`, `TOGGLE`).
   - `asm_eval_boolean_expr_str_64(const char* expr_str)`:
     - Evaluates simple Boolean string expressions (e.g., `"1 & 0"`, `"1 | 0"`, `"~1"`) directly in assembly registers.
3. **Project Usage:**
   - Simulates hardware multiplier units inside arithmetic logic units (ALU).
   - Dynamically calculates the propagation delay: $\text{Total Delay} = \text{Stages} \times \text{Gate Delay (5ns)}$ using repeated addition.
   - Validates user input commands and parses gate opcodes from string inputs.
4. **Demonstration & Test Evidence:**
   - Run: `COA/Tests/test_alp.exe`
   - Verified Tests:
     - Successive addition: `asm_mul_successive_addition_64(5, 3)` returns `15`; boundary `asm_mul_successive_addition_64(7, 0)` returns `0`.
     - Truth table rows: `N = 1 -> 2`, `N = 3 -> 8`, `N = 4 -> 16`, boundary `N = 0 -> 1`.
     - String parsing: `"AND"` returns opcode `1`, `"OR"` returns `2`, `"NOT"` returns `3`, `"XOR"` returns `6`, invalid string returns `0`.
     - Expression evaluator: `"1 & 1"` returns `1`, `"1 & 0"` returns `0`, `"0 | 1"` returns `1`, `"~1"` returns `0`.

---

### C. Computer Graphics Lab (CO6: Texture Mapping & Lighting Effects)

1. **Exact Source Files:**
   - [GateRenderer.h](file:///c:/Users/LOQ/Downloads/LogicCraft/Computer%20Graphics/Rendering/GateRenderer.h) (Lines 20–62)
   - [GateRenderer.cpp](file:///c:/Users/LOQ/Downloads/LogicCraft/Computer%20Graphics/Rendering/GateRenderer.cpp) (Lines 25–160)
   - [GLWindow.cpp](file:///c:/Users/LOQ/Downloads/LogicCraft/Computer%20Graphics/OpenGL/GLWindow.cpp) (Lines 165–175, 254–263)
   - [Canvas.cpp](file:///c:/Users/LOQ/Downloads/LogicCraft/Computer%20Graphics/Canvas/Canvas.cpp) (Lines 35–55)
2. **Exact Functions & Methods:**
   - `GateRenderer::initTextures()`:
     - Generates two procedural 2D textures: `boardTextureId` (procedural dark emerald PCB substrate with copper grid pattern) and `chipTextureId` (procedural dark matte silicon IC texture).
     - Uses standard OpenGL calls: `glGenTextures`, `glBindTexture(GL_TEXTURE_2D, ...)`, `glTexParameteri`, and `glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, data)`.
   - `GateRenderer::initLighting()`:
     - Configures directional light `GL_LIGHT0` with ambient (`0.2, 0.2, 0.2`), diffuse (`0.8, 0.8, 0.8`), and specular (`1.0, 1.0, 1.0`) components.
     - Enables `GL_COLOR_MATERIAL` with `glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE)` to blend realistic shading with gate symbol colors.
   - `GateRenderer::drawTexturedSubstrate(minX, minY, maxX, maxY)`:
     - Binds `boardTextureId`, enables `GL_TEXTURE_2D`, and renders a textured quadrilateral substrate mapped across the circuit workspace.
   - `GateRenderer::applyMaterial(bool isSelected, bool isActiveSignal)`:
     - Dynamically applies material specular highlights and emissive glows for active logic signals (`GL_EMISSION`).
   - `GateRenderer::toggleTextures()` and `toggleLighting()`:
     - Allows runtime toggling via keyboard shortcuts `[T]` and `[L]`.
3. **Project Usage:**
   - Provides an authentic electronic CAD environment with a textured PCB board beneath logic gates without reducing schematic readability.
   - Active signals and selected gates emit glowing highlights to guide user focus during simulation.
4. **Demonstration & Test Evidence:**
   - Run: `Computer Graphics/logiccraft_cg.exe --test`
   - Verified Output:
     - Context initialized on Intel hardware renderer (OpenGL 4.6.0).
     - Texture IDs successfully generated and bound.
     - Lighting and color materials enabled and toggled.
     - Viewport clipping, gate translation, and zoom scaling verified.

---

### D. Problem Solving Using OOP — Java (CO3: Exceptions, Collections & Swing)

1. **Exact Source Files:**
   - [InvalidInputException.java](file:///c:/Users/LOQ/Downloads/LogicCraft/java/src/logiccraft/exceptions/InvalidInputException.java) (Lines 1–24)
   - [ComponentList.java](file:///c:/Users/LOQ/Downloads/LogicCraft/java/src/logiccraft/collections/ComponentList.java) (Lines 1–52)
   - [LogicCraftUI.java](file:///c:/Users/LOQ/Downloads/LogicCraft/java/src/logiccraft/ui/LogicCraftUI.java) (Lines 1–320)
   - [Main.java](file:///c:/Users/LOQ/Downloads/LogicCraft/java/src/logiccraft/Main.java) (Lines 1–180)
2. **Exact Classes & Methods:**
   - `logiccraft.exceptions.InvalidInputException`:
     - Custom checked exception extending `java.lang.Exception`. Thrown when an invalid logic value ($\neq 0, 1$), out-of-bounds pin index, or cyclic circuit loop is detected.
   - `logiccraft.collections.ComponentList<T extends Gate>`:
     - Custom generic collection implementing `Iterable<T>`. Supports `add(T component)`, `removeById(int id)`, `findById(int id)`, `filterByType(String type)`, and boundary validation.
   - `logiccraft.ui.LogicCraftUI`:
     - Complete Java Swing graphical interface featuring component toolbars, visual circuit canvas, truth table display dialogs, and menu actions.
   - `logiccraft.file.ProjectFileManager`:
     - Serialization and deserialization of circuit state to text and JSON formats.
3. **Project Usage:**
   - Validates user input before simulation execution, preventing runtime crashes.
   - Manages circuit gates in memory with type-safe collections.
   - Provides an optional standalone Java Swing GUI desktop application.
4. **Demonstration & Test Evidence:**
   - Run: `java/src/test/ExceptionTest.java` and `GenericTest.java`
   - Verified Tests:
     - `testInvalidLogicValue()`: Setting input to `5` correctly throws `InvalidInputException: Invalid logic input value: 5. Must be 0 or 1.`
     - `testInvalidPinIndex()`: Accessing pin `-1` or `99` correctly triggers boundary exception.
     - `ComponentListTest`: Tests adding, finding, filtering, and removing generic gate components.

---

## 3. How to Execute and Demonstrate Each Subject

### Full Multi-Subject Suite
Run the master PowerShell demonstration script from the workspace root:
```powershell
powershell -ExecutionPolicy Bypass -File .\run_all.ps1
```

### Full-Stack Web Application (Frontend + Backend)
Double-click `run_web.bat` or run:
```cmd
run_web.bat
```
Then open [http://localhost:3000](http://localhost:3000) in your web browser.

### Independent Subject Execution Commands
- **C++:** `.\run_cpp.bat` (Console demo) or `cd Cpp && Tests\test_runner.exe` (Unit tests)
- **COA / Assembly:** `.\run_coa.bat` (Console demo) or `cd COA && Tests\test_alp.exe` (Unit tests)
- **Computer Graphics:** `.\run_graphics.bat` (Interactive OpenGL GUI) or `cd "Computer Graphics" && logiccraft_cg.exe --test` (Automated verification)
- **Java:** `.\run_java.bat` (Console demo) or `.\run_java.bat --gui` (Java Swing GUI)
