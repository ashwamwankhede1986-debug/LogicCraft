# LogicCraft — Task 4 Technical Implementation Guide

**Project Name:** LogicCraft — Visual Logic Gate Simulator  
**Phase:** Task 4 Implementation & System Integration  
**Academic Target:** Semester III Exam Reform Guidelines  
**Authoritative Reference for Engineering Viva Voce & Code Review**

---

## 1. Architectural Overview & Design Principles

LogicCraft is a multi-disciplinary visual logic gate simulator integrating four core computer science disciplines:
1. **Programming Lab (C++):** Procedural data structures and algorithms (Arrays, Linked Lists, Stacks, Queues, Searching, Sorting, Hashing).
2. **Computer Organization & Architecture (COA):** Native 64-bit AMD64 assembly language programming (ALP), register-level logic execution, successive addition delay computation, and string opcode parsing.
3. **Computer Graphics (OpenGL):** 2D primitive rendering, affine transformations, viewing transformations, coordinate alignment precision, and procedural texture mapping with Phong lighting.
4. **Problem Solving Using OOP (Java):** Object-oriented simulation engine, inheritance hierarchies, custom exception safety, generic collections (`ComponentList<T>`), file persistence, and Java Swing GUI.

### Strict Procedural C++ Academic Constraint
Per academic guidelines, the C++ module **strictly excludes Object-Oriented Programming (OOP)**. No classes (`class`), inheritance (`:`), polymorphism (`virtual`), encapsulation (`private`/`public`), or member methods were introduced. All algorithms and data structures are implemented using:
- C-style structures (`struct`)
- Flat arrays and pointer references
- Procedural functions taking explicit data pointers
- Clear imperative logic suitable for a second-year student to explain clearly in a viva.

---

## 2. Programming Lab — Task 4 (CO4 & CO5 Implementation)

### 2.1 Searching Techniques (CO4)
Source: `Cpp/Searching/Searching.cpp`, `Cpp/Searching/Searching.h`

#### A. Linear Search (`linearSearchById`, `linearSearchByName`)
- **Mechanism:** Iterates sequentially through the array from index `0` to `size - 1`.
- **Comparison Metric:** Increments an integer comparison counter at every iteration.
- **Complexity:**
  - Best Case: $O(1)$ (element at index 0).
  - Worst Case: $O(N)$ (element at index $N-1$ or not present).
  - Average Case: $\frac{N+1}{2}$ comparisons.
- **Unfound Elements:** Returns `-1` if the loop terminates without a match.

#### B. Binary Search (`binarySearchById`, `binarySearchByName`)
- **Mechanism:** Requires the component array to be pre-sorted by the search key. Maintains `low` and `high` indices and examines the midpoint:
  $$\text{mid} = \text{low} + \frac{\text{high} - \text{low}}{2}$$
- **Comparison Metric:** Increments comparison counter at each midpoint evaluation.
- **Complexity:**
  - Best Case: $O(1)$ (element at exact midpoint).
  - Worst Case: $O(\log_2 N)$ comparisons.
- **Unfound Elements:** Returns `-1` when $\text{low} > \text{high}$.

---

### 2.2 Sorting Techniques (CO4)
Source: `Cpp/Sorting/Sorting.cpp`, `Cpp/Sorting/Sorting.h`

#### A. Bubble Sort (`bubbleSortComponents`)
- **Mechanism:** Repeatedly steps through the list, compares adjacent elements, and swaps them if they are in the wrong order. Includes an early-exit optimization flag (`swapped` boolean). If a complete pass finishes without a single swap, the array is already sorted and the algorithm terminates immediately.
- **Supported Keys:** Component ID, Component Name, or Gate Type in Ascending or Descending order.
- **Metrics Tracked:** Total comparisons and total swap operations.
- **Complexity:**
  - Worst/Average Comparisons: $\frac{N(N-1)}{2} = O(N^2)$.
  - Best Case (already sorted): $N-1$ comparisons, 0 swaps.

#### B. Selection Sort (`selectionSortComponents`)
- **Mechanism:** Divides the array into a sorted sublist and an unsorted sublist. In each pass, finds the minimum (or maximum) element from the unsorted sublist and swaps it into the beginning of the unsorted sublist.
- **Key Viva Distinction:** Selection sort performs the exact same number of comparisons as Bubble Sort ($O(N^2)$), but guarantees **at most $N-1$ swaps** ($O(N)$ write operations). This is critical in embedded systems or hardware where flash memory writes are expensive.

---

### 2.3 Hashing with Linear Probing (CO5)
Source: `Cpp/Hashing/HashTable.cpp`, `Cpp/Hashing/HashTable.h`

#### A. Data Structure & Prime Modulus
- **Capacity:** Fixed at prime number $M = 17$. Prime table sizes reduce hash collisions under modular arithmetic.
- **Slot States:**
  - `SLOT_EMPTY (0)`: Never contained an element.
  - `SLOT_OCCUPIED (1)`: Contains an active component.
  - `SLOT_TOMBSTONE (2)`: Contained an element that was deleted. Essential so search probes do not prematurely terminate on deleted slots.

#### B. Hash Function (Polynomial Rolling Hash)
For string-based component keys (e.g., `"G1"`, `"Primary_AND"`):
$$H = \left(\sum_{i=0}^{L-1} \text{str}[i] \times 31^i\right) \pmod{17}$$
The prime multiplier 31 ensures good bit distribution across character permutations.

#### C. Collision Resolution (Linear Probing)
When slot $h_0$ is occupied by another key:
$$h_i = (h_0 + i) \pmod{17} \quad \text{for } i = 1, 2, \dots, 16$$
- **Insertion:** Probes until finding either a `SLOT_EMPTY` or `SLOT_TOMBSTONE`.
- **Search:** Probes until finding the matching key or encountering a `SLOT_EMPTY`. Tombstones are skipped over during search.
- **Deletion:** Marks the matching slot as `SLOT_TOMBSTONE` and decrements active count.
- **Full Table Handling:** If $i$ reaches 17 without finding a slot, returns an overflow error (`false`), preventing infinite probe loops.

---

### 2.4 Empirical Performance Benchmark Suite
Source: `Cpp/Algorithms/PerformanceComparison.cpp`

The benchmarking suite runs automated empirical tests across dataset sizes $N = 8, 16, 32$:
- Demonstrates how Binary Search scales logarithmically ($\approx 3, 4, 5$ comparisons) versus Linear Search scaling linearly ($\approx 4.5, 8.5, 16.5$ average comparisons).
- Quantifies Bubble Sort vs Selection Sort swap counts, proving Selection Sort uses up to $16\times$ fewer swap writes on larger datasets.

---

## 3. C++ Problem Solving — Task 4 (Smart Circuit Validation & Decision Engine)

Source: `Cpp/CircuitEngine/CircuitValidator.cpp`, `Cpp/CircuitEngine/CircuitValidator.h`

### 3.1 Design & Decision-Making Capabilities
The validation engine performs topological and semantic analysis on the circuit graph before simulation:
1. **Gate Connectivity & Range Checks:** Validates that wire source and target gate IDs exist in the active circuit table and pin indices are within bounds.
2. **Floating Pin Detection:** Detects input terminals that have no incoming connections and no default input state, flagging them as dangling floating nodes.
3. **Self-Loop & Feedback Cycle Detection:** Logic gates in combinational circuits must form a Directed Acyclic Graph (DAG). The validator runs a procedural Depth-First Search (DFS) using 3-color vertex marking:
   - `0 (WHITE)`: Unvisited.
   - `1 (GRAY)`: Currently in DFS recursion stack (active path).
   - `2 (BLACK)`: Fully explored.
   If a DFS traversal encounters a `GRAY` node, a back-edge exists, indicating an **invalid feedback cycle**.
4. **Actionable Recommendations:** Outputs specific engineering guidance (e.g., "Connect missing input on pin 1 of Gate G2" or "Break cyclic feedback loop between G1 and G2").

### 3.2 Academic Test Scenarios Evaluated
1. **Scenario 1 (Valid Multi-Stage Cascade):** AND $\to$ OR $\to$ NOT. Acyclic, all inputs bound.
   - *Verdict:* `VALID`. Evaluation Readiness: `TRUE`.
2. **Scenario 2 (Incomplete / Dangling Inputs):** OR gate with floating pin 1.
   - *Verdict:* `WARNING` / `INVALID`. Action: Prompt user to tie pin to ground or signal.
3. **Scenario 3 (Cyclic Feedback Dependency):** G1 connected to G2, and G2 output connected back to G1 input.
   - *Verdict:* `INVALID`. Cycle Detected: `TRUE`. Action: Prohibit simulation until loop broken.

---

## 4. Computer Organization and Architecture — Task 4 (Hardware Integration)

Source: `COA/ALP/logic_ops_64.s`, `COA/Tests/Task4IntegrationTests.cpp`

### 4.1 Application-to-Processor Mapping
The application maps user-level logic gate evaluations directly to 64-bit AMD64 hardware instructions:

| Application Gate | Assembly Opcode | Native Instruction | Hardware Registers Used |
| :--- | :--- | :--- | :--- |
| `AND` | Opcode 1 | `and %rcx, %rax` | `%rax` (Result), `%rcx` (Input A), `%rdx` (Input B) |
| `OR` | Opcode 2 | `or %rcx, %rax` | `%rax` (Result), `%rcx` (Input A), `%rdx` (Input B) |
| `NOT` | Opcode 3 | `not %rax` + mask | `%rax` (Accumulator), bitwise NOT with 1-bit mask |
| `XOR` | Opcode 4 | `xor %rcx, %rax` | `%rax` (Result), `%rcx` (Input A), `%rdx` (Input B) |
| `NAND` | Opcode 5 | `and %rcx, %rax` + `not` | Two-stage instruction pipeline in `%rax` |
| `NOR` | Opcode 6 | `or %rcx, %rax` + `not` | Two-stage instruction pipeline in `%rax` |
| `XNOR` | Opcode 7 | `xor %rcx, %rax` + `not` | Two-stage instruction pipeline in `%rax` |

### 4.2 Register Flags & Conditional Branching
`asm_branch_verify_64(a, b, op)` demonstrates hardware condition codes:
- Uses `cmp` instruction to compare computed output with expected branch reference.
- Evaluates Zero Flag (`ZF`) using `je` (jump if equal) and `jne` (jump if not equal).
- Returns processor status codes (`100` for match, `200` for mismatch).

### 4.3 Successive Addition Delay & Truth Table Generation
- Multi-stage propagation delay is computed without hardware multiplier units via `asm_mul_successive_addition_64`:
  $$\text{Delay} = \sum_{i=1}^{\text{Stages}} \text{GateDelay}$$
- Truth table row capacity is doubled in a loop via `asm_compute_truth_table_rows_64` to demonstrate $2^N$ calculation using shift/add hardware registers.

---

## 5. Computer Graphics — Task 4 (Authentic Logic Gate Shapes & Coordinate Precision)

Source: `Computer Graphics/Rendering/GateRenderer.cpp`, `Computer Graphics/Canvas/Canvas.cpp`

### 5.1 Authentic ANSI/IEEE Std 91-1984 Logic Gate Symbol Rendering
Previous revisions rendered gates as rounded rectangular cards with text badges. Task 4 completely replaces this with authentic mathematical logic gate vector geometry rendered using core OpenGL primitives:

1. **AND Gate (`drawANDGate`):**
   - **Geometry:** Standard D-shaped body. Flat vertical input edge on the left, straight horizontal top and bottom segments, and a semi-circular curved output arc spanning from angle $-\pi/2$ to $+\pi/2$.
   - **Drawing Techniques:** `GL_QUADS` for the rectangular back half, `GL_TRIANGLE_FAN` for the sweeping curved front half, and `GL_LINE_STRIP` for crisp cyan-blue border definition.
2. **NAND Gate (`drawNANDGate`):**
   - **Geometry:** D-shaped body combined with a circular inversion bubble at the output apex.
   - **Drawing Techniques:** Same D-shape geometry terminating at $x_{\text{apex}} = x_{\text{right}} - 2 \cdot r_{\text{bubble}}$, followed by an inversion circle (`drawInversionBubble`) with dark background fill and crisp cyan stroke.
3. **OR Gate (`drawORGate`):**
   - **Geometry:** Concave curved input back arc that curves into the gate body towards the right, combined with two sweeping convex curves that converge at the rightmost apex.
   - **Drawing Techniques:** `GL_POLYGON` for dark translucent interior fill, `GL_LINE_STRIP` for smooth continuous perimeter outlines, and input pin wire stubs that extend mathematically to contact the curved back arc.
4. **NOR Gate (`drawNORGate`):**
   - **Geometry:** Standard OR-shaped body terminating with a circular inversion bubble at the apex.
   - **Drawing Techniques:** Continuous contour polygon with `drawInversionBubble` at the apex.
5. **NOT Gate (`drawNOTGate`):**
   - **Geometry:** Triangular inverter body pointing from left to right, terminating with a circular inversion bubble.
   - **Drawing Techniques:** `GL_TRIANGLES` for body fill, `GL_LINE_LOOP` for perimeter stroke, and circular bubble at the tip.
6. **XOR Gate (`drawXORGate`):**
   - **Geometry:** OR-shaped body shifted to the right, accompanied by an additional detached curved arc on the input side.
   - **Drawing Techniques:** Main body perimeter with `GL_POLYGON` + `GL_LINE_STRIP`, and detached input arc rendered with `GL_LINE_STRIP` at $x_{\text{left}}$. Input wire stubs terminate directly at the detached arc.
7. **XNOR Gate (`drawXNORGate`):**
   - **Geometry:** XOR-shaped body with detached input arc and a circular inversion bubble at the output apex.

### 5.2 Visual Styling & Workbench Theme
- **Theme:** Consistent dark electronic workbench aesthetic (`#07090d` background, `#080c12` substrate).
- **Body Styling:** Dark translucent navy fill (`rgba(12, 18, 28, 0.88)` unselected, `rgba(18, 34, 55, 0.92)` selected).
- **Glow & Outlines:** Crisp electric cyan border (`#38bdf8`) with a multi-pass widened outer glow on selection.
- **State Indicators:** Active HIGH (1) outputs glow bright emerald green (`#10b981`), while inactive LOW (0) outputs show technical slate grey (`#64748b`).
- **Typography:** Gate type symbols (`AND`, `OR`, `NAND`, etc.) rendered inside gate bodies using vector stroke fonts; component instance IDs (e.g., `NAND_G2`, `Primary_AND`) rendered below the gate in smaller font.

### 5.3 Sub-Pixel Pin Precision (`getGatePinPosition`)
- Every gate terminal (input and output) is aligned with sub-pixel precision ($\pm 0.001$ units).
- Output terminals are uniformly positioned at $(x + \text{halfW} + \text{pinLen}, y)$, ensuring connecting wires never glitch, jump, or exhibit gaps when changing gate types.

### 5.4 7-Gate Showcase & Interactive Controls
- Pressing `[7]` in the OpenGL viewport loads a complete 7-gate showcase displaying all 7 standard symbols simultaneously.
- Pressing `[4]` reverts to the 4-gate multi-stage cascade circuit.
- Pressing `[G]` dynamically cycles the selected gate through all 7 types (AND $\to$ NAND $\to$ OR $\to$ NOR $\to$ XOR $\to$ XNOR $\to$ NOT $\to$ AND).
- Pressing `[Spacebar]` toggles input pin states and re-evaluates logic in real time.

---

## 6. Problem Solving Using Java — Task 4 (File Handling & Java Generics)

Source: `java/src/logiccraft/file/ProjectFileManager.java`, `java/src/logiccraft/collections/ComponentList.java`

### 6.1 Circuit File Persistence (`saveCircuitToFile`, `loadCircuitFromFile`)
- Serializes complete circuit topologies into structured `.circ` text files containing gate definitions, coordinates, and wire netlists.
- Robust exception handling:
  - If a requested file does not exist, throws and catches `FileNotFoundException`.
  - If a file has corrupted syntax or missing tokens, throws `IOException` with detailed line number and token feedback without crashing the application.

### 6.2 Java Generics (`ComponentList<T>`)
- Reusable generic collection implementing `Iterable<T>`.
- Supports functional filtering via `filter(Predicate<T>)` (e.g., filtering multi-input gates vs single-input inverters).
- Fully integrated with the Swing desktop GUI (`LogicCraftUI`) with file save/load dialogs.

---

## 7. Full-Stack Web & Backend Integration

- **Backend:** `backend/server.js` provides REST endpoints:
  - `POST /api/search`: Executes procedural searching with comparison tracking.
  - `POST /api/sort`: Executes procedural sorting with metrics.
  - `GET/POST /api/hashtable`: Real-time open addressing hash table inspection and collision demos.
  - `POST /api/benchmark`: Runs dataset profiling suite across N=8, 16, 32.
  - `POST /api/validate`: Evaluates circuit topology and outputs smart recommendations.
  - `POST /api/circuit/export`: Generates downloadable `.circ` files.
- **Frontend Dashboard:** `frontend/index.html` and `app.js` feature a modern dark electronic-workbench theme, real-time HTML5 canvas rendering, interactive Task 4 panels, and authoritative CO mapping tables.
