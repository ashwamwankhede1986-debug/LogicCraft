# LogicCraft — Interdisciplinary Architecture Overview

---

## 1. High-Level Architectural Flow

LogicCraft bridges software layers from visual computer graphics down to processor micro-architecture:

```
+-------------------------------------------------------------------------+
|                  LAYER 1: COMPUTER GRAPHICS (OPENGL)                    |
|  - 2D Canvas & Engineering Grid                                         |
|  - IEEE Logic Symbol Primitives (AND, OR, NOT)                          |
|  - Interactive Transformations (Translation Dragging, Zoom Scaling)    |
|  - Orthographic Viewport & Boundary Clipping                            |
+-------------------------------------------------------------------------+
                                    │
                                    ▼
+-------------------------------------------------------------------------+
|                  LAYER 2: OBJECT-ORIENTED DOMAIN (JAVA)                 |
|  - Abstract LogicGate Hierarchy & Concrete Subclasses                   |
|  - Runtime Polymorphism & Dynamic Method Dispatch                       |
|  - Functional Interface & Lambda Predicates (Majority Gate & Filter)   |
|  - Circuit State Reporting via StringBuilder                            |
+-------------------------------------------------------------------------+
                                    │
                                    ▼
+-------------------------------------------------------------------------+
|                  LAYER 3: CIRCUIT ENGINE & STRUCTURES (C++)             |
|  - Contiguous Array Storage & CRUD Operations (CO1)                     |
|  - 2^N Boolean Permutations & Automated Truth Table Generation (CO1)   |
|  - Dynamic Heap-Allocated Singly-Linked List Component Engine (CO2)     |
|  - Inter-Gate Signal Routing & Forward Propagation                      |
+-------------------------------------------------------------------------+
                                    │
                                    ▼
+-------------------------------------------------------------------------+
|                  LAYER 4: PROCESSOR HARDWARE & ALP (COA)                |
|  - Register-Transfer Level (RTL) Micro-Operation Trace (CO1)            |
|  - Hardware Registers (R1, R2, R3, PC, IR) & Flags (ZF, SF, CF) (CO1)   |
|  - Native 64-Bit x86-64 Machine Instructions (MOV, AND, OR, CMP) (CO2)  |
|  - Conditional Branching (JE / JNE Control Flow Verification) (CO2)     |
+-------------------------------------------------------------------------+
```

---

## 2. Shared Data Contract (`shared/circuit_data.json`)

To ensure genuine integration across subjects, all four layers process the identical 3-gate reference circuit:

```json
{
  "circuitName": "LogicCraft Review-2 Reference Circuit",
  "version": "1.0",
  "gates": [
    { "id": "G1", "type": "AND", "inputs": [1, 0], "output": 0 },
    { "id": "G2", "type": "OR",  "inputs": [0, 1], "output": 1 },
    { "id": "G3", "type": "NOT", "inputs": [1],    "output": 0 }
  ],
  "connections": [
    { "sourceGateId": "G1", "destinationGateId": "G2", "destinationInputIndex": 0 },
    { "sourceGateId": "G2", "destinationGateId": "G3", "destinationInputIndex": 0 }
  ]
}
```

* **OpenGL Canvas:** Maps `G1`, `G2`, `G3` into visual interactive models on the grid.
* **Java Domain:** Models `G1`, `G2`, `G3` as polymorphic objects and generates audit logs.
* **C++ Engine:** Simulates signal propagation across the netlist using array and linked-list structures.
* **COA Simulator & Assembly:** Executes gate evaluation down to registers `R1`, `R2`, `R3` and native 64-bit ALU instructions.
