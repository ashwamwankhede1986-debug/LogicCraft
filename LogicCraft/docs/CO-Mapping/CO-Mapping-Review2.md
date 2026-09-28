# LogicCraft — Course Outcome (CO) Mapping Specification
## Review-2 Academic Alignment Document

This document records the exact alignment between each academic course syllabus, the designated Course Outcomes (CO1 & CO2), and the concrete software implementations delivered in Review 2.

---

### Course 1: Programming Lab (C++)

* **CO1: Arrays**
  * **Syllabus Competency:** Understand and implement linear contiguous data storage, array indexing, insertion, deletion with shifting, updates, and linear search.
  * **LogicCraft Implementation:**
    * `GateArray`: Contiguous array of `Gate` structures with fixed capacity `MAX_CIRCUIT_GATES`.
    * Implemented and tested CRUD: `insertGate()`, `deleteGate()` with left-shift contiguous consolidation, `updateGate()`, `searchGateById()`, `searchGateByName()`, `displayAll()`.
    * `TruthTable`: Generates all $2^N$ binary rows in 2D array representation and computes output column array for AND, OR, and NOT gates.
* **CO2: Linked List**
  * **Syllabus Competency:** Understand dynamic memory allocation, pointer manipulation, and dynamic node addition and deletion.
  * **LogicCraft Implementation:**
    * `CircuitLinkedList`: Singly linked list of heap-allocated `GateNode` structures (`gateId`, `gateType`, `inputs[]`, `outputValue`, `next` pointer).
    * Dynamic CRUD: `insertNode()`, `deleteNode()` with proper pointer rewiring and `delete` memory deallocation, `updateNode()`, `searchNode()`, `traverseAndDisplay()`.
    * Viva Justification: Enables arbitrary runtime component additions and deletions on the circuit canvas without static array bounds.

---

### Course 2: Computer Graphics (OpenGL)

* **CO1: OpenGL Primitives**
  * **Syllabus Competency:** Understand basic graphics primitives (points, lines, polygons) and window coordinate systems.
  * **LogicCraft Implementation:**
    * Hardware canvas with dark IDE theme and coordinate grid via `GL_LINES`.
    * AND Gate: Constructed using `GL_QUADS` (rectangle) and `GL_TRIANGLE_FAN` / `GL_LINE_STRIP` (semi-circle).
    * OR Gate: Concave input arc and converging arched lines.
    * NOT Gate: `GL_TRIANGLES` and `GL_LINE_LOOP` circular inversion bubble.
    * Input/Output Pins and Orthogonal Stepped Wires (`GL_LINE_STRIP`).
    * Vector stroke font typography drawn using pure `GL_LINES`.
* **CO2: Transformation & Clipping**
  * **Syllabus Competency:** Implement 2D geometric transformations (translation, scaling) and viewport boundary clipping.
  * **LogicCraft Implementation:**
    * Translation: Interactive mouse dragging of gates and canvas panning via `glTranslatef()`.
    * Scaling: Canvas zooming via `glScalef()` and orthographic frustum resizing.
    * Coordinate Conversion: Screen pixel coordinates converted to World Space via inverse projection (`screenToWorld`).
    * Viewport & Frustum Clipping: Window resizing handled with `glViewport()`; gates outside the camera frustum are culled via Axis-Aligned Bounding Box (AABB) clipping tests.

---

### Course 3: Problem Solving Using OOP (Java)

* **CO1: Java Fundamentals**
  * **Syllabus Competency:** Master core object-oriented principles, classes, methods, arrays, control flow, encapsulation, and string handling.
  * **LogicCraft Implementation:**
    * Domain classes: `LogicGate`, `ANDGate`, `ORGate`, `NOTGate`, `Circuit`, `Connection`, `Project`, and `ReportGenerator`.
    * Strict encapsulation: Private/protected instance fields, getters/setters, input validation.
    * Boolean inputs stored and manipulated using arrays (`boolean[] inputs`).
    * `ReportGenerator`: Formats circuit inventory and wire connections using `StringBuilder`.
* **CO2: Advanced OOP**
  * **Syllabus Competency:** Apply inheritance, abstract classes, method overriding, runtime polymorphism, static/final modifiers, and lambda expressions.
  * **LogicCraft Implementation:**
    * `abstract class LogicGate`: Extends to `ANDGate`, `ORGate`, `NOTGate`, `XORGate`, `NANDGate`, `NORGate`.
    * Runtime Polymorphism: Overridden `evaluate()` called polymorphically through base `LogicGate` references.
    * Keywords: `static final int MAX_INPUTS`, static instantiation counters, `final getId()`.
    * Functional Interface & Lambdas: `@FunctionalInterface GateEvaluator` implementing a custom Majority Vote gate and stream/predicate filtering of active circuit gates.

---

### Course 4: Computer Organization & Architecture / ALP (64-Bit Assembly)

* **CO1: Processor-Level Modeling**
  * **Syllabus Competency:** Understand CPU register architecture, ALU operations, instruction decoding, and status flags.
  * **LogicCraft Implementation:**
    * `ProcessorSimulator`: Models general purpose registers (`R1`, `R2`, `R3`), Program Counter (`PC`), Instruction Register (`IR`), and Flags Register (`ZF`, `SF`, `CF`).
    * Register-Transfer Level (RTL) trace detailing the step-by-step fetch, load, ALU execution, and status flag update for Boolean logic operations.
* **CO2: 64-Bit Assembly (ALP)**
  * **Syllabus Competency:** Write, assemble, and execute native 64-bit assembly routines utilizing registers, logic instructions, comparisons, and conditional branching.
  * **LogicCraft Implementation:**
    * Pure x86-64 assembly in `COA/ALP/logic_ops_64.s` using Microsoft x64 ABI.
    * Instructions: `mov`, `and`, `or`, `xor`, `cmp`.
    * 64-bit Registers: `RAX`, `RCX`, `RDX`, `R8`, `R9`.
    * Conditional Branching: `asm_branch_verify_64` utilizes `cmp` and conditional branches (`je`, `jne`) to verify gate evaluation outcomes and redirect control flow.
